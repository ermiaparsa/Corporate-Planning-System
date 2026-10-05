#include "corporate_planning/core/ProductionService.hpp"
#include <iostream>

using namespace std;

#include <algorithm>
#include <sstream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

namespace corporate_planning::core {

ProductionService::ProductionService() {
    // Seed default enterprise production lines
    ProductionLine line1("PL-01", "Assembly Line Alpha", 100000.0);
    ProductionLine line2("PL-02", "Precision Machining Beta", 75000.0);
    ProductionLine line3("PL-03", "Fabrication & Stamping Gamma", 120000.0);
    ProductionLine line4("PL-04", "Finishing & Coating Delta", 80000.0);
    ProductionLine line5("PL-05", "Packaging & QA Epsilon", 150000.0);

    // Pre-populate 10-year historical records for Assembly Line Alpha (2017 - 2026)
    struct SeedEntry {
        int year;
        double actual;
        double downtime;
        double defects;
        double labor;
        double maint;
    };

    const vector<SeedEntry> seedData = {
        {2017, 78500.0, 420.0, 1200.0, 42000.0, 115000.0},
        {2018, 82100.0, 380.0, 1050.0, 43500.0, 122000.0},
        {2019, 86400.0, 340.0, 920.0,  44800.0, 128000.0},
        {2020, 74200.0, 650.0, 1350.0, 38000.0, 105000.0},
        {2021, 88900.0, 310.0, 810.0,  45200.0, 132000.0},
        {2022, 91500.0, 270.0, 740.0,  46000.0, 138000.0},
        {2023, 93800.0, 240.0, 690.0,  46800.0, 142000.0},
        {2024, 95200.0, 210.0, 620.0,  47100.0, 146000.0},
        {2025, 96800.0, 190.0, 580.0,  47500.0, 150000.0},
        {2026, 98200.0, 175.0, 510.0,  48000.0, 154000.0}
    };

    for (const auto& entry : seedData) {
        ProductionLineRecord rec;
        rec.fiscalYear = entry.year;
        rec.lineId = "PL-01";
        rec.lineName = "Assembly Line Alpha";
        rec.nominalCapacity = 100000.0;
        rec.actualOutput = entry.actual;
        rec.downtimeHours = entry.downtime;
        rec.defectUnits = entry.defects;
        rec.directLaborHours = entry.labor;
        rec.maintenanceCost = entry.maint;
        rec.compute();
        line1.addOrUpdateRecord(rec);
    }

    // Seed sample historical records for Machining Line Beta
    const vector<SeedEntry> betaData = {
        {2024, 68000.0, 290.0, 820.0, 36000.0, 95000.0},
        {2025, 71200.0, 260.0, 750.0, 37500.0, 98000.0},
        {2026, 73500.0, 230.0, 680.0, 38200.0, 102000.0}
    };
    for (const auto& entry : betaData) {
        ProductionLineRecord rec;
        rec.fiscalYear = entry.year;
        rec.lineId = "PL-02";
        rec.lineName = "Precision Machining Beta";
        rec.nominalCapacity = 75000.0;
        rec.actualOutput = entry.actual;
        rec.downtimeHours = entry.downtime;
        rec.defectUnits = entry.defects;
        rec.directLaborHours = entry.labor;
        rec.maintenanceCost = entry.maint;
        rec.compute();
        line2.addOrUpdateRecord(rec);
    }

    lines[line1.getId()] = line1;
    lines[line2.getId()] = line2;
    lines[line3.getId()] = line3;
    lines[line4.getId()] = line4;
    lines[line5.getId()] = line5;
}

ProductionSaveResult ProductionService::saveRecord(const ProductionLineRecord& record) {
    ProductionSaveResult result;

    if (record.fiscalYear < MIN_FISCAL_YEAR || record.fiscalYear > MAX_FISCAL_YEAR) {
        result.message = "Fiscal year must be between "
            + to_string(MIN_FISCAL_YEAR) + " and "
            + to_string(MAX_FISCAL_YEAR) + ".";
        return result;
    }

    if (record.lineId.empty()) {
        result.message = "Production line ID cannot be empty.";
        return result;
    }

    auto it = lines.find(record.lineId);
    if (it == lines.end()) {
        ProductionLine newLine(record.lineId, record.lineName.empty() ? record.lineId : record.lineName, record.nominalCapacity);
        lines[record.lineId] = newLine;
        it = lines.find(record.lineId);
    }

    ProductionLineRecord prepared = record;
    if (prepared.lineName.empty()) {
        prepared.lineName = it->second.getName();
    }
    prepared.compute();

    result.created = !it->second.hasRecord(prepared.fiscalYear);
    it->second.addOrUpdateRecord(prepared);

    result.success = true;
    result.message = "Record for Line " + prepared.lineId
        + " (" + to_string(prepared.fiscalYear) + ")"
        + (result.created ? " saved successfully." : " updated successfully.");

    return result;
}

optional<ProductionLineRecord> ProductionService::getRecord(const string& lineId, int year) const {
    const auto it = lines.find(lineId);
    if (it == lines.end()) {
        return nullopt;
    }
    return it->second.getRecord(year);
}

bool ProductionService::deleteRecord(const string& lineId, int year) {
    auto it = lines.find(lineId);
    if (it == lines.end()) {
        return false;
    }
    return it->second.removeRecord(year);
}

bool ProductionService::hasRecord(const string& lineId, int year) const {
    const auto it = lines.find(lineId);
    if (it == lines.end()) {
        return false;
    }
    return it->second.hasRecord(year);
}

vector<ProductionLineRecord> ProductionService::getRecordsByLine(const string& lineId) const {
    const auto it = lines.find(lineId);
    if (it == lines.end()) {
        return {};
    }
    auto records = it->second.getAllRecords();
    sort(records.begin(), records.end(), [](const ProductionLineRecord& a, const ProductionLineRecord& b) {
        return a.fiscalYear < b.fiscalYear;
    });
    return records;
}

vector<ProductionLineRecord> ProductionService::getAllRecords() const {
    vector<ProductionLineRecord> result;
    for (const auto& [id, line] : lines) {
        auto recs = line.getAllRecords();
        result.insert(result.end(), recs.begin(), recs.end());
    }
    sort(result.begin(), result.end(), [](const ProductionLineRecord& a, const ProductionLineRecord& b) {
        if (a.fiscalYear != b.fiscalYear) return a.fiscalYear < b.fiscalYear;
        return a.lineId < b.lineId;
    });
    return result;
}

vector<ProductionLineRecord> ProductionService::getHistoricalRecordsForSpan(
    const string& lineId,
    int endYear,
    int span) const
{
    const int startYear = endYear - span + 1;
    const auto lineRecords = getRecordsByLine(lineId);
    vector<ProductionLineRecord> result;
    for (const auto& rec : lineRecords) {
        if (rec.fiscalYear >= startYear && rec.fiscalYear <= endYear) {
            result.push_back(rec);
        }
    }
    return result;
}

vector<ProductionLine> ProductionService::getAllLines() const {
    vector<ProductionLine> result;
    result.reserve(lines.size());
    for (const auto& [id, line] : lines) {
        result.push_back(line);
    }
    return result;
}

optional<ProductionLine> ProductionService::getLine(const string& lineId) const {
    const auto it = lines.find(lineId);
    if (it == lines.end()) {
        return nullopt;
    }
    return it->second;
}

void ProductionService::registerLine(const ProductionLine& line) {
    lines[line.getId()] = line;
}

bool ProductionService::removeLine(const string& lineId) {
    return lines.erase(lineId) > 0;
}

size_t ProductionService::getRecordCount() const {
    size_t count = 0;
    for (const auto& [id, line] : lines) {
        count += line.getRecordCount();
    }
    return count;
}

size_t ProductionService::getRecordCount(const string& lineId) const {
    const auto it = lines.find(lineId);
    if (it == lines.end()) {
        return 0;
    }
    return it->second.getRecordCount();
}

QString ProductionService::exportToJson(const string& lineId) const {
    QJsonArray array;
    const auto records = lineId.empty() ? getAllRecords() : getRecordsByLine(lineId);
    for (const auto& record : records) {
        const QJsonDocument doc = QJsonDocument::fromJson(record.toJson().toUtf8());
        if (doc.isObject()) {
            array.append(doc.object());
        }
    }
    const QJsonDocument doc(array);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

bool ProductionService::importFromJson(const QString& json) {
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        return false;
    }

    const QJsonArray array = doc.array();
    for (const auto& val : array) {
        if (!val.isObject()) continue;
        const QJsonDocument singleDoc(val.toObject());
        const ProductionLineRecord record = ProductionLineRecord::fromJson(QString::fromUtf8(singleDoc.toJson(QJsonDocument::Compact)));
        if (!record.lineId.empty()) {
            saveRecord(record);
        }
    }
    return true;
}

string ProductionService::exportToSqlScript(const string& lineId) const {
    ostringstream oss;
    oss << "-- SQLite Database Dump for Corporate Planning: Production Lines\n";
    oss << ProductionLineRecord::getSqlCreateTableScript() << "\n\n";

    const auto records = lineId.empty() ? getAllRecords() : getRecordsByLine(lineId);
    for (const auto& record : records) {
        oss << record.toSqlInsertOrReplace() << "\n";
    }
    return oss.str();
}

bool ProductionService::importFromSqlScript(const string& sqlContent) {
    // Simple line parser for INSERT statements
    istringstream iss(sqlContent);
    string line;
    bool anyImported = false;

    while (getline(iss, line)) {
        if (line.rfind("INSERT", 0) == 0 || line.rfind("insert", 0) == 0) {
            // Find VALUES (...)
            const auto valPos = line.find("VALUES");
            if (valPos == string::npos) continue;

            const auto startParen = line.find('(', valPos);
            const auto endParen = line.rfind(')');
            if (startParen == string::npos || endParen == string::npos || endParen <= startParen) continue;

            const string valStr = line.substr(startParen + 1, endParen - startParen - 1);
            stringstream valStream(valStr);
            string item;
            vector<string> items;
            while (getline(valStream, item, ',')) {
                // trim
                size_t p1 = item.find_first_not_of(" \t\r\n'");
                size_t p2 = item.find_last_not_of(" \t\r\n'");
                if (p1 != string::npos && p2 != string::npos) {
                    items.push_back(item.substr(p1, p2 - p1 + 1));
                } else {
                    items.push_back("");
                }
            }

            if (items.size() >= 9) {
                try {
                    ProductionLineRecord rec;
                    rec.fiscalYear = stoi(items[0]);
                    rec.lineId = items[1];
                    rec.lineName = items[2];
                    rec.nominalCapacity = stod(items[3]);
                    rec.actualOutput = stod(items[4]);
                    rec.downtimeHours = stod(items[5]);
                    rec.defectUnits = stod(items[6]);
                    rec.directLaborHours = stod(items[7]);
                    rec.maintenanceCost = stod(items[8]);
                    rec.compute();
                    saveRecord(rec);
                    anyImported = true;
                } catch (...) {
                    // Ignore malformed line
                }
            }
        }
    }
    return anyImported;
}

} // namespace corporate_planning::core
