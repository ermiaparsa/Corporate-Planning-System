#include "corporate_planning/core/ManufacturingCompany.hpp"
#include <iostream>

using namespace std;

namespace corporate_planning::core {

ManufacturingCompany::ManufacturingCompany()
    : companyName(""),
      registrationNumber(""),
      industryType(""),
      fiscalYear(0),
      balanceSheet(),
      incomeStatement(),
      personnelInfo(),
      otherInfo(),
      productionLines() {
}

ManufacturingCompany::ManufacturingCompany(const string& name, const string& regNo,
                                           const string& industry, int year)
    : companyName(name),
      registrationNumber(regNo),
      industryType(industry),
      fiscalYear(year),
      balanceSheet(),
      incomeStatement(),
      personnelInfo(),
      otherInfo(),
      productionLines() {
}

// Getters - Company Info
string ManufacturingCompany::getCompanyName() const {
    return companyName;
}

string ManufacturingCompany::getRegistrationNumber() const {
    return registrationNumber;
}

string ManufacturingCompany::getIndustryType() const {
    return industryType;
}

int ManufacturingCompany::getFiscalYear() const {
    return fiscalYear;
}

// Setters - Company Info
void ManufacturingCompany::setCompanyName(const string& name) {
    companyName = name;
}

void ManufacturingCompany::setRegistrationNumber(const string& regNo) {
    registrationNumber = regNo;
}

void ManufacturingCompany::setIndustryType(const string& industry) {
    industryType = industry;
}

void ManufacturingCompany::setFiscalYear(int year) {
    fiscalYear = year;
}

// Core Modules Accessors
BalanceSheet& ManufacturingCompany::getBalanceSheet() {
    return balanceSheet;
}

const BalanceSheet& ManufacturingCompany::getBalanceSheet() const {
    return balanceSheet;
}

void ManufacturingCompany::setBalanceSheet(const BalanceSheet& bs) {
    balanceSheet = bs;
}

IncomeStatement& ManufacturingCompany::getIncomeStatement() {
    return incomeStatement;
}

const IncomeStatement& ManufacturingCompany::getIncomeStatement() const {
    return incomeStatement;
}

void ManufacturingCompany::setIncomeStatement(const IncomeStatement& is) {
    incomeStatement = is;
}

PersonnelInfo& ManufacturingCompany::getPersonnelInfo() {
    return personnelInfo;
}

const PersonnelInfo& ManufacturingCompany::getPersonnelInfo() const {
    return personnelInfo;
}

void ManufacturingCompany::setPersonnelInfo(const PersonnelInfo& pi) {
    personnelInfo = pi;
}

OtherInfo& ManufacturingCompany::getOtherInfo() {
    return otherInfo;
}

const OtherInfo& ManufacturingCompany::getOtherInfo() const {
    return otherInfo;
}

void ManufacturingCompany::setOtherInfo(const OtherInfo& oi) {
    otherInfo = oi;
}

// Dynamic Production Lines Management
const vector<ProductionLineInfo>& ManufacturingCompany::getProductionLines() const {
    return productionLines;
}

vector<ProductionLineInfo>& ManufacturingCompany::getProductionLines() {
    return productionLines;
}

void ManufacturingCompany::addProductionLine(const ProductionLineInfo& line) {
    productionLines.push_back(line);
}

void ManufacturingCompany::clearProductionLines() {
    productionLines.clear();
}

size_t ManufacturingCompany::getProductionLineCount() const {
    return productionLines.size();
}

// Aggregate Business Logic
double ManufacturingCompany::calculateTotalCompanyRevenue() const {
    double totalRevenue = 0.0;
    for (size_t i = 0; i < productionLines.size(); ++i) {
        totalRevenue += productionLines[i].calculateRevenue();
    }
    // اگر در صورت سود و زیان هم فروش ثبت شده بود و خطوط خالی بودن:
    if (totalRevenue == 0.0) {
        totalRevenue = incomeStatement.getSales();
    }
    return totalRevenue;
}

double ManufacturingCompany::calculateTotalProductionCosts() const {
    double totalCosts = 0.0;
    for (size_t i = 0; i < productionLines.size(); ++i) {
        totalCosts += productionLines[i].calculateTotalCosts();
    }
    return totalCosts;
}

double ManufacturingCompany::calculateTotalCompanyProfit() const {
    return calculateTotalCompanyRevenue() - calculateTotalProductionCosts();
}

int ManufacturingCompany::calculateTotalCompanyEmployment() const {
    int total = personnelInfo.calculateTotalEmployment();
    for (size_t i = 0; i < productionLines.size(); ++i) {
        total += productionLines[i].getWorkerCount();
    }
    return total;
}

} // namespace corporate_planning::core
