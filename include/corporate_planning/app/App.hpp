#ifndef CORPORATE_PLANNING_APP_APP_HPP
#define CORPORATE_PLANNING_APP_APP_HPP

#include <string>

#include "corporate_planning/core/ManufacturingCompany.hpp"
#include "corporate_planning/core/User.hpp"
#include "corporate_planning/core/Equation.hpp"

namespace corporate_planning::app {

class App {
public:
    App();

    // Main loop
    void run();

private:
    // Core aggregate owner
    corporate_planning::core::ManufacturingCompany company;
    corporate_planning::core::User currentUser;
    bool isRunning;

    // Helper methods
    void displayMainMenu();
    void displayUserMenu();
    void handleUserRoleSelection();
    void displayDashboard();
    void displayCompanyOverviewMenu();
    void displayProductionLinesMenu();
    void displayPersonnelMenu();
    void displayFinancialMenu();
    void displayEquationsMenu();

    // Input helpers
    int readIntFromUser();
    double readDoubleFromUser();
    std::string readLineFromUser(const std::string& prompt);
    bool confirm(const std::string& message);

    // Domain helpers (used by Console UI only)
    void addSampleProductionLine();
};

} // namespace corporate_planning::app

#endif // CORPORATE_PLANNING_APP_APP_HPP
