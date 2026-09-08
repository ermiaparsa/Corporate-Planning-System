#include "corporate_planning/core/PersonnelInfo.hpp"
#include <iostream>

namespace corporate_planning::core {

PersonnelInfo::PersonnelInfo()
    : productionStaff(0),
      salesAndDistributionStaff(0),
      administrativeStaff(0),
      otherStaff(0),
      totalEmployment(0) {
}

// Getters
int PersonnelInfo::getProductionStaff() const {
    return productionStaff;
}

int PersonnelInfo::getSalesAndDistributionStaff() const {
    return salesAndDistributionStaff;
}

int PersonnelInfo::getAdministrativeStaff() const {
    return administrativeStaff;
}

int PersonnelInfo::getOtherStaff() const {
    return otherStaff;
}

int PersonnelInfo::getTotalEmployment() const {
    return totalEmployment;
}

// Setters
void PersonnelInfo::setProductionStaff(int value) {
    productionStaff = value;
}

void PersonnelInfo::setSalesAndDistributionStaff(int value) {
    salesAndDistributionStaff = value;
}

void PersonnelInfo::setAdministrativeStaff(int value) {
    administrativeStaff = value;
}

void PersonnelInfo::setOtherStaff(int value) {
    otherStaff = value;
}

void PersonnelInfo::setTotalEmployment(int value) {
    totalEmployment = value;
}

// Calculations
int PersonnelInfo::calculateTotalEmployment() const {
    if (totalEmployment > 0) {
        return totalEmployment;
    }
    return productionStaff + salesAndDistributionStaff + administrativeStaff + otherStaff;
}

} // namespace corporate_planning::core
