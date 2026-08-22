#ifndef CORPORATE_PLANNING_CORE_LIABILITIES_HPP
#define CORPORATE_PLANNING_CORE_LIABILITIES_HPP

namespace corporate_planning::core {

class Liabilities {
public:
    Liabilities();

    double calculateTotal() const;

    void setBankOverdraft(double value);
    void setAccountsPayable(double value);
    void setShortTermLoans(double value);
    void setLongTermLoans(double value);

    double getBankOverdraft() const;
    double getAccountsPayable() const;
    double getShortTermLoans() const;
    double getLongTermLoans() const;

private:
    // بدهی‌های جاری
    double bankOverdraft;
    double accountsPayable;
    double shortTermLoans;

    // بدهی‌های بلندمدت
    double longTermLoans;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_LIABILITIES_HPP
