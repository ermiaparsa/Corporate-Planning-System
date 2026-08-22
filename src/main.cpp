#include <iostream>
#include "corporate_planning/core/BalanceSheet.hpp"

using namespace std;

namespace corporate_planning::api {
    void runCli();
}

int main() {
    corporate_planning::api::runCli();

    cout <<"-------------"<<endl<<"Corporate Planning - Firm Planning Model\n";
    cout << "Version 1.0.0\n";
    cout << "Corporate Planning started successfully." << endl<<"-------------"<< endl;

    corporate_planning::core::BalanceSheet balanceSheet;

    balanceSheet.getAssets().setCashOnHand(10000.0);
    balanceSheet.getAssets().setCashInBank(25000.0);
    balanceSheet.getAssets().setLand(50000.0);

    balanceSheet.getLiabilities().setAccountsPayable(8000.0);
    balanceSheet.getLiabilities().setLongTermLoans(20000.0);

    cout << "Total assets: "
    << balanceSheet.calculateTotalAssets()
    << endl;

    cout << "Total liabilities: "
    << balanceSheet.calculateTotalLiabilities()
    << endl;    

return 0;
}
