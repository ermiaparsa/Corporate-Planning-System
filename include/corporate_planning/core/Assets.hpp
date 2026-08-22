#ifndef CORPORATE_PLANNING_CORE_ASSETS_HPP
#define CORPORATE_PLANNING_CORE_ASSETS_HPP

namespace corporate_planning::core {

class Assets {
public:
    Assets();
    double calculateInventoryTotal() const;
    double calculateTangibleAssetsTotal() const;
    double calculateTotal() const;

    void setCashOnHand(double value);
    void setCashInBank(double value);
    void setAccountsReceivable(double value);

    void setLand(double value);
    void setBuildings(double value);
    void setMachinery(double value);
    void setFurniture(double value);
    void setVehicles(double value);
    void setInstallations(double value);

    void setRawMaterials(double value);
    void setWorkInProcess(double value);
    void setFinishedGoods(double value);
    void setSpareParts(double value);
    void setSaleableWaste(double value);

    void setLongTermInvestments(double value);
    void setExternalInvestments(double value);

    double getCashOnHand() const;
    double getCashInBank() const;
    double getAccountsReceivable() const;

private:
    // دارایی‌های جاری
    double cashOnHand;
    double cashInBank;
    double accountsReceivable;

    // دارایی‌های ثابت
    double land;
    double buildings;
    double machinery;
    double furniture;
    double vehicles;
    double installations;

    // اجزای موجودی انبار
    double rawMaterials;
    double workInProcess;
    double finishedGoods;
    double spareParts;
    double saleableWaste;

    // سایر دارایی‌ها
    double longTermInvestments;
    double externalInvestments;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_ASSETS_HPP
