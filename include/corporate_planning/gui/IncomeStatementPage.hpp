#ifndef CORPORATE_PLANNING_GUI_INCOME_STATEMENT_PAGE_HPP
#define CORPORATE_PLANNING_GUI_INCOME_STATEMENT_PAGE_HPP

#include "corporate_planning/core/IncomeStatementService.hpp"

#include <QList>
#include <QWidget>

class QDoubleSpinBox;
class QFormLayout;
class QLabel;
class QSpinBox;
class QTableWidget;
class QVBoxLayout;

namespace corporate_planning::gui {

class IncomeStatementPage final : public QWidget {

public:
    explicit IncomeStatementPage(core::IncomeStatementService& service,
                                 QWidget* parent = nullptr);

private:
    enum class StatusKind { Info, Success, Error };

    QWidget* buildEntryCard();
    QWidget* buildDirectoryCard();
    QFormLayout* addSection(QWidget* parent, QVBoxLayout* container,
                            const QString& title);
    QDoubleSpinBox* addAmountField(QFormLayout* layout, const QString& label);
    QLabel* addSummaryRow(QFormLayout* layout, const QString& label);
    QList<QDoubleSpinBox*> amountFields() const;

    core::IncomeStatementRecord collectFormValues() const;
    void applyRecord(const core::IncomeStatementRecord& record);
    void clearAmountFields();
    void recalculateSummary();

    void refreshDirectory();
    void selectDirectoryYear(int year);
    void handleYearChanged(int year);
    void handleDirectorySelection(int row, int column);
    void handleSave();
    void handleClear();
    void handleDelete();
    void showStatus(const QString& message, StatusKind kind);

    core::IncomeStatementService& incomeStatementService;

    QSpinBox* fiscalYearSpin = nullptr;

    QDoubleSpinBox* grossRevenueSpin = nullptr;
    QDoubleSpinBox* cogsSpin = nullptr;
    QDoubleSpinBox* sgaSpin = nullptr;
    QDoubleSpinBox* rdSpin = nullptr;
    QDoubleSpinBox* daSpin = nullptr;
    QDoubleSpinBox* interestExpenseSpin = nullptr;
    QDoubleSpinBox* taxRateSpin = nullptr;

    QLabel* grossProfitLabel = nullptr;
    QLabel* ebitLabel = nullptr;
    QLabel* ebtLabel = nullptr;
    QLabel* netIncomeLabel = nullptr;
    QLabel* netProfitMarginLabel = nullptr;

    QLabel* statusLabel = nullptr;
    QLabel* directoryCountLabel = nullptr;
    QTableWidget* recordsTable = nullptr;
};

} // namespace corporate_planning::gui

#endif // CORPORATE_PLANNING_GUI_INCOME_STATEMENT_PAGE_HPP
