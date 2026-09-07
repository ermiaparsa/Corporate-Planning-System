#include "corporate_planning/core/FinancialRatios.hpp"

namespace corporate_planning::core {

double FinancialRatios::safeDivide(double numerator, double denominator) {
    if (denominator == 0.0) {
        return 0.0;
    }
    return numerator / denominator;
}

// ۱. نسبت وجوه = (نقد + بانک) / بدهی جاری
double FinancialRatios::calculateCashRatio(const BalanceSheet& balanceSheet) const {
    const auto& assets = balanceSheet.getAssets();
    const auto& liabilities = balanceSheet.getLiabilities();

    double cashAndBank = assets.getCashOnHand() + assets.getCashInBank();
    double currentLiabilities = liabilities.calculateCurrentLiabilities();

    return safeDivide(cashAndBank, currentLiabilities);
}

// ۲. نسبت جاری = دارایی جاری / بدهی جاری
double FinancialRatios::calculateCurrentRatio(const BalanceSheet& balanceSheet) const {
    double currentAssets = balanceSheet.getAssets().calculateCurrentAssets();
    double currentLiabilities = balanceSheet.getLiabilities().calculateCurrentLiabilities();

    return safeDivide(currentAssets, currentLiabilities);
}

// ۳. نسبت سریع = (دارایی جاری - موجودی کالا) / بدهی جاری
double FinancialRatios::calculateQuickRatio(const BalanceSheet& balanceSheet) const {
    const auto& assets = balanceSheet.getAssets();
    double quickAssets = assets.calculateCurrentAssets() - assets.calculateInventoryTotal();
    double currentLiabilities = balanceSheet.getLiabilities().calculateCurrentLiabilities();

    return safeDivide(quickAssets, currentLiabilities);
}

// ۴. نسبت بدهی = کل بدهی / کل دارایی
double FinancialRatios::calculateDebtRatio(const BalanceSheet& balanceSheet) const {
    double totalLiabilities = balanceSheet.calculateTotalLiabilities();
    double totalAssets = balanceSheet.calculateTotalAssets();

    return safeDivide(totalLiabilities, totalAssets);
}

// ۵. گردش موجودی کالا = قیمت تمام شده کالای فروش رفته / موجودی کالا
double FinancialRatios::calculateInventoryTurnover(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const {
    double cogs = incomeStatement.calculateCostOfGoodsSold();
    double inventory = balanceSheet.getAssets().calculateInventoryTotal();

    return safeDivide(cogs, inventory);
}

// ۶. نسبت سود به فروش = سود قبل از مالیات / فروش
double FinancialRatios::calculateProfitMargin(const IncomeStatement& incomeStatement) const {
    double profit = incomeStatement.calculatePreTaxProfit();
    double sales = incomeStatement.getSales();

    return safeDivide(profit, sales);
}

// ۷. گردش دارایی‌های ثابت = فروش / کل دارایی ثابت
double FinancialRatios::calculateFixedAssetTurnover(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const {
    double sales = incomeStatement.getSales();
    double fixedAssets = balanceSheet.getAssets().calculateFixedAssetsTotal();

    return safeDivide(sales, fixedAssets);
}

// ۸. گردش کل دارایی‌ها = فروش / کل دارایی‌ها
double FinancialRatios::calculateTotalAssetTurnover(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const {
    double sales = incomeStatement.getSales();
    double totalAssets = balanceSheet.calculateTotalAssets();

    return safeDivide(sales, totalAssets);
}

// ۹. بازده کل دارایی‌ها = سود قبل از مالیات / کل دارایی‌ها
double FinancialRatios::calculateReturnOnAssets(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const {
    double profit = incomeStatement.calculatePreTaxProfit();
    double totalAssets = balanceSheet.calculateTotalAssets();

    return safeDivide(profit, totalAssets);
}

} // namespace corporate_planning::core
