#include "corporate_planning/core/BalanceSheet.hpp"
#include <iostream>

using namespace std;

namespace corporate_planning::core {

BalanceSheet::BalanceSheet()
    : assets(),
      liabilities(),
      netWorth(0.0) {
}

double BalanceSheet::calculateTotalAssets() const {
return assets.calculateTotal();
}

double BalanceSheet::calculateTotalLiabilities() const {
return liabilities.calculateTotal();
}

double BalanceSheet::calculateEquity() const {
return netWorth;
}

Assets& BalanceSheet::getAssets() {
return assets;
}

Liabilities& BalanceSheet::getLiabilities() {
return liabilities;
}

void BalanceSheet::setNetWorth(double value) {
netWorth = value;
}

double BalanceSheet::getNetWorth() const {
return netWorth;
}

} // namespace corporate_planning::core
