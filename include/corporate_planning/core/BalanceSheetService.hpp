#ifndef CORPORATE_PLANNING_CORE_BALANCE_SHEET_SERVICE_HPP
#define CORPORATE_PLANNING_CORE_BALANCE_SHEET_SERVICE_HPP

#include "corporate_planning/core/BalanceSheetRecord.hpp"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace corporate_planning::core {

struct BalanceSheetSaveResult {
    bool success = false;
    bool created = false;
    std::string message;
};

// Owns the in-memory multi-year balance sheet store. Records are keyed by
// fiscal year, so saving an existing year updates it in place.
class BalanceSheetService {
public:
    static constexpr int MIN_FISCAL_YEAR = 1990;
    static constexpr int MAX_FISCAL_YEAR = 2100;

    BalanceSheetSaveResult saveRecord(const BalanceSheetRecord& record);
    std::optional<BalanceSheetRecord> getRecordByYear(int year) const;

    // Sorted by fiscal year in ascending order.
    std::vector<BalanceSheetRecord> getAllRecords() const;

    bool deleteRecord(int year);
    bool hasRecord(int year) const;
    std::size_t getRecordCount() const;

private:
    std::map<int, BalanceSheetRecord> records;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_BALANCE_SHEET_SERVICE_HPP
