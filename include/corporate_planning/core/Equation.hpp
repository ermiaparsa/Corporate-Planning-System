#ifndef CORPORATE_PLANNING_CORE_EQUATION_HPP
#define CORPORATE_PLANNING_CORE_EQUATION_HPP

#include <string>
#include <vector>

namespace corporate_planning::core {

enum class EquationType {
    LINEAR,           // مدل‌های خطی مثل برآورد هزینه کل
    POLYNOMIAL,       // توابع غیرخطی درجه چندم
    PRODUCTION_FUNC,  // توابع تولید (مانند Cobb-Douglas / تخمین تولید)
    CUSTOM            // معادلات سفارشی تحلیل‌گر
};

class Equation {
public:
    Equation();
    Equation(const std::string& eqId, const std::string& eqName,
             EquationType eqType, double interceptValue = 0.0);

    // Getters
    std::string getEquationId() const;
    std::string getEquationName() const;
    EquationType getType() const;
    std::string getTypeString() const;
    double getIntercept() const;
    const std::vector<double>& getCoefficients() const;
    const std::vector<std::string>& getVariableNames() const;

    // Setters
    void setEquationId(const std::string& eqId);
    void setEquationName(const std::string& eqName);
    void setType(EquationType eqType);
    void setIntercept(double interceptValue);
    void setCoefficients(const std::vector<double>& coeffs);
    void setVariableNames(const std::vector<std::string>& varNames);

    // Coefficients & Variables Management
    void addTerm(const std::string& varName, double coefficient);
    void clearTerms();
    size_t getTermCount() const;

    // Mathematical Evaluation & AI Logic
    double evaluate(const std::vector<double>& inputs) const;
    double calculateResidual(const std::vector<double>& inputs, double actualValue) const;
    double calculateMSE(const std::vector<std::vector<double>>& datasetInputs,
                        const std::vector<double>& actualValues) const;

    // Representation
    std::string toFormulaString() const;

private:
    std::string equationId;
    std::string equationName;
    EquationType type;
    double intercept;
    std::vector<double> coefficients;
    std::vector<std::string> variableNames;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_EQUATION_HPP
