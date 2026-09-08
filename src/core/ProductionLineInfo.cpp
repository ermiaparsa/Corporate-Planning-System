#include "corporate_planning/core/ProductionLineInfo.hpp"
#include <iostream>

namespace corporate_planning::core {

OtherVariables::OtherVariables()
    : workerWageAmount(0.0),
      rawMaterialPurchasePrice(0.0),
      capitalDepreciationAmount(0.0) {
}

ProductionLineInfo::ProductionLineInfo()
    : rawMaterialsCost(0.0),
      laborCost(0.0),
      variableCosts(0.0),
      fixedCosts(0.0),
      totalCosts(0.0),
      machinery(0.0),
      workerCount(0),
      depreciationAmount(0.0),
      salesAmount(0.0),
      productionAmount(0.0),
      salePrice(0.0),
      otherVariables() {
}

// Getters
double ProductionLineInfo::getRawMaterialsCost() const { return rawMaterialsCost; }
double ProductionLineInfo::getLaborCost() const { return laborCost; }
double ProductionLineInfo::getVariableCosts() const { return variableCosts; }
double ProductionLineInfo::getFixedCosts() const { return fixedCosts; }
double ProductionLineInfo::getTotalCosts() const { return totalCosts; }
double ProductionLineInfo::getMachinery() const { return machinery; }
int ProductionLineInfo::getWorkerCount() const { return workerCount; }
double ProductionLineInfo::getDepreciationAmount() const { return depreciationAmount; }
double ProductionLineInfo::getSalesAmount() const { return salesAmount; }
double ProductionLineInfo::getProductionAmount() const { return productionAmount; }
double ProductionLineInfo::getSalePrice() const { return salePrice; }
const OtherVariables& ProductionLineInfo::getOtherVariables() const { return otherVariables; }

// Setters
void ProductionLineInfo::setRawMaterialsCost(double value) { rawMaterialsCost = value; }
void ProductionLineInfo::setLaborCost(double value) { laborCost = value; }
void ProductionLineInfo::setVariableCosts(double value) { variableCosts = value; }
void ProductionLineInfo::setFixedCosts(double value) { fixedCosts = value; }
void ProductionLineInfo::setTotalCosts(double value) { totalCosts = value; }
void ProductionLineInfo::setMachinery(double value) { machinery = value; }
void ProductionLineInfo::setWorkerCount(int value) { workerCount = value; }
void ProductionLineInfo::setDepreciationAmount(double value) { depreciationAmount = value; }
void ProductionLineInfo::setSalesAmount(double value) { salesAmount = value; }
void ProductionLineInfo::setProductionAmount(double value) { productionAmount = value; }
void ProductionLineInfo::setSalePrice(double value) { salePrice = value; }
void ProductionLineInfo::setOtherVariables(const OtherVariables& value) { otherVariables = value; }

// Helper
double ProductionLineInfo::safeDivide(double numerator, double denominator) {
    if (denominator == 0.0) {
        return 0.0;
    }
    return numerator / denominator;
}

// Calculations
double ProductionLineInfo::calculateTotalCosts() const {
    if (totalCosts > 0.0) {
        return totalCosts;
    }
    return fixedCosts + variableCosts;
}

double ProductionLineInfo::calculateRevenue() const {
    return salesAmount * salePrice;
}

double ProductionLineInfo::calculateUnitCost() const {
    return safeDivide(calculateTotalCosts(), productionAmount);
}

double ProductionLineInfo::calculateUnitProfit() const {
    return salePrice - calculateUnitCost();
}

double ProductionLineInfo::calculateCapacityUtilization() const {
    return safeDivide(salesAmount, productionAmount) * 100.0;
}

double ProductionLineInfo::calculateLaborCostFromRate() const {
    return workerCount * otherVariables.workerWageAmount;
}

double ProductionLineInfo::calculateRawMaterialsCostFromRate() const {
    return productionAmount * otherVariables.rawMaterialPurchasePrice;
}

} // namespace corporate_planning::core
