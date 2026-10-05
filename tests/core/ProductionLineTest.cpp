#include "corporate_planning/core/ProductionLine.hpp"
#include "corporate_planning/core/ProductionService.hpp"
#include <iostream>

using namespace std;

#include <cassert>
#include <cmath>

bool approxEqual(double a, double b, double epsilon = 1e-4) {
    return std::fabs(a - b) < epsilon;
}

void testProductionLineRecordComputations() {
    using namespace corporate_planning::core;

    ProductionLineRecord record;
    record.fiscalYear = 2026;
    record.lineId = "PL-TEST";
    record.lineName = "Test Production Line";
    record.nominalCapacity = 100000.0;
    record.actualOutput = 80000.0;
    record.downtimeHours = 876.0;   // 10% downtime of 8760 planned hours
    record.defectUnits = 1600.0;     // 2% defect rate of 80000 actual output
    record.directLaborHours = 40000.0;
    record.maintenanceCost = 100000.0;
    record.plannedOperatingHours = 8760.0;
    record.standardHourlyLaborRate = 25.0;

    record.compute();

    // 1. Capacity Utilization = 80,000 / 100,000 = 80.0%
    assert(approxEqual(record.capacityUtilization, 80.0));

    // 2. Defect Rate = 1,600 / 80,000 = 2.0%
    assert(approxEqual(record.defectRate, 2.0));

    // 3. Yield Rate = (80,000 - 1,600) / 80,000 = 98.0%
    assert(approxEqual(record.yieldRate, 98.0));

    // 4. OEE Availability = (8760 - 876) / 8760 = 90.0%
    assert(approxEqual(record.availabilityRate, 90.0));

    // Performance = 80,000 / 100,000 = 80.0%
    assert(approxEqual(record.performanceRate, 80.0));

    // Quality = 98.0%
    assert(approxEqual(record.qualityRate, 98.0));

    // OEE = 0.90 * 0.80 * 0.98 * 100 = 70.56%
    assert(approxEqual(record.oee, 70.56));

    // 5. Unit Production Cost = (100,000 + 40,000 * 25.0) / 80,000 = (100,000 + 1,000,000) / 80,000 = 1,100,000 / 80,000 = 13.75
    assert(approxEqual(record.unitProductionCost, 13.75));

    cout << "[PASS] testProductionLineRecordComputations" << endl;
}

void testSerialization() {
    using namespace corporate_planning::core;

    ProductionLineRecord record;
    record.fiscalYear = 2025;
    record.lineId = "PL-01";
    record.lineName = "Assembly Line Alpha";
    record.nominalCapacity = 100000.0;
    record.actualOutput = 95000.0;
    record.downtimeHours = 200.0;
    record.defectUnits = 500.0;
    record.directLaborHours = 45000.0;
    record.maintenanceCost = 140000.0;
    record.compute();

    // JSON Serialization
    const QString json = record.toJson();
    assert(!json.isEmpty());

    const ProductionLineRecord restored = ProductionLineRecord::fromJson(json);
    assert(restored.fiscalYear == 2025);
    assert(restored.lineId == "PL-01");
    assert(approxEqual(restored.nominalCapacity, 100000.0));
    assert(approxEqual(restored.actualOutput, 95000.0));
    assert(approxEqual(restored.capacityUtilization, record.capacityUtilization));
    assert(approxEqual(restored.oee, record.oee));

    // SQLite SQL Serialization
    const string sql = record.toSqlInsertOrReplace();
    assert(sql.find("INSERT OR REPLACE INTO production_line_records") != string::npos);
    assert(sql.find("2025") != string::npos);
    assert(sql.find("PL-01") != string::npos);

    const string ddl = ProductionLineRecord::getSqlCreateTableScript();
    assert(ddl.find("CREATE TABLE IF NOT EXISTS") != string::npos);

    cout << "[PASS] testSerialization" << endl;
}

void testProductionService() {
    using namespace corporate_planning::core;

    ProductionService service;

    // Check pre-seeded default lines
    const auto lines = service.getAllLines();
    assert(!lines.empty());

    // Check 10-year historical span retrieval
    const auto hist = service.getHistoricalRecordsForSpan("PL-01", 2026, 10);
    assert(hist.size() == 10);
    assert(hist.front().fiscalYear == 2017);
    assert(hist.back().fiscalYear == 2026);

    // Save and update record
    ProductionLineRecord newRec;
    newRec.fiscalYear = 2027;
    newRec.lineId = "PL-01";
    newRec.nominalCapacity = 105000.0;
    newRec.actualOutput = 102000.0;
    newRec.compute();

    auto saveRes = service.saveRecord(newRec);
    assert(saveRes.success);
    assert(saveRes.created);

    // Retrieve
    auto fetched = service.getRecord("PL-01", 2027);
    assert(fetched.has_value());
    assert(approxEqual(fetched->actualOutput, 102000.0));

    // Delete
    assert(service.deleteRecord("PL-01", 2027));
    assert(!service.hasRecord("PL-01", 2027));

    // Service JSON export / import
    const QString serviceJson = service.exportToJson("PL-01");
    assert(!serviceJson.isEmpty());

    ProductionService importService;
    assert(importService.importFromJson(serviceJson));
    assert(importService.hasRecord("PL-01", 2026));

    // Service SQL export
    const string sqlDump = service.exportToSqlScript("PL-01");
    assert(!sqlDump.empty());
    assert(sqlDump.find("INSERT OR REPLACE") != string::npos);

    cout << "[PASS] testProductionService" << endl;
}

int main() {
    cout << "Running ProductionLine Core Tests..." << endl;
    testProductionLineRecordComputations();
    testSerialization();
    testProductionService();
    cout << "All ProductionLine tests passed successfully!" << endl;
    return 0;
}
