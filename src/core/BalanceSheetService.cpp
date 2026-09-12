#include "corporate_planning/core/BalanceSheetService.hpp"

namespace corporate_planning::core {

BalanceSheetSaveResult BalanceSheetService::saveRecord(const BalanceSheetRecord& record) {
    BalanceSheetSaveResult result;

    if (record.fiscalYear < MIN_FISCAL_YEAR || record.fiscalYear > MAX_FISCAL_YEAR) {
        result.message = "Fiscal year must be between "
            + std::to_string(MIN_FISCAL_YEAR) + " and "
            + std::to_string(MAX_FISCAL_YEAR) + ".";
        return result;
    }

    result.created = records.find(record.fiscalYear) == records.end();
    records[record.fiscalYear] = record;
    result.success = true;
    result.message = "Record for fiscal year " + std::to_string(record.fiscalYear)
        + (result.created ? " saved successfully." : " updated successfully.");
    return result;
}

std::optional<BalanceSheetRecord> BalanceSheetService::getRecordByYear(int year) const {
    const auto it = records.find(year);
    if (it == records.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<BalanceSheetRecord> BalanceSheetService::getAllRecords() const {
    std::vector<BalanceSheetRecord> result;
    result.reserve(records.size());
    for (const auto& [year, record] : records) {
        result.push_back(record);
    }
    return result;
}

bool BalanceSheetService::deleteRecord(int year) {
    return records.erase(year) > 0;
}

bool BalanceSheetService::hasRecord(int year) const {
    return records.find(year) != records.end();
}

std::size_t BalanceSheetService::getRecordCount() const {
    return records.size();
}

} // namespace corporate_planning::core
