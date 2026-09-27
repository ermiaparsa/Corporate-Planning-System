#include "corporate_planning/core/IncomeStatementRecord.hpp"
#include <iostream>

namespace corporate_planning::core {

void IncomeStatementRecord::compute() {
    grossProfit = grossRevenue - costOfGoodsSold;

    const double totalOpEx = sellingGeneralAndAdministrative
                           + researchAndDevelopment
                           + depreciationAndAmortization;
    operatingIncome = grossProfit - totalOpEx;

    earningsBeforeTax = operatingIncome - interestExpense;

    taxExpense = earningsBeforeTax * (taxRate / 100.0);
    netIncome = earningsBeforeTax - taxExpense;

    netProfitMargin = grossRevenue > 0.0
                          ? (netIncome / grossRevenue) * 100.0
                          : 0.0;
}

QString IncomeStatementRecord::toJson() const {
    QString result;
    result += "{";
    result += "\"fiscalYear\":" + QString::number(fiscalYear) + ",";
    result += "\"grossRevenue\":" + QString::number(grossRevenue, 'f', 6) + ",";
    result += "\"costOfGoodsSold\":" + QString::number(costOfGoodsSold, 'f', 6) + ",";
    result += "\"sellingGeneralAndAdministrative\":" + QString::number(sellingGeneralAndAdministrative, 'f', 6) + ",";
    result += "\"researchAndDevelopment\":" + QString::number(researchAndDevelopment, 'f', 6) + ",";
    result += "\"depreciationAndAmortization\":" + QString::number(depreciationAndAmortization, 'f', 6) + ",";
    result += "\"interestExpense\":" + QString::number(interestExpense, 'f', 6) + ",";
    result += "\"taxRate\":" + QString::number(taxRate, 'f', 6);
    result += "}";
    return result;
}

IncomeStatementRecord IncomeStatementRecord::fromJson(const QString& json) {
    IncomeStatementRecord record;

    auto extractValue = [&](const QString& key) -> QString {
        const QString search = "\"" + key + "\"";
        const int keyIdx = json.indexOf(search);
        if (keyIdx < 0) return "";

        const int colonIdx = json.indexOf(':', keyIdx);
        if (colonIdx < 0) return "";

        int valStart = colonIdx + 1;
        while (valStart < json.size() && json[valStart].isSpace()) ++valStart;

        // Check if value is a number or string
        if (valStart >= json.size()) return "";

        int valEnd;
        if (json[valStart] == '"') {
            // String value
            valEnd = json.indexOf('"', valStart + 1);
            if (valEnd < 0) return "";
            return json.mid(valStart + 1, valEnd - valStart - 1);
        } else {
            // Numeric value
            valEnd = valStart;
            while (valEnd < json.size() && json[valEnd] != ',' && json[valEnd] != '}' && !json[valEnd].isSpace()) {
                ++valEnd;
            }
            return json.mid(valStart, valEnd - valStart);
        }
    };

    record.fiscalYear = extractValue("fiscalYear").toInt();
    record.grossRevenue = extractValue("grossRevenue").toDouble();
    record.costOfGoodsSold = extractValue("costOfGoodsSold").toDouble();
    record.sellingGeneralAndAdministrative = extractValue("sellingGeneralAndAdministrative").toDouble();
    record.researchAndDevelopment = extractValue("researchAndDevelopment").toDouble();
    record.depreciationAndAmortization = extractValue("depreciationAndAmortization").toDouble();
    record.interestExpense = extractValue("interestExpense").toDouble();
    record.taxRate = extractValue("taxRate").toDouble();

    record.compute();
    return record;
}

} // namespace corporate_planning::core
