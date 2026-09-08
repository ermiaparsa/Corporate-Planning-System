#include <iostream>
#include <iomanip>
#include "corporate_planning/core/BalanceSheet.hpp"
#include "corporate_planning/core/IncomeStatement.hpp"  
#include "corporate_planning/core/FinancialRatios.hpp"  
#include "corporate_planning/core/ProductionLineInfo.hpp"
#include "corporate_planning/core/PersonnelInfo.hpp"
#include "corporate_planning/core/OtherInfo.hpp"
#include "corporate_planning/core/ManufacturingCompany.hpp"
#include "corporate_planning/core/User.hpp"
#include "corporate_planning/core/Equation.hpp"


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

     //--- OtherInfo Test ---
     corporate_planning::core::OtherInfo otherInfo;

     // ۱. نرخ بازگشت روی سرمایه‌گذاری
     otherInfo.setRoiRate(18.5);

     // ۲. متغیرهای ارزی
     otherInfo.setUnitForeignExchangeForProduction(12.5); // ارز به ازای هر واحد محصول (مثلاً دلار)
     otherInfo.setForeignExchangeUsed(125000.0);          // کل ارز مصرف‌شده

     // ۳. ارزش تولیدات
     otherInfo.setProductionValue(850000.0);

     // ۴. متغیرهای کمکی (Auxiliary Variables)
     corporate_planning::core::AuxiliaryVariables auxVars;
     auxVars.crisisPeriodFactor = 1.15;           // ضریب دوران بحران
     auxVars.administrativeExpensesFactor = 0.08; // ضریب هزینه‌های اداری
     auxVars.fxShortagePeriodFactor = 1.25;       // ضریب کاهش منابع ارزی
     auxVars.otherAuxiliaryFactor = 0.05;         // متغیر کمکی دیگر
     otherInfo.setAuxiliaryVariables(auxVars);

     // ۵. متغیر مربوط به زمان (مثلاً سال یا دوره زمانی)
     otherInfo.setTimeVariable(2026.0);

     cout << "--- OtherInfo ---\n";
     cout << "ROI rate: "
          << otherInfo.getRoiRate()
          << "%\n";

     cout << "Foreign exchange used: "
          << otherInfo.getForeignExchangeUsed()
          << '\n';

     cout << "Unit foreign exchange for production: "
          << otherInfo.getUnitForeignExchangeForProduction()
          << '\n';

     cout << "Production value: "
          << otherInfo.getProductionValue()
          << '\n';

     cout << "Crisis period factor: "
          << otherInfo.getAuxiliaryVariables().crisisPeriodFactor
          << '\n';

     cout << "FX shortage period factor: "
          << otherInfo.getAuxiliaryVariables().fxShortagePeriodFactor
          << '\n';

     cout << "Time variable: "
          << otherInfo.getTimeVariable()
          << '\n';

     //--- ManufacturingCompany Test ---
     corporate_planning::core::ManufacturingCompany company(
          "Novin Industrial Co.", "REG-1402-998", "Electronics Manufacturing", 2026
     );

     // ۱. مقداردهی اطلاعات مالی (ترازنامه و سود و زیان)
     company.getBalanceSheet().getAssets().setCashInBank(150000.0);
     company.getBalanceSheet().getAssets().setLand(300000.0);
     company.getBalanceSheet().getLiabilities().setAccountsPayable(45000.0);

     // ۲. مقداردهی پرسنل ستادی
     company.getPersonnelInfo().setAdministrativeStaff(10);
     company.getPersonnelInfo().setSalesAndDistributionStaff(15);
     company.getPersonnelInfo().setOtherStaff(5);

     // ۳. افزودن خطوط تولید داینامیک با وکتور
     // خط تولید اول: مدار الکترونیکی
     corporate_planning::core::ProductionLineInfo lineCircuit;
     lineCircuit.setWorkerCount(25);
     lineCircuit.setProductionAmount(2000.0);
     lineCircuit.setSalesAmount(1800.0);
     lineCircuit.setSalePrice(120.0);
     lineCircuit.setFixedCosts(35000.0);
     lineCircuit.setVariableCosts(80000.0);
     company.addProductionLine(lineCircuit);

     // خط تولید دوم: قطعات مکانیکی
     corporate_planning::core::ProductionLineInfo lineMech;
     lineMech.setWorkerCount(15);
     lineMech.setProductionAmount(1000.0);
     lineMech.setSalesAmount(900.0);
     lineMech.setSalePrice(95.0);
     lineMech.setFixedCosts(20000.0);
     lineMech.setVariableCosts(45000.0);
     company.addProductionLine(lineMech);

     cout << "--- ManufacturingCompany ---\n";
     cout << "Company name: "
          << company.getCompanyName()
          << '\n';

     cout << "Industry type: "
          << company.getIndustryType()
          << '\n';

     cout << "Fiscal year: "
          << company.getFiscalYear()
          << '\n';

     cout << "Production lines count: "
          << company.getProductionLineCount()
          << '\n';

     cout << "Total company revenue: "
          << company.calculateTotalCompanyRevenue()
          << '\n';

     cout << "Total production costs: "
          << company.calculateTotalProductionCosts()
          << '\n';

     cout << "Total company profit: "
          << company.calculateTotalCompanyProfit()
          << '\n';

     cout << "Total company employment: "
          << company.calculateTotalCompanyEmployment()
          << '\n';
     
     //--- User Test ---
     corporate_planning::core::User user(
          "USR-101",
          "Ermia Parsamanesh",
          "ermiaparsa",
          "hashed_sec_pass_2026",
          "parsamanesh.ermia@gmail.com",
          corporate_planning::core::UserRole::PLANNER
     );

     user.setLastLogin("2026-09-08 08:30:00");

     cout << "--- User ---\n";
     cout << "User ID: "
          << user.getUserId()
          << '\n';

     cout << "Full name: "
          << user.getFullName()
          << '\n';

     cout << "Username: "
          << user.getUsername()
          << '\n';

     cout << "Email: "
          << user.getEmail()
          << '\n';

     cout << "Role: "
          << user.getRoleString()
          << '\n';

     cout << "Is active: "
          << (user.getIsActive() ? "true" : "false")
          << '\n';

     cout << "Last login: "
          << user.getLastLogin()
          << '\n';

     cout << "Auth test (correct): "
          << (user.authenticate("ermiaparsa", "hashed_sec_pass_2026") ? "Success" : "Failed")
          << '\n';

     cout << "Auth test (wrong pwd): "
          << (user.authenticate("ermiaparsa", "wrong_pass") ? "Success" : "Failed")
          << '\n';

     //--- Equation Test ---
     corporate_planning::core::Equation costModel(
          "EQ-COST-01",
          "Total Production Cost Model",
          corporate_planning::core::EquationType::LINEAR,
          5000.0 // Intercept (هزینه ثابت پایه)
     );

     // افزودن ضرایب متغیرها (مثلاً: ۲۵ دلار برای هر ساعت کار، ۱۲.۵ دلار برای هر واحد مواد اولیه)
     costModel.addTerm("LaborHours", 25.0);
     costModel.addTerm("RawMaterialUnits", 12.5);

     // بردار ورودی برای تست: ۱۰۰ ساعت کار و ۲۰۰ واحد ماده اولیه
     vector<double> sampleInput = {100.0, 200.0};
     double predictedCost = costModel.evaluate(sampleInput);

     // محاسبه خطا با مقدار واقعی ثبت‌شده در بنگاه (مثلاً ۱۰۰۰۰ دلار)
     double actualCost = 10000.0;
     double residual = costModel.calculateResidual(sampleInput, actualCost);

     // تست دیتاست کوچک برای محاسبه MSE
     vector<vector<double>> dataset = {
          {100.0, 200.0},
          {120.0, 250.0},
          {80.0, 150.0}
     };
     vector<double> actuals = {10000.0, 11125.0, 8875.0};
     double mse = costModel.calculateMSE(dataset, actuals);

     cout << "--- Equation ---\n";
     cout << "Equation ID: "
          << costModel.getEquationId()
          << '\n';

     cout << "Equation name: "
          << costModel.getEquationName()
          << '\n';

     cout << "Equation type: "
          << costModel.getTypeString()
          << '\n';

     cout << "Formula: "
          << costModel.toFormulaString()
          << '\n';

     cout << "Predicted cost: "
          << predictedCost
          << '\n';

     cout << "Actual cost: "
          << actualCost
          << '\n';

     cout << "Residual (Error): "
          << residual
          << '\n';

     cout << "Dataset MSE: "
          << mse
          << '\n';

     return 0;

}
