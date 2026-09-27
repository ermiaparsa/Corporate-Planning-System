#ifndef CORPORATE_PLANNING_CORE_INCOME_STATEMENT_RECORD_HPP
#define CORPORATE_PLANNING_CORE_INCOME_STATEMENT_RECORD_HPP

#include <QString>

namespace corporate_planning::core {

// Template 2: a single fiscal year of historical income statement data.
// All monetary fields are in base currency units (e.g., USD).
// taxRate is expressed as a percentage (0-100), e.g., 25.0 means 25%.
struct IncomeStatementRecord {
    int fiscalYear = 0;

    // --- Revenue ---
    double grossRevenue = 0.0;

    // --- Cost of Goods Sold (COGS) ---
    double costOfGoodsSold = 0.0;

    // --- Operating Expenses (OpEx) ---
    double sellingGeneralAndAdministrative = 0.0;  // SG&A
    double researchAndDevelopment = 0.0;           // R&D
    double depreciationAndAmortization = 0.0;      // D&A

    // --- Below Operating Income ---
    double interestExpense = 0.0;

    // --- Tax ---
    double taxRate = 0.0;  // percentage (e.g., 25.0 for 25%)

    // --- Computed (populated by compute()) ---
    double grossProfit = 0.0;
    double operatingIncome = 0.0;    // EBIT
    double earningsBeforeTax = 0.0;  // EBT
    double taxExpense = 0.0;
    double netIncome = 0.0;
    double netProfitMargin = 0.0;    // percentage

    // --- Public methods ---
    void compute();

    QString toJson() const;
    static IncomeStatementRecord fromJson(const QString& json);

    bool operator==(const IncomeStatementRecord&) const = default;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_INCOME_STATEMENT_RECORD_HPP
