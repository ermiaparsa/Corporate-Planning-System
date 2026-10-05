#ifndef CORPORATE_PLANNING_CORE_PRODUCTION_LINE_HPP
#define CORPORATE_PLANNING_CORE_PRODUCTION_LINE_HPP

#include <QString>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace corporate_planning::core {

// Operational and capacity record for a single fiscal year on a production line.
struct ProductionLineRecord {
    int fiscalYear = 0;
    std::string lineId;
    std::string lineName;

    // Direct operational inputs
    double nominalCapacity = 0.0;    // Nominal (Design) Capacity in units/year
    double actualOutput = 0.0;        // Actual Output Units produced
    double downtimeHours = 0.0;       // Total Downtime Hours in the period
    double defectUnits = 0.0;         // Defective / Scrap Units
    double directLaborHours = 0.0;    // Direct Labor Hours expended
    double maintenanceCost = 0.0;     // Total Maintenance Cost ($)

    // Baseline operational constants
    double plannedOperatingHours = 8760.0; // Standard annual planned operating hours (365 days * 24 h)
    double standardHourlyLaborRate = 25.0; // Standard labor wage rate ($ / hour)

    // Core Computations (populated via compute())
    double capacityUtilization = 0.0; // Capacity Utilization Rate (%)
    double defectRate = 0.0;          // Scrap / Defect Rate (%)
    double yieldRate = 100.0;         // Yield Rate (%) = Good Units / Actual Output
    double availabilityRate = 0.0;    // Operating Time / Planned Operating Time (%)
    double performanceRate = 0.0;     // Output Performance (%)
    double qualityRate = 100.0;       // Quality Rate (%)
    double oee = 0.0;                 // Overall Equipment Effectiveness approximation (%)
    double unitProductionCost = 0.0;  // Direct unit manufacturing cost ($ / unit)

    void compute();

    // Serialization methods (JSON & SQLite)
    QString toJson() const;
    static ProductionLineRecord fromJson(const QString& json);
    std::string toSqlInsertOrReplace() const;
    static std::string getSqlCreateTableScript();

    bool operator==(const ProductionLineRecord&) const = default;
};

// Represents a configured manufacturing production line and its historical operational records.
class ProductionLine {
public:
    ProductionLine() = default;
    ProductionLine(std::string id, std::string name, double nominalCapacity = 0.0);

    const std::string& getId() const;
    void setId(const std::string& id);

    const std::string& getName() const;
    void setName(const std::string& name);

    double getDefaultNominalCapacity() const;
    void setDefaultNominalCapacity(double capacity);

    void addOrUpdateRecord(const ProductionLineRecord& record);
    std::optional<ProductionLineRecord> getRecord(int year) const;
    bool removeRecord(int year);
    bool hasRecord(int year) const;
    std::vector<ProductionLineRecord> getAllRecords() const;
    std::size_t getRecordCount() const;

private:
    std::string id;
    std::string name;
    double defaultNominalCapacity = 0.0;
    std::map<int, ProductionLineRecord> records;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_PRODUCTION_LINE_HPP
