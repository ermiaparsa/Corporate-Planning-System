#ifndef CORPORATE_PLANNING_CORE_MANUFACTURINGCOMPANY_HPP
#define CORPORATE_PLANNING_CORE_MANUFACTURINGCOMPANY_HPP

#include <string>
#include <vector>
#include "corporate_planning/core/BalanceSheet.hpp"
#include "corporate_planning/core/IncomeStatement.hpp"
#include "corporate_planning/core/ProductionLineInfo.hpp"
#include "corporate_planning/core/PersonnelInfo.hpp"
#include "corporate_planning/core/OtherInfo.hpp"

namespace corporate_planning::core {

class ManufacturingCompany {
public:
    ManufacturingCompany();
    ManufacturingCompany(const std::string& name, const std::string& registrationNo,
                         const std::string& industry, int fiscalYear);

    // Getters - Company Info
    std::string getCompanyName() const;
    std::string getRegistrationNumber() const;
    std::string getIndustryType() const;
    int getFiscalYear() const;

    // Setters - Company Info
    void setCompanyName(const std::string& name);
    void setRegistrationNumber(const std::string& regNo);
    void setIndustryType(const std::string& industry);
    void setFiscalYear(int year);

    // Getters & Setters - Core Modules (Non-const & Const overloads)
    BalanceSheet& getBalanceSheet();
    const BalanceSheet& getBalanceSheet() const;
    void setBalanceSheet(const BalanceSheet& bs);

    IncomeStatement& getIncomeStatement();
    const IncomeStatement& getIncomeStatement() const;
    void setIncomeStatement(const IncomeStatement& is);

    PersonnelInfo& getPersonnelInfo();
    const PersonnelInfo& getPersonnelInfo() const;
    void setPersonnelInfo(const PersonnelInfo& pi);

    OtherInfo& getOtherInfo();
    const OtherInfo& getOtherInfo() const;
    void setOtherInfo(const OtherInfo& oi);

    // Dynamic Production Lines Management (Vector-based)
    const std::vector<ProductionLineInfo>& getProductionLines() const;
    std::vector<ProductionLineInfo>& getProductionLines();
    void addProductionLine(const ProductionLineInfo& line);
    void clearProductionLines();
    size_t getProductionLineCount() const;

    // Aggregate Business Logic
    double calculateTotalCompanyRevenue() const;
    double calculateTotalProductionCosts() const;
    double calculateTotalCompanyProfit() const;
    int calculateTotalCompanyEmployment() const;

private:
    std::string companyName;
    std::string registrationNumber;
    std::string industryType;
    int fiscalYear;

    BalanceSheet balanceSheet;
    IncomeStatement incomeStatement;
    PersonnelInfo personnelInfo;
    OtherInfo otherInfo;
    std::vector<ProductionLineInfo> productionLines;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_MANUFACTURINGCOMPANY_HPP
