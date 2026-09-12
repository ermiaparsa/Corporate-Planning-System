#ifndef CORPORATE_PLANNING_CORE_BALANCE_SHEET_RECORD_HPP
#define CORPORATE_PLANNING_CORE_BALANCE_SHEET_RECORD_HPP

namespace corporate_planning::core {

// Template 1: a single fiscal year of baseline historical balance sheet data.
// Line items are grouped exactly like the report layout so the GUI can bind
// to them without any remapping.
struct BalanceSheetRecord {
    int fiscalYear = 0;

    // Current assets
    double cashAndEquivalents = 0.0;
    double accountsReceivable = 0.0;
    double inventory = 0.0;
    double shortTermInvestments = 0.0;
    double otherCurrentAssets = 0.0;

    // Non-current assets
    double propertyPlantEquipment = 0.0;
    double intangibleAssets = 0.0;
    double longTermInvestments = 0.0;

    // Current liabilities
    double accountsPayable = 0.0;
    double shortTermDebt = 0.0;
    double accruedExpenses = 0.0;

    // Non-current liabilities
    double longTermDebt = 0.0;
    double deferredTaxLiabilities = 0.0;

    // Equity
    double commonStock = 0.0;
    double retainedEarnings = 0.0;
    double additionalPaidInCapital = 0.0;

    double totalCurrentAssets() const;
    double totalNonCurrentAssets() const;
    double totalAssets() const;

    double totalCurrentLiabilities() const;
    double totalNonCurrentLiabilities() const;
    double totalLiabilities() const;

    double totalEquity() const;

    // Total Assets - (Total Liabilities + Total Equity)
    double balanceDifference() const;
    bool isBalanced(double tolerance = 0.005) const;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_BALANCE_SHEET_RECORD_HPP
