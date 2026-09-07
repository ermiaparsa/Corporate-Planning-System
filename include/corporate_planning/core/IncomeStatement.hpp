#ifndef CORPORATE_PLANNING_CORE_INCOMESTATEMENT_HPP
#define CORPORATE_PLANNING_CORE_INCOMESTATEMENT_HPP

namespace corporate_planning::core {

class IncomeStatement {
public:
    IncomeStatement();
    // --- توابع محاسباتی (Calculations) ---
    double calculateOverheadCosts() const;
    double calculateCostOfGoodsSold() const;
    double calculateOtherIncomes() const;
    double calculateOtherExpenses() const;
    double calculateGrossProfit() const;
    double calculatePreTaxProfit() const;

    // --- ۱. فروش (Sales) ---
    void setSales(double value);
    double getSales() const;

    // --- ۳. هزینه‌های توزیع و فروش ---
    void setDistributionAndSalesCosts(double value);
    double getDistributionAndSalesCosts() const;

    // --- ۴. هزینه‌های اداری ---
    void setAdministrativeCosts(double value);
    double getAdministrativeCosts() const;

    // --- ۶. قیمت تمام شده کالای فروش رفته (اجزا) ---
    void setRawMaterials(double value);
    double getRawMaterials() const;

    void setDirectLaborCosts(double value);
    double getDirectLaborCosts() const;

    void setInventoryAdjustments(double value);
    double getInventoryAdjustments() const;

    // ۶-۴: دستمزد غیرمستقیم و سربار
    void setDepreciation(double value);
    double getDepreciation() const;

    void setUtilityCosts(double value);
    double getUtilityCosts() const;

    void setMaintenanceAndRepairs(double value);
    double getMaintenanceAndRepairs() const;

    void setConsumableSupplies(double value);
    double getConsumableSupplies() const;

    void setTransportationCosts(double value);
    double getTransportationCosts() const;

    // --- ۷. سایر درآمدها ---
    void setUnusedItemsSales(double value);
    double getUnusedItemsSales() const;

    void setAssetDisposalGains(double value);
    double getAssetDisposalGains() const;

    void setLoadingRevenues(double value);
    double getLoadingRevenues() const;

    void setMiscellaneousIncomes(double value);
    double getMiscellaneousIncomes() const;

    // --- ۸. سایر هزینه‌ها ---
    void setLongTermLoanInterest(double value);
    double getLongTermLoanInterest() const;

    void setInvestmentImpairmentLoss(double value);
    double getInvestmentImpairmentLoss() const;

    void setMiscellaneousExpenses(double value);
    double getMiscellaneousExpenses() const;

private:
    // ۱. فروش
    double sales;

    // ۳. هزینه‌های توزیع و فروش
    double distributionAndSalesCosts;

    // ۴. هزینه‌های اداری
    double administrativeCosts;

    // ۶. اجزای قیمت تمام شده کالای فروش رفته
    double rawMaterials;
    double directLaborCosts;
    double inventoryAdjustments;

    // ۶-۴. اجزای دستمزد غیرمستقیم و سربار
    double depreciation;              // استهلاک
    double utilityCosts;              // آب و برق و تلفن
    double maintenanceAndRepairs;     // تعمیرات و نگهداری
    double consumableSupplies;        // ملزومات مصرفی
    double transportationCosts;       // ایاب و ذهاب

    // ۷. اجزای سایر درآمدها
    double unusedItemsSales;          // فروش اجناس بلااستفاده
    double assetDisposalGains;        // سود فروش اموال و تجهیزات
    double loadingRevenues;           // درآمدهای بارگیری
    double miscellaneousIncomes;      // درآمدهای متفرقه

    // ۸. اجزای سایر هزینه‌ها
    double longTermLoanInterest;      // بهره وام بلندمدت
    double investmentImpairmentLoss;  // هزینه کاهش ارزش سرمایه‌گذاری خارج شرکت
    double miscellaneousExpenses;     // سایر هزینه‌های متفرقه
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_IncomeStatement_HPP
