#ifndef CORPORATE_PLANNING_CORE_BALANCESHEET_HPP
#define CORPORATE_PLANNING_CORE_BALANCESHEET_HPP

#include "corporate_planning/core/Assets.hpp"
#include "corporate_planning/core/Liabilities.hpp"

namespace corporate_planning::core {

class BalanceSheet {
    public:
        BalanceSheet();

        double calculateTotalAssets() const;
        double calculateTotalLiabilities() const;
        double calculateEquity() const;

        Assets& getAssets();
        Liabilities& getLiabilities();

        void setNetWorth(double value);
        double getNetWorth() const;

    private:
        Assets assets;
        Liabilities liabilities;
        double netWorth;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_BALANCESHEET_HPP
