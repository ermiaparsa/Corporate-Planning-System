#include "corporate_planning/core/OtherInfo.hpp"
#include <iostream>


namespace corporate_planning::core {

AuxiliaryVariables::AuxiliaryVariables()
    : crisisPeriodFactor(0.0),
      administrativeExpensesFactor(0.0),
      fxShortagePeriodFactor(0.0),
      otherAuxiliaryFactor(0.0) {
}

OtherInfo::OtherInfo()
    : roiRate(0.0),
      foreignExchangeUsed(0.0),
      unitForeignExchangeForProduction(0.0),
      productionValue(0.0),
      auxiliaryVariables(),
      timeVariable(0.0) {
}

// Getters
double OtherInfo::getRoiRate() const {
    return roiRate;
}

double OtherInfo::getForeignExchangeUsed() const {
    return foreignExchangeUsed;
}

double OtherInfo::getUnitForeignExchangeForProduction() const {
    return unitForeignExchangeForProduction;
}

double OtherInfo::getProductionValue() const {
    return productionValue;
}

const AuxiliaryVariables& OtherInfo::getAuxiliaryVariables() const {
    return auxiliaryVariables;
}

AuxiliaryVariables& OtherInfo::getAuxiliaryVariables() {
    return auxiliaryVariables;
}

double OtherInfo::getTimeVariable() const {
    return timeVariable;
}

// Setters
void OtherInfo::setRoiRate(double value) {
    roiRate = value;
}

void OtherInfo::setForeignExchangeUsed(double value) {
    foreignExchangeUsed = value;
}

void OtherInfo::setUnitForeignExchangeForProduction(double value) {
    unitForeignExchangeForProduction = value;
}

void OtherInfo::setProductionValue(double value) {
    productionValue = value;
}

void OtherInfo::setAuxiliaryVariables(const AuxiliaryVariables& value) {
    auxiliaryVariables = value;
}

void OtherInfo::setTimeVariable(double value) {
    timeVariable = value;
}

// Helper
double OtherInfo::safeDivide(double numerator, double denominator) {
    if (denominator == 0.0) {
        return 0.0;
    }
    return numerator / denominator;
}

// Calculations
double OtherInfo::calculateForeignExchangeUsedFromRate(double productionAmount) const {
    return unitForeignExchangeForProduction * productionAmount;
}

double OtherInfo::calculateUnitForeignExchangeFromTotal(double productionAmount) const {
    return safeDivide(foreignExchangeUsed, productionAmount);
}

double OtherInfo::calculateProductionValue(double productionAmount, double unitPrice) const {
    return productionAmount * unitPrice;
}

} // namespace corporate_planning::core
