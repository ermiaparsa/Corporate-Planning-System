#include <iostream>
#include "corporate_planning/core/BalanceSheet.hpp"
#include "corporate_planning/core/IncomeStatement.hpp"  
#include "corporate_planning/core/FinancialRatios.hpp"  


using namespace std;

namespace corporate_planning::api {
    void runCli();
}

int main() {
    corporate_planning::api::runCli();

    cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl<<"Corporate Planning - Firm Planning Model\n";
    cout << "Version 1.0.0\n";
    cout << "Corporate Planning started successfully." <<endl;

    //--- BalanceSheet Test ---
    corporate_planning::core::BalanceSheet balanceSheet;

    balanceSheet.getAssets().setCashOnHand(10000.0);
    balanceSheet.getAssets().setCashInBank(25000.0);
    balanceSheet.getAssets().setLand(50000.0);

    balanceSheet.getLiabilities().setAccountsPayable(8000.0);
    balanceSheet.getLiabilities().setLongTermLoans(20000.0);

    cout << "--- BalanceSheet ---\n";
    cout << "Total assets: "
    << balanceSheet.calculateTotalAssets()
    << endl;

    cout << "Total liabilities: "
    << balanceSheet.calculateTotalLiabilities()<< endl;
    
    //--- IncomeStatement Test ---
     corporate_planning::core::IncomeStatement incomeStatement;

    // ۱. فروش
    incomeStatement.setSales(500000.0);

    // ۳. هزینه‌های توزیع و فروش
    incomeStatement.setDistributionAndSalesCosts(30000.0);

    // ۴. هزینه‌های اداری
    incomeStatement.setAdministrativeCosts(25000.0);

    // ۶. اجزای قیمت تمام‌شده کالای فروش‌رفته
    incomeStatement.setRawMaterials(120000.0);
    incomeStatement.setDirectLaborCosts(80000.0);
    incomeStatement.setInventoryAdjustments(10000.0);

    // ۶-۴. هزینه‌های دستمزد غیرمستقیم و سربار
    incomeStatement.setDepreciation(20000.0);
    incomeStatement.setUtilityCosts(15000.0);
    incomeStatement.setMaintenanceAndRepairs(12000.0);
    incomeStatement.setConsumableSupplies(8000.0);
    incomeStatement.setTransportationCosts(5000.0);

    // ۷. سایر درآمدها
    incomeStatement.setUnusedItemsSales(10000.0);
    incomeStatement.setAssetDisposalGains(15000.0);
    incomeStatement.setLoadingRevenues(5000.0);
    incomeStatement.setMiscellaneousIncomes(3000.0);

    // ۸. سایر هزینه‌ها
    incomeStatement.setLongTermLoanInterest(12000.0);
    incomeStatement.setInvestmentImpairmentLoss(4000.0);
    incomeStatement.setMiscellaneousExpenses(2000.0);

    cout << "--- IncomeStatement ---\n";
    cout << "Sales: "
         << incomeStatement.getSales()
         << '\n';

    cout << "Overhead costs: "
         << incomeStatement.calculateOverheadCosts()
         << '\n';

    cout << "Cost of goods sold: "
         << incomeStatement.calculateCostOfGoodsSold()
         << '\n';

    cout << "Gross profit: "
         << incomeStatement.calculateGrossProfit()
         << '\n';

    cout << "Distribution and sales costs: "
         << incomeStatement.getDistributionAndSalesCosts()
         << '\n';

    cout << "Administrative costs: "
         << incomeStatement.getAdministrativeCosts()
         << '\n';

    cout << "Other incomes: "
         << incomeStatement.calculateOtherIncomes()
         << '\n';

    cout << "Other expenses: "
         << incomeStatement.calculateOtherExpenses()
         << '\n';

    cout << "Pre-tax profit: "
         << incomeStatement.calculatePreTaxProfit()
         << '\n';

    
    // --- Financial Ratios Test ---
    corporate_planning::core::FinancialRatios ratios;

    cout << "--- Financial Ratios ---\n";
    cout << "1. Cash Ratio: " << ratios.calculateCashRatio(balanceSheet) << "\n";
    cout << "2. Current Ratio: " << ratios.calculateCurrentRatio(balanceSheet) << "\n";
    cout << "3. Quick Ratio: " << ratios.calculateQuickRatio(balanceSheet) << "\n";
    cout << "4. Debt Ratio: " << ratios.calculateDebtRatio(balanceSheet) << "\n";
    cout << "5. Inventory Turnover: " << ratios.calculateInventoryTurnover(balanceSheet, incomeStatement) << "\n";
    cout << "6. Profit Margin: " << ratios.calculateProfitMargin(incomeStatement) << "\n";
    cout << "7. Fixed Asset Turnover: " << ratios.calculateFixedAssetTurnover(balanceSheet, incomeStatement) << "\n";
    cout << "8. Total Asset Turnover: " << ratios.calculateTotalAssetTurnover(balanceSheet, incomeStatement) << "\n";
    cout << "9. Return on Assets (ROA): " << ratios.calculateReturnOnAssets(balanceSheet, incomeStatement) << "\n";


return 0;
}
