#ifndef CORPORATE_PLANNING_CORE_FinancialRatios_HPP
#define CORPORATE_PLANNING_CORE_FinancialRatios_HPP

#include "corporate_planning/core/BalanceSheet.hpp"
#include "corporate_planning/core/IncomeStatement.hpp"

namespace corporate_planning::core {

class FinancialRatios {
public:
    FinancialRatios() = default;
    
     // ۱. نسبت وجوه (Cash Ratio)
    double calculateCashRatio(const BalanceSheet& balanceSheet) const;

    // ۲. نسبت جاری (Current Ratio)
    double calculateCurrentRatio(const BalanceSheet& balanceSheet) const;

    // ۳. نسبت سریع / آنی (Quick Ratio)
    double calculateQuickRatio(const BalanceSheet& balanceSheet) const;

    // ۴. نسبت بدهی (Debt Ratio)
    double calculateDebtRatio(const BalanceSheet& balanceSheet) const;

    // ۵. گردش موجودی کالا (Inventory Turnover)
    double calculateInventoryTurnover(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const;

    // ۶. نسبت سود به فروش (Profit Margin)
    double calculateProfitMargin(const IncomeStatement& incomeStatement) const;

    // ۷. گردش دارایی‌های ثابت (Fixed Asset Turnover)
    double calculateFixedAssetTurnover(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const;

    // ۸. گردش کل دارایی‌ها (Total Asset Turnover)
    double calculateTotalAssetTurnover(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const;

    // ۹. بازده کل دارایی‌ها (Return on Assets - ROA)
    double calculateReturnOnAssets(const BalanceSheet& balanceSheet, const IncomeStatement& incomeStatement) const;

private:
    static double safeDivide(double numerator, double denominator);
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_FinancialRatios_HPP
