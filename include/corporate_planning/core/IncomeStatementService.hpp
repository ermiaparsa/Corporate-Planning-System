#ifndef CORPORATE_PLANNING_CORE_INCOME_STATEMENT_SERVICE_HPP
#define CORPORATE_PLANNING_CORE_INCOME_STATEMENT_SERVICE_HPP

#include "corporate_planning/core/IncomeStatementRecord.hpp"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace corporate_planning::core {

struct IncomeStatementSaveResult {
    bool success = false;
    bool created = false;
    std::string message;
};

// Owns the in-memory multi-year income statement store. Records are keyed by
// fiscal year, so saving an existing year updates it in place.
class IncomeStatementService {
public:
    static constexpr int MIN_FISCAL_YEAR = 1990;
    static constexpr int MAX_FISCAL_YEAR = 2100;

    IncomeStatementSaveResult saveRecord(const IncomeStatementRecord& record);
    std::optional<IncomeStatementRecord> getRecordByYear(int year) const;

    // Sorted by fiscal year in ascending order.
    std::vector<IncomeStatementRecord> getAllRecords() const;

    bool deleteRecord(int year);
    bool hasRecord(int year) const;
    std::size_t getRecordCount() const;

private:
    std::map<int, IncomeStatementRecord> records;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_INCOME_STATEMENT_SERVICE_HPP
