#include "corporate_planning/core/Liabilities.hpp"

using namespace std;

namespace corporate_planning::core {

Liabilities::Liabilities()
    : bankOverdraft(0.0),
      accountsPayable(0.0),
      shortTermLoans(0.0),
      longTermLoans(0.0) {
}

double Liabilities::calculateTotal() const {
    return bankOverdraft
         + accountsPayable
         + shortTermLoans
         + longTermLoans;
}

void Liabilities::setBankOverdraft(double value) {
    bankOverdraft = value;
}

void Liabilities::setAccountsPayable(double value) {
    accountsPayable = value;
}

void Liabilities::setShortTermLoans(double value) {
    shortTermLoans = value;
}

void Liabilities::setLongTermLoans(double value) {
    longTermLoans = value;
}

double Liabilities::getBankOverdraft() const {
    return bankOverdraft;
}

double Liabilities::getAccountsPayable() const {
    return accountsPayable;
}

double Liabilities::getShortTermLoans() const {
    return shortTermLoans;
}

double Liabilities::getLongTermLoans() const {
    return longTermLoans;
}


} // namespace corporate_planning::core
