#include "corporate_planning/core/IncomeStatement.hpp"
#include <iostream>

namespace corporate_planning::core {

IncomeStatement::IncomeStatement()
    : sales(0.0),
      distributionAndSalesCosts(0.0),
      administrativeCosts(0.0),
      rawMaterials(0.0),
      directLaborCosts(0.0),
      inventoryAdjustments(0.0),
      depreciation(0.0),
      utilityCosts(0.0),
      maintenanceAndRepairs(0.0),
      consumableSupplies(0.0),
      transportationCosts(0.0),
      unusedItemsSales(0.0),
      assetDisposalGains(0.0),
      loadingRevenues(0.0),
      miscellaneousIncomes(0.0),
      longTermLoanInterest(0.0),
      investmentImpairmentLoss(0.0),
      miscellaneousExpenses(0.0) {
}

// ۶-۴. محاسبه مجموع دستمزد غیرمستقیم و سربار
double IncomeStatement::calculateOverheadCosts() const {
    return depreciation
         + utilityCosts
         + maintenanceAndRepairs
         + consumableSupplies
         + transportationCosts;
}

// ۶. محاسبه کل قیمت تمام شده کالای فروش رفته (COGS)
double IncomeStatement::calculateCostOfGoodsSold() const {
    return rawMaterials
         + directLaborCosts
         + inventoryAdjustments
         + calculateOverheadCosts();
}

// ۷. محاسبه مجموع سایر درآمدها
double IncomeStatement::calculateOtherIncomes() const {
    return unusedItemsSales
         + assetDisposalGains
         + loadingRevenues
         + miscellaneousIncomes;
}

// ۸. محاسبه مجموع سایر هزینه‌ها
double IncomeStatement::calculateOtherExpenses() const {
    return longTermLoanInterest
         + investmentImpairmentLoss
         + miscellaneousExpenses;
}

// ۲. محاسبه سود ناویژه (فروش منهای بهای تمام شده کالای فروش رفته)
double IncomeStatement::calculateGrossProfit() const {
    return sales - calculateCostOfGoodsSold();
}

// ۵. محاسبه سود ویژه قبل از مالیات
double IncomeStatement::calculatePreTaxProfit() const {
    return calculateGrossProfit()
         - (distributionAndSalesCosts + administrativeCosts)
         + calculateOtherIncomes()
         - calculateOtherExpenses();
}

// --- Getter & Setter ها ---

void IncomeStatement::setSales(double value) { sales = value; }
double IncomeStatement::getSales() const { return sales; }

void IncomeStatement::setDistributionAndSalesCosts(double value) { distributionAndSalesCosts = value; }
double IncomeStatement::getDistributionAndSalesCosts() const { return distributionAndSalesCosts; }

void IncomeStatement::setAdministrativeCosts(double value) { administrativeCosts = value; }
double IncomeStatement::getAdministrativeCosts() const { return administrativeCosts; }

void IncomeStatement::setRawMaterials(double value) { rawMaterials = value; }
double IncomeStatement::getRawMaterials() const { return rawMaterials; }

void IncomeStatement::setDirectLaborCosts(double value) { directLaborCosts = value; }
double IncomeStatement::getDirectLaborCosts() const { return directLaborCosts; }

void IncomeStatement::setInventoryAdjustments(double value) { inventoryAdjustments = value; }
double IncomeStatement::getInventoryAdjustments() const { return inventoryAdjustments; }

void IncomeStatement::setDepreciation(double value) { depreciation = value; }
double IncomeStatement::getDepreciation() const { return depreciation; }

void IncomeStatement::setUtilityCosts(double value) { utilityCosts = value; }
double IncomeStatement::getUtilityCosts() const { return utilityCosts; }

void IncomeStatement::setMaintenanceAndRepairs(double value) { maintenanceAndRepairs = value; }
double IncomeStatement::getMaintenanceAndRepairs() const { return maintenanceAndRepairs; }

void IncomeStatement::setConsumableSupplies(double value) { consumableSupplies = value; }
double IncomeStatement::getConsumableSupplies() const { return consumableSupplies; }

void IncomeStatement::setTransportationCosts(double value) { transportationCosts = value; }
double IncomeStatement::getTransportationCosts() const { return transportationCosts; }

void IncomeStatement::setUnusedItemsSales(double value) { unusedItemsSales = value; }
double IncomeStatement::getUnusedItemsSales() const { return unusedItemsSales; }

void IncomeStatement::setAssetDisposalGains(double value) { assetDisposalGains = value; }
double IncomeStatement::getAssetDisposalGains() const { return assetDisposalGains; }

void IncomeStatement::setLoadingRevenues(double value) { loadingRevenues = value; }
double IncomeStatement::getLoadingRevenues() const { return loadingRevenues; }

void IncomeStatement::setMiscellaneousIncomes(double value) { miscellaneousIncomes = value; }
double IncomeStatement::getMiscellaneousIncomes() const { return miscellaneousIncomes; }

void IncomeStatement::setLongTermLoanInterest(double value) { longTermLoanInterest = value; }
double IncomeStatement::getLongTermLoanInterest() const { return longTermLoanInterest; }

void IncomeStatement::setInvestmentImpairmentLoss(double value) { investmentImpairmentLoss = value; }
double IncomeStatement::getInvestmentImpairmentLoss() const { return investmentImpairmentLoss; }

void IncomeStatement::setMiscellaneousExpenses(double value) { miscellaneousExpenses = value; }
double IncomeStatement::getMiscellaneousExpenses() const { return miscellaneousExpenses; }

} // namespace corporate_planning::core
