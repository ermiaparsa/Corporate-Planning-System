#ifndef CORPORATE_PLANNING_CORE_PERSONNELINFO_HPP
#define CORPORATE_PLANNING_CORE_PERSONNELINFO_HPP

namespace corporate_planning::core {

class PersonnelInfo {
public:
    PersonnelInfo();
    
     // Getters
    int getProductionStaff() const;
    int getSalesAndDistributionStaff() const;
    int getAdministrativeStaff() const;
    int getOtherStaff() const;
    int getTotalEmployment() const;

    // Setters
    void setProductionStaff(int value);
    void setSalesAndDistributionStaff(int value);
    void setAdministrativeStaff(int value);
    void setOtherStaff(int value);
    void setTotalEmployment(int value);

    // Business Logic / Calculations
    int calculateTotalEmployment() const;

private:
    int productionStaff;             // کادر تولید
    int salesAndDistributionStaff;   // کادر توزیع و فروش
    int administrativeStaff;         // کادر اداری
    int otherStaff;                  // سایر پرسنل
    int totalEmployment;             // کل اشتغال
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_PERSONNELINFO_HPP
