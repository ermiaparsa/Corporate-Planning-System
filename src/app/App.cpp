#include <iostream>

#include "corporate_planning/app/App.hpp"

using namespace std;

namespace corporate_planning::app {

App::App()
    : company("Default Industrial Co.", "REG-0000", "General", 2026),
      currentUser(),
      isRunning(true) {
    // راه‌اندازی یک کاربر پیش‌فرض (پلنر)
    currentUser = corporate_planning::core::User(
        "USR-001",
        "System Admin",
        "admin",
        "admin_pass_hash",
        "admin@company.local",
        corporate_planning::core::UserRole::ADMIN
    );
    currentUser.setLastLogin("2026-09-08 09:00:00");
}

void App::run() {
    displayMainMenu();
}

// --- Main Menu ---
void App::displayMainMenu() {
    while (isRunning) {
        cout << "\n========================================\n";
        cout << "   CORPORATE PLANNING SYSTEM\n";
        cout << "   Welcome, " << currentUser.getFullName() << " (" 
             << currentUser.getRoleString() << ")\n";
        cout << "========================================\n";

        cout << "1) Company Overview\n";
        cout << "2) Production Lines\n";
        cout << "3) Personnel Management\n";
        cout << "4) Financial Ratios\n";
        cout << "5) Equations / AI Models\n";
        cout << "6) User Profile & Authentication\n";
        cout << "0) Exit\n";
        cout << "Select: ";

        int choice = readIntFromUser();
        switch (choice) {
            case 1:
                displayCompanyOverviewMenu();
                break;
            case 2:
                displayProductionLinesMenu();
                break;
            case 3:
                displayPersonnelMenu();
                break;
            case 4:
                displayFinancialMenu();
                break;
            case 5:
                displayEquationsMenu();
                break;
            case 6:
                displayUserMenu();
                break;
            case 0:
                isRunning = false;
                cout << "\nExiting the system. Goodbye!\n";
                break;
            default:
                cout << "\n[!] Invalid choice. Try again.\n";
                break;
        }
    }
}

void App::displayCompanyOverviewMenu() {
    cout << "\n--- Company Overview ---\n";
    cout << "Name: " << company.getCompanyName() << '\n';
    cout << "Registration No.: " << company.getRegistrationNumber() << '\n';
    cout << "Industry: " << company.getIndustryType() << '\n';
    cout << "Fiscal Year: " << company.getFiscalYear() << '\n';
    cout << "Number of Production Lines: " 
         << company.getProductionLineCount() << '\n';
    cout << "Total Employment: " 
         << company.calculateTotalCompanyEmployment() << '\n';
    cout << "Total Revenue: " 
         << company.calculateTotalCompanyRevenue() << '\n';
    cout << "Total Production Costs: " 
         << company.calculateTotalProductionCosts() << '\n';
    cout << "Total Profit: " 
         << company.calculateTotalCompanyProfit() << '\n';
}

void App::displayProductionLinesMenu() {
    cout << "\n--- Production Lines ---\n";
    if (company.getProductionLineCount() == 0) {
        cout << "No production lines registered yet.\n";
        if (confirm("Add a sample production line?")) {
            addSampleProductionLine();
        }
        return;
    }

    const vector<corporate_planning::core::ProductionLineInfo>& lines =
        company.getProductionLines();
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << "  [" << (i + 1) << "] Worker count: " 
             << lines[i].getWorkerCount()
             << " | Revenue: " << lines[i].calculateRevenue()
             << '\n';
    }
}

void App::displayPersonnelMenu() {
    cout << "\n--- Personnel ---\n";
    cout << "Administrative Staff: " 
         << company.getPersonnelInfo().getAdministrativeStaff() << '\n';
    cout << "Sales & Distribution: " 
         << company.getPersonnelInfo().getSalesAndDistributionStaff() << '\n';
    cout << "Other Staff: " 
         << company.getPersonnelInfo().getOtherStaff() << '\n';
    cout << "Total Employment: " 
         << company.getPersonnelInfo().calculateTotalEmployment() << '\n';
}

void App::displayFinancialMenu() {
    cout << "\n--- Financial Ratios ---\n";

    corporate_planning::core::Assets assets =
        company.getBalanceSheet().getAssets();
    corporate_planning::core::Liabilities liabilities =
        company.getBalanceSheet().getLiabilities();

    double totalAssets = assets.calculateTotal();
    double totalLiabilities = liabilities.calculateTotal();

    cout << "Total Assets:       " << totalAssets << '\n';
    cout << "Total Liabilities:  " << totalLiabilities << '\n';
    cout << "Equity (Owner's):   " << (totalAssets - totalLiabilities) << '\n';
}

void App::displayEquationsMenu() {
    cout << "\n--- Equations & AI Models ---\n";
    cout << "Model list placeholder. (Equation registry to be developed)\n";

    // مثال تستی محاسبه خطی
    corporate_planning::core::Equation demoModel(
        "EQ-DEMO", "Demo Linear Model",
        corporate_planning::core::EquationType::LINEAR, 10.0);
    demoModel.addTerm("X1", 2.0);
    demoModel.addTerm("X2", 3.0);
    vector<double> demoInput = {5.0, 7.0};
    cout << "Demo formula: " << demoModel.toFormulaString() << '\n';
    cout << "Demo evaluate([5,7]): " << demoModel.evaluate(demoInput) << '\n';
}

void App::displayUserMenu() {
    cout << "\n--- User Profile ---\n";
    cout << "ID: " << currentUser.getUserId() << '\n';
    cout << "Fullname: " << currentUser.getFullName() << '\n';
    cout << "Username: " << currentUser.getUsername() << '\n';
    cout << "Email: " << currentUser.getEmail() << '\n';
    cout << "Role: " << currentUser.getRoleString() << '\n';
    cout << "Is Active: " << (currentUser.getIsActive() ? "Yes" : "No") << '\n';
    cout << "Last Login: " << currentUser.getLastLogin() << '\n';
}

// --- Helper: Sample Data ---
void App::addSampleProductionLine() {
    corporate_planning::core::ProductionLineInfo sample;
    sample.setWorkerCount(20);
    sample.setProductionAmount(1500.0);
    sample.setSalesAmount(1400.0);
    sample.setSalePrice(110.0);
    sample.setFixedCosts(30000.0);
    sample.setVariableCosts(90000.0);
    company.addProductionLine(sample);
    cout << "Sample production line added successfully!\n";
}

// --- Input Helpers ---
int App::readIntFromUser() {
    int value = 0;
    cin >> value;
    return value;
}

double App::readDoubleFromUser() {
    double value = 0.0;
    cin >> value;
    return value;
}

string App::readLineFromUser(const string& prompt) {
    cout << prompt;
    string value;
    cin.ignore();
    getline(cin, value);
    return value;
}

bool App::confirm(const string& message) {
    cout << message << " (y/n): ";
    char answer = 'n';
    cin >> answer;
    return (answer == 'y' || answer == 'Y');
}

} // namespace corporate_planning::app
