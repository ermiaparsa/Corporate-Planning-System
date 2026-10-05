#include "corporate_planning/core/ProductionLine.hpp"
#include <iostream>

using namespace std;

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <QJsonDocument>
#include <QJsonObject>

namespace corporate_planning::core {

void ProductionLineRecord::compute() {
    // 1. Capacity Utilization Rate (%)
    capacityUtilization = nominalCapacity > 0.0
        ? (actualOutput / nominalCapacity) * 100.0
        : 0.0;

    // 2. Scrap / Defect Rate (%)
    defectRate = actualOutput > 0.0
        ? (defectUnits / actualOutput) * 100.0
        : 0.0;

    // 3. Yield Rate (%)
    yieldRate = actualOutput > 0.0
        ? clamp((actualOutput - defectUnits) / actualOutput, 0.0, 1.0) * 100.0
        : 100.0;

    // 4. OEE (Overall Equipment Effectiveness) approximation:
    // Availability = (Planned Operating Time - Downtime) / Planned Operating Time
    availabilityRate = plannedOperatingHours > 0.0
        ? clamp((plannedOperatingHours - downtimeHours) / plannedOperatingHours, 0.0, 1.0) * 100.0
        : 100.0;

    // Performance = Actual Output / Nominal Capacity
    performanceRate = nominalCapacity > 0.0
        ? clamp(actualOutput / nominalCapacity, 0.0, 1.0) * 100.0
        : 0.0;

    // Quality = (Actual Output - Defects) / Actual Output
    qualityRate = actualOutput > 0.0
        ? clamp((actualOutput - defectUnits) / actualOutput, 0.0, 1.0) * 100.0
        : 100.0;

    // OEE = Availability * Performance * Quality
    oee = clamp((availabilityRate / 100.0) * (performanceRate / 100.0) * (qualityRate / 100.0) * 100.0, 0.0, 100.0);

    // 5. Unit Production Cost ($ / unit)
    unitProductionCost = actualOutput > 0.0
        ? (maintenanceCost + directLaborHours * standardHourlyLaborRate) / actualOutput
        : 0.0;
}

QString ProductionLineRecord::toJson() const {
    QJsonObject obj;
    obj["fiscalYear"] = fiscalYear;
    obj["lineId"] = QString::fromStdString(lineId);
    obj["lineName"] = QString::fromStdString(lineName);
    obj["nominalCapacity"] = nominalCapacity;
    obj["actualOutput"] = actualOutput;
    obj["downtimeHours"] = downtimeHours;
    obj["defectUnits"] = defectUnits;
    obj["directLaborHours"] = directLaborHours;
    obj["maintenanceCost"] = maintenanceCost;
    obj["plannedOperatingHours"] = plannedOperatingHours;
    obj["standardHourlyLaborRate"] = standardHourlyLaborRate;

    // Computed
    obj["capacityUtilization"] = capacityUtilization;
    obj["defectRate"] = defectRate;
    obj["yieldRate"] = yieldRate;
    obj["availabilityRate"] = availabilityRate;
    obj["performanceRate"] = performanceRate;
    obj["qualityRate"] = qualityRate;
    obj["oee"] = oee;
    obj["unitProductionCost"] = unitProductionCost;

    const QJsonDocument doc(obj);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

ProductionLineRecord ProductionLineRecord::fromJson(const QString& json) {
    ProductionLineRecord record;
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject()) {
        return record;
    }

    const QJsonObject obj = doc.object();
    record.fiscalYear = obj.value("fiscalYear").toInt();
    record.lineId = obj.value("lineId").toString().toStdString();
    record.lineName = obj.value("lineName").toString().toStdString();
    record.nominalCapacity = obj.value("nominalCapacity").toDouble();
    record.actualOutput = obj.value("actualOutput").toDouble();
    record.downtimeHours = obj.value("downtimeHours").toDouble();
    record.defectUnits = obj.value("defectUnits").toDouble();
    record.directLaborHours = obj.value("directLaborHours").toDouble();
    record.maintenanceCost = obj.value("maintenanceCost").toDouble();

    if (obj.contains("plannedOperatingHours")) {
        record.plannedOperatingHours = obj.value("plannedOperatingHours").toDouble();
    }
    if (obj.contains("standardHourlyLaborRate")) {
        record.standardHourlyLaborRate = obj.value("standardHourlyLaborRate").toDouble();
    }

    record.compute();
    return record;
}

std::string ProductionLineRecord::toSqlInsertOrReplace() const {
    ostringstream oss;
    oss << fixed << setprecision(4);
    oss << "INSERT OR REPLACE INTO production_line_records ("
        << "fiscal_year, line_id, line_name, nominal_capacity, actual_output, "
        << "downtime_hours, defect_units, direct_labor_hours, maintenance_cost, "
        << "capacity_utilization, defect_rate, yield_rate, oee, unit_production_cost"
        << ") VALUES ("
        << fiscalYear << ", "
        << "'" << lineId << "', "
        << "'" << lineName << "', "
        << nominalCapacity << ", "
        << actualOutput << ", "
        << downtimeHours << ", "
        << defectUnits << ", "
        << directLaborHours << ", "
        << maintenanceCost << ", "
        << capacityUtilization << ", "
        << defectRate << ", "
        << yieldRate << ", "
        << oee << ", "
        << unitProductionCost
        << ");";
    return oss.str();
}

std::string ProductionLineRecord::getSqlCreateTableScript() {
    return "CREATE TABLE IF NOT EXISTS production_line_records (\n"
           "    fiscal_year INTEGER NOT NULL,\n"
           "    line_id TEXT NOT NULL,\n"
           "    line_name TEXT NOT NULL,\n"
           "    nominal_capacity REAL NOT NULL DEFAULT 0.0,\n"
           "    actual_output REAL NOT NULL DEFAULT 0.0,\n"
           "    downtime_hours REAL NOT NULL DEFAULT 0.0,\n"
           "    defect_units REAL NOT NULL DEFAULT 0.0,\n"
           "    direct_labor_hours REAL NOT NULL DEFAULT 0.0,\n"
           "    maintenance_cost REAL NOT NULL DEFAULT 0.0,\n"
           "    capacity_utilization REAL NOT NULL DEFAULT 0.0,\n"
           "    defect_rate REAL NOT NULL DEFAULT 0.0,\n"
           "    yield_rate REAL NOT NULL DEFAULT 0.0,\n"
           "    oee REAL NOT NULL DEFAULT 0.0,\n"
           "    unit_production_cost REAL NOT NULL DEFAULT 0.0,\n"
           "    PRIMARY KEY (line_id, fiscal_year)\n"
           ");";
}

// -----------------------------------------------------------------------------
// ProductionLine Implementation
// -----------------------------------------------------------------------------

ProductionLine::ProductionLine(string id, string name, double nominalCapacity)
    : id(std::move(id)),
      name(std::move(name)),
      defaultNominalCapacity(nominalCapacity) {
}

const string& ProductionLine::getId() const {
    return id;
}

void ProductionLine::setId(const string& newId) {
    id = newId;
}

const string& ProductionLine::getName() const {
    return name;
}

void ProductionLine::setName(const string& newName) {
    name = newName;
}

double ProductionLine::getDefaultNominalCapacity() const {
    return defaultNominalCapacity;
}

void ProductionLine::setDefaultNominalCapacity(double capacity) {
    defaultNominalCapacity = capacity;
}

void ProductionLine::addOrUpdateRecord(const ProductionLineRecord& record) {
    records[record.fiscalYear] = record;
}

optional<ProductionLineRecord> ProductionLine::getRecord(int year) const {
    const auto it = records.find(year);
    if (it == records.end()) {
        return nullopt;
    }
    return it->second;
}

bool ProductionLine::removeRecord(int year) {
    return records.erase(year) > 0;
}

bool ProductionLine::hasRecord(int year) const {
    return records.find(year) != records.end();
}

vector<ProductionLineRecord> ProductionLine::getAllRecords() const {
    vector<ProductionLineRecord> result;
    result.reserve(records.size());
    for (const auto& [year, record] : records) {
        result.push_back(record);
    }
    return result;
}

size_t ProductionLine::getRecordCount() const {
    return records.size();
}

} // namespace corporate_planning::core
