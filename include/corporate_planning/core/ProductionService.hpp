#ifndef CORPORATE_PLANNING_CORE_PRODUCTION_SERVICE_HPP
#define CORPORATE_PLANNING_CORE_PRODUCTION_SERVICE_HPP

#include "corporate_planning/core/ProductionLine.hpp"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace corporate_planning::core {

struct ProductionSaveResult {
    bool success = false;
    bool created = false;
    std::string message;
};

// Manages configured production lines and multi-year historical operational data.
class ProductionService {
public:
    static constexpr int MIN_FISCAL_YEAR = 1990;
    static constexpr int MAX_FISCAL_YEAR = 2100;
    static constexpr int HISTORICAL_YEAR_SPAN = 10;

    ProductionService();

    // Production Line Record CRUD
    ProductionSaveResult saveRecord(const ProductionLineRecord& record);
    std::optional<ProductionLineRecord> getRecord(const std::string& lineId, int year) const;
    bool deleteRecord(const std::string& lineId, int year);
    bool hasRecord(const std::string& lineId, int year) const;

    // Retrieval
    std::vector<ProductionLineRecord> getRecordsByLine(const std::string& lineId) const;
    std::vector<ProductionLineRecord> getAllRecords() const;
    std::vector<ProductionLineRecord> getHistoricalRecordsForSpan(
        const std::string& lineId,
        int endYear,
        int span = HISTORICAL_YEAR_SPAN
    ) const;

    // Line Entity Management
    std::vector<ProductionLine> getAllLines() const;
    std::optional<ProductionLine> getLine(const std::string& lineId) const;
    void registerLine(const ProductionLine& line);
    bool removeLine(const std::string& lineId);

    // Counts
    std::size_t getRecordCount() const;
    std::size_t getRecordCount(const std::string& lineId) const;

    // Serialization (JSON & SQLite)
    QString exportToJson(const std::string& lineId = "") const;
    bool importFromJson(const QString& json);
    std::string exportToSqlScript(const std::string& lineId = "") const;
    bool importFromSqlScript(const std::string& sqlContent);

private:
    std::map<std::string, ProductionLine> lines;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_PRODUCTION_SERVICE_HPP
