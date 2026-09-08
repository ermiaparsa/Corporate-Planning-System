#include "corporate_planning/core/Equation.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

namespace corporate_planning::core {

Equation::Equation()
    : equationId(""),
      equationName(""),
      type(EquationType::LINEAR),
      intercept(0.0),
      coefficients(),
      variableNames() {
}
Equation::Equation(const string& eqId, const string& eqName,
                   EquationType eqType, double interceptValue)
    : equationId(eqId),
      equationName(eqName),
      type(eqType),
      intercept(interceptValue),
      coefficients(),
      variableNames() {
}
// Getters
string Equation::getEquationId() const {
    return equationId;
}

string Equation::getEquationName() const {
    return equationName;
}

EquationType Equation::getType() const {
    return type;
}

string Equation::getTypeString() const {
    switch (type) {
        case EquationType::LINEAR:
            return "LINEAR";
        case EquationType::POLYNOMIAL:
            return "POLYNOMIAL";
        case EquationType::PRODUCTION_FUNC:
            return "PRODUCTION_FUNC";
        case EquationType::CUSTOM:
            return "CUSTOM";
        default:
            return "UNKNOWN";
    }
}

double Equation::getIntercept() const {
    return intercept;
}

const vector<double>& Equation::getCoefficients() const {
    return coefficients;
}

const vector<string>& Equation::getVariableNames() const {
    return variableNames;
}

// Setters
void Equation::setEquationId(const string& eqId) {
    equationId = eqId;
}

void Equation::setEquationName(const string& eqName) {
    equationName = eqName;
}

void Equation::setType(EquationType eqType) {
    type = eqType;
}

void Equation::setIntercept(double interceptValue) {
    intercept = interceptValue;
}

void Equation::setCoefficients(const vector<double>& coeffs) {
    coefficients = coeffs;
}

void Equation::setVariableNames(const vector<string>& varNames) {
    variableNames = varNames;
}

// Terms Management
void Equation::addTerm(const string& varName, double coefficient) {
    variableNames.push_back(varName);
    coefficients.push_back(coefficient);
}

void Equation::clearTerms() {
    variableNames.clear();
    coefficients.clear();
}

size_t Equation::getTermCount() const {
    return coefficients.size();
}

// Mathematical Evaluation & AI Logic
double Equation::evaluate(const vector<double>& inputs) const {
    double result = intercept;
    size_t count = min(inputs.size(), coefficients.size());

    for (size_t i = 0; i < count; ++i) {
        result += coefficients[i] * inputs[i];
    }
    return result;
}

double Equation::calculateResidual(const vector<double>& inputs, double actualValue) const {
    return evaluate(inputs) - actualValue;
}

double Equation::calculateMSE(const vector<vector<double>>& datasetInputs,
                              const vector<double>& actualValues) const {
    if (datasetInputs.empty() || datasetInputs.size() != actualValues.size()) {
        return 0.0;
    }

    double sumSquaredErrors = 0.0;
    size_t n = datasetInputs.size();

    for (size_t i = 0; i < n; ++i) {
        double diff = evaluate(datasetInputs[i]) - actualValues[i];
        sumSquaredErrors += diff * diff;
    }

    return sumSquaredErrors / static_cast<double>(n);
}

// Representation
string Equation::toFormulaString() const {
    ostringstream oss;
    oss << "Y = ";

    bool hasTerms = false;
    for (size_t i = 0; i < coefficients.size(); ++i) {
        double coeff = coefficients[i];
        string var = (i < variableNames.size()) ? variableNames[i] : ("X" + to_string(i + 1));

        if (i == 0) {
            oss << coeff << " * " << var;
        } else {
            if (coeff >= 0) {
                oss << " + " << coeff << " * " << var;
            } else {
                oss << " - " << abs(coeff) << " * " << var;
            }
        }
        hasTerms = true;
    }

    if (intercept != 0.0 || !hasTerms) {
        if (hasTerms) {
            if (intercept > 0) {
                oss << " + " << intercept;
            } else {
                oss << " - " << abs(intercept);
            }
        } else {
            oss << intercept;
        }
    }

    return oss.str();
}

} // namespace corporate_planning::core
