#include <iostream>
#include <iomanip>
#include "corporate_planning/core/BalanceSheet.hpp"
#include "corporate_planning/core/IncomeStatement.hpp"  
#include "corporate_planning/core/FinancialRatios.hpp"  
#include "corporate_planning/core/ProductionLineInfo.hpp"
#include "corporate_planning/core/PersonnelInfo.hpp"



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


     //--- ProductionLineInfo Test ---
     corporate_planning::core::ProductionLineInfo productionLine;

     // ۱. تعریف متغیرهای دیگر (Other Variables)
     // (شامل ۳ زیربخش: دستمزد کارگر، قیمت خرید مواد اولیه، استهلاک سرمایه)
     corporate_planning::core::OtherVariables otherVars;
     otherVars.workerWageAmount = 1500.0;
     otherVars.rawMaterialPurchasePrice = 25.0;
     otherVars.capitalDepreciationAmount = 5000.0;
     productionLine.setOtherVariables(otherVars);

     // ۲. پارامترهای عملیاتی خط تولید
     productionLine.setWorkerCount(20);                 // تعداد کارگر
     productionLine.setProductionAmount(1000.0);     // میزان تولید (واحد)
     productionLine.setSalesAmount(950.0);           // میزان فروش (واحد)
     productionLine.setSalePrice(85.0);              // قیمت فروش هر واحد
     productionLine.setMachinery(250000.0);          // ارزش ماشین‌آلات
     productionLine.setDepreciationAmount(7500.0);    // میزان استهلاک

     // ۳. محاسبه و تنظیم هزینه‌ها
     productionLine.setLaborCost(productionLine.calculateLaborCostFromRate());          // دستمزد
     productionLine.setRawMaterialsCost(productionLine.calculateRawMaterialsCostFromRate()); // مواد اولیه
     productionLine.setVariableCosts(productionLine.getRawMaterialsCost() + 5000.0);  // متغیر = مواد + سربار متغیر
     productionLine.setFixedCosts(productionLine.getLaborCost() + productionLine.getDepreciationAmount() + 4500.0); // ثابت = دستمزد + استهلاک + اجاره
     productionLine.setTotalCosts(productionLine.calculateTotalCosts());

     cout << "--- ProductionLineInfo ---\n";

     cout << "Total costs: "
          << productionLine.calculateTotalCosts()
          << '\n';

     cout << "Revenue: "
          << productionLine.calculateRevenue()
          << '\n';

     cout << "Unit cost: "
          << productionLine.calculateUnitCost()
          << '\n';

     cout << "Unit profit: "
          << productionLine.calculateUnitProfit()
          << '\n';

     cout << "Capacity utilization: "
          << productionLine.calculateCapacityUtilization()
          << '%'
          << '\n';

     cout << "Labor cost (from rate): "
          << productionLine.calculateLaborCostFromRate()
          << '\n';

     cout << "Raw materials cost (from rate): "
          << productionLine.calculateRawMaterialsCostFromRate()
          << '\n';

     //--- PersonnelInfo Test ---
     corporate_planning::core::PersonnelInfo personnelInfo;

     // ۱. کادر تولید
     personnelInfo.setProductionStaff(45);

     // ۲. کادر توزیع و فروش
     personnelInfo.setSalesAndDistributionStaff(12);

     // ۳. کادر اداری
     personnelInfo.setAdministrativeStaff(8);

     // ۴. سایر پرسنل
     personnelInfo.setOtherStaff(5);

     cout << "--- PersonnelInfo ---\n";
     cout << "Production staff: "
          << personnelInfo.getProductionStaff()
          << '\n';

     cout << "Sales and distribution staff: "
          << personnelInfo.getSalesAndDistributionStaff()
          << '\n';

     cout << "Administrative staff: "
          << personnelInfo.getAdministrativeStaff()
          << '\n';

     cout << "Other staff: "
          << personnelInfo.getOtherStaff()
          << '\n';

     cout << "Total employment: "
          << personnelInfo.calculateTotalEmployment()
          << '\n';

          
     return 0;

}
