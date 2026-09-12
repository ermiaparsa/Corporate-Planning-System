#include "corporate_planning/core/BalanceSheetRecord.hpp"

#include <cmath>

namespace corporate_planning::core {

double BalanceSheetRecord::totalCurrentAssets() const {
    return cashAndEquivalents
         + accountsReceivable
         + inventory
         + shortTermInvestments
         + otherCurrentAssets;
}

double BalanceSheetRecord::totalNonCurrentAssets() const {
    return propertyPlantEquipment
         + intangibleAssets
         + longTermInvestments;
}

double BalanceSheetRecord::totalAssets() const {
    return totalCurrentAssets() + totalNonCurrentAssets();
}

double BalanceSheetRecord::totalCurrentLiabilities() const {
    return accountsPayable
         + shortTermDebt
         + accruedExpenses;
}

double BalanceSheetRecord::totalNonCurrentLiabilities() const {
    return longTermDebt
         + deferredTaxLiabilities;
}

double BalanceSheetRecord::totalLiabilities() const {
    return totalCurrentLiabilities() + totalNonCurrentLiabilities();
}

double BalanceSheetRecord::totalEquity() const {
    return commonStock
         + retainedEarnings
         + additionalPaidInCapital;
}

double BalanceSheetRecord::balanceDifference() const {
    return totalAssets() - (totalLiabilities() + totalEquity());
}

bool BalanceSheetRecord::isBalanced(double tolerance) const {
    return std::abs(balanceDifference()) <= tolerance;
}

} // namespace corporate_planning::core
