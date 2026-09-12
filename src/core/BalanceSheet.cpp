#include "corporate_planning/core/BalanceSheet.hpp"

namespace corporate_planning::core {

BalanceSheet::BalanceSheet()
    : assets(),
      liabilities() {
}

double BalanceSheet::calculateTotalAssets() const {
    return assets.calculateTotal();
}

double BalanceSheet::calculateTotalLiabilities() const {
    return liabilities.calculateTotal();
}

double BalanceSheet::calculateEquity() const {
    return calculateTotalAssets() - calculateTotalLiabilities();
}

Assets& BalanceSheet::getAssets() {
    return assets;
}

const Assets& BalanceSheet::getAssets() const {
    return assets;
}

Liabilities& BalanceSheet::getLiabilities() {
    return liabilities;
}

const Liabilities& BalanceSheet::getLiabilities() const {
    return liabilities;
}

} // namespace corporate_planning::core
