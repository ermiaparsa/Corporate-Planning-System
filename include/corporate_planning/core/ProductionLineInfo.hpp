#ifndef CORPORATE_PLANNING_CORE_ProductionLineInfo_HPP
#define CORPORATE_PLANNING_CORE_ProductionLineInfo_HPP

namespace corporate_planning::core {

struct OtherVariables {
    double workerWageAmount;
    double rawMaterialPurchasePrice;
    double capitalDepreciationAmount;

    OtherVariables();
};

class ProductionLineInfo {
public:
    ProductionLineInfo();
    
    // Getters
    double getRawMaterialsCost() const;
    double getLaborCost() const;
    double getVariableCosts() const;
    double getFixedCosts() const;
    double getTotalCosts() const;
    double getMachinery() const;
    int getWorkerCount() const;
    double getDepreciationAmount() const;
    double getSalesAmount() const;
    double getProductionAmount() const;
    double getSalePrice() const;
    const OtherVariables& getOtherVariables() const;

    // Setters
    void setRawMaterialsCost(double value);
    void setLaborCost(double value);
    void setVariableCosts(double value);
    void setFixedCosts(double value);
    void setTotalCosts(double value);
    void setMachinery(double value);
    void setWorkerCount(int value);
    void setDepreciationAmount(double value);
    void setSalesAmount(double value);
    void setProductionAmount(double value);
    void setSalePrice(double value);
    void setOtherVariables(const OtherVariables& value);

    // Business Logic / Calculation Methods
    double calculateTotalCosts() const;
    double calculateRevenue() const;
    double calculateUnitCost() const;
    double calculateUnitProfit() const;
    double calculateCapacityUtilization() const;
    double calculateLaborCostFromRate() const;
    double calculateRawMaterialsCostFromRate() const;

private:
    double rawMaterialsCost;
    double laborCost;
    double variableCosts;
    double fixedCosts;
    double totalCosts;
    double machinery;
    int workerCount;
    double depreciationAmount;
    double salesAmount;
    double productionAmount;
    double salePrice;
    OtherVariables otherVariables;

    static double safeDivide(double numerator, double denominator);
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_ProductionLineInfo_HPP
