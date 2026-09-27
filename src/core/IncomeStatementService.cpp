#include "corporate_planning/core/IncomeStatementService.hpp"

namespace corporate_planning::core {

IncomeStatementSaveResult IncomeStatementService::saveRecord(
    const IncomeStatementRecord& record)
{
    IncomeStatementSaveResult result;

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

std::optional<IncomeStatementRecord> IncomeStatementService::getRecordByYear(
    int year) const
{
    const auto it = records.find(year);
    if (it == records.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<IncomeStatementRecord> IncomeStatementService::getAllRecords() const {
    std::vector<IncomeStatementRecord> result;
    result.reserve(records.size());
    for (const auto& [year, record] : records) {
        result.push_back(record);
    }
    return result;
}

bool IncomeStatementService::deleteRecord(int year) {
    return records.erase(year) > 0;
}

bool IncomeStatementService::hasRecord(int year) const {
    return records.find(year) != records.end();
}

std::size_t IncomeStatementService::getRecordCount() const {
    return records.size();
}

} // namespace corporate_planning::core
