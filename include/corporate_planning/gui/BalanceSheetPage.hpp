#ifndef CORPORATE_PLANNING_GUI_BALANCE_SHEET_PAGE_HPP
#define CORPORATE_PLANNING_GUI_BALANCE_SHEET_PAGE_HPP

#include "corporate_planning/core/BalanceSheetService.hpp"

#include <QList>
#include <QWidget>

class QDoubleSpinBox;
class QFormLayout;
class QLabel;
class QSpinBox;
class QTableWidget;
class QVBoxLayout;

namespace corporate_planning::gui {

// Template 1: multi-year historical balance sheet entry form with live
// recalculation of totals plus a directory of every recorded fiscal year.
class BalanceSheetPage final : public QWidget {

public:
    explicit BalanceSheetPage(core::BalanceSheetService& service, QWidget* parent = nullptr);

private:
    enum class StatusKind { Info, Success, Error };

    QWidget* buildEntryCard();
    QWidget* buildDirectoryCard();
    QFormLayout* addSection(QWidget* parent, QVBoxLayout* container, const QString& title);
    QDoubleSpinBox* addAmountField(QFormLayout* layout, const QString& label);
    QLabel* addSummaryRow(QFormLayout* layout, const QString& label);
    QList<QDoubleSpinBox*> amountFields() const;

    core::BalanceSheetRecord collectFormValues() const;
    void applyRecord(const core::BalanceSheetRecord& record);
    void clearAmountFields();
    void recalculateTotals();

    void refreshDirectory();
    void selectDirectoryYear(int year);
    void handleYearChanged(int year);
    void handleDirectorySelection(int row, int column);
    void handleSave();
    void handleClear();
    void handleDelete();
    void showStatus(const QString& message, StatusKind kind);

    core::BalanceSheetService& balanceSheetService;

    QSpinBox* fiscalYearSpin = nullptr;

    QDoubleSpinBox* cashAndEquivalentsSpin = nullptr;
    QDoubleSpinBox* accountsReceivableSpin = nullptr;
    QDoubleSpinBox* inventorySpin = nullptr;
    QDoubleSpinBox* shortTermInvestmentsSpin = nullptr;
    QDoubleSpinBox* otherCurrentAssetsSpin = nullptr;

    QDoubleSpinBox* propertyPlantEquipmentSpin = nullptr;
    QDoubleSpinBox* intangibleAssetsSpin = nullptr;
    QDoubleSpinBox* longTermInvestmentsSpin = nullptr;

    QDoubleSpinBox* accountsPayableSpin = nullptr;
    QDoubleSpinBox* shortTermDebtSpin = nullptr;
    QDoubleSpinBox* accruedExpensesSpin = nullptr;

    QDoubleSpinBox* longTermDebtSpin = nullptr;
    QDoubleSpinBox* deferredTaxLiabilitiesSpin = nullptr;

    QDoubleSpinBox* commonStockSpin = nullptr;
    QDoubleSpinBox* retainedEarningsSpin = nullptr;
    QDoubleSpinBox* additionalPaidInCapitalSpin = nullptr;

    QLabel* totalCurrentAssetsLabel = nullptr;
    QLabel* totalNonCurrentAssetsLabel = nullptr;
    QLabel* totalAssetsLabel = nullptr;
    QLabel* totalCurrentLiabilitiesLabel = nullptr;
    QLabel* totalNonCurrentLiabilitiesLabel = nullptr;
    QLabel* totalLiabilitiesLabel = nullptr;
    QLabel* totalEquityLabel = nullptr;
    QLabel* balanceIndicatorLabel = nullptr;

    QLabel* statusLabel = nullptr;
    QLabel* directoryCountLabel = nullptr;
    QTableWidget* recordsTable = nullptr;
};

} // namespace corporate_planning::gui

#endif // CORPORATE_PLANNING_GUI_BALANCE_SHEET_PAGE_HPP
