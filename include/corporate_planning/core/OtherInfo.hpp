#ifndef CORPORATE_PLANNING_CORE_OTHERINFO_HPP
#define CORPORATE_PLANNING_CORE_OTHERINFO_HPP

namespace corporate_planning::core {

// ساختار متغیرهای کمکی (شامل ۴ زیربخش)
struct AuxiliaryVariables {
    double crisisPeriodFactor;              // مربوط به دوران بحران
    double administrativeExpensesFactor;    // هزینه‌های اداری در بنگاه
    double fxShortagePeriodFactor;          // دوران کاهش منابع ارزی
    double otherAuxiliaryFactor;            // متغیرهای کمکی دیگر

    AuxiliaryVariables();
};

class OtherInfo {
public:
    OtherInfo();
    
    // Getters
    double getRoiRate() const;
    double getForeignExchangeUsed() const;
    double getUnitForeignExchangeForProduction() const;
    double getProductionValue() const;
    const AuxiliaryVariables& getAuxiliaryVariables() const;
    AuxiliaryVariables& getAuxiliaryVariables();
    double getTimeVariable() const;

    // Setters
    void setRoiRate(double value);
    void setForeignExchangeUsed(double value);
    void setUnitForeignExchangeForProduction(double value);
    void setProductionValue(double value);
    void setAuxiliaryVariables(const AuxiliaryVariables& value);
    void setTimeVariable(double value);

    // Business Logic / Calculations
    double calculateForeignExchangeUsedFromRate(double productionAmount) const;
    double calculateUnitForeignExchangeFromTotal(double productionAmount) const;
    double calculateProductionValue(double productionAmount, double unitPrice) const;

private:
    double roiRate;                               // نرخ بازگشت روی سرمایه‌گذاری
    double foreignExchangeUsed;                   // میزان ارز مورد استفاده
    double unitForeignExchangeForProduction;      // ارز واحد برای تولید محصول
    double productionValue;                       // ارزش تولیدات
    AuxiliaryVariables auxiliaryVariables;        // متغیرهای کمکی (۴ زیربخش)
    double timeVariable;                          // متغیرهای مربوط به زمان

    static double safeDivide(double numerator, double denominator);
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_OTHERINFO_HPP
