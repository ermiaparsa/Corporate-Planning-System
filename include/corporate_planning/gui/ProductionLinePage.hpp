#ifndef CORPORATE_PLANNING_GUI_PRODUCTION_LINE_PAGE_HPP
#define CORPORATE_PLANNING_GUI_PRODUCTION_LINE_PAGE_HPP

#include "corporate_planning/core/ProductionService.hpp"

#include <QList>
#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QFormLayout;
class QLabel;
class QSpinBox;
class QTableWidget;
class QVBoxLayout;

namespace corporate_planning::gui {

// Template 3: multi-year historical operational records for production lines
// with live recalculation of operational indicators (Capacity Utilization, Yield Rate,
// Defect Rate, OEE Efficiency, Unit Cost) plus a directory of recorded fiscal years.
class ProductionLinePage final : public QWidget {

public:
    explicit ProductionLinePage(core::ProductionService& service, QWidget* parent = nullptr);

private:
    enum class StatusKind { Info, Success, Error };

    QWidget* buildEntryCard();
    QWidget* buildDirectoryCard();
    QFormLayout* addSection(QWidget* parent, QVBoxLayout* container, const QString& title);
    QDoubleSpinBox* addAmountField(
        QFormLayout* layout,
        const QString& label,
        double minVal = 0.0,
        double maxVal = 1.0e12,
        double step = 1000.0,
        int decimals = 2,
        const QString& suffix = ""
    );
    QLabel* addSummaryRow(QFormLayout* layout, const QString& label);
    QList<QDoubleSpinBox*> amountFields() const;

    core::ProductionLineRecord collectFormValues() const;
    void applyRecord(const core::ProductionLineRecord& record);
    void clearAmountFields();
    void recalculateSummary();

    void populateLineCombo();
    void refreshDirectory();
    void selectDirectoryYear(int year);

    void handleLineChanged(int index);
    void handleYearChanged(int year);
    void handleDirectorySelection(int row, int column);
    void handleSave();
    void handleClear();
    void handleDelete();
    void showStatus(const QString& message, StatusKind kind);

    core::ProductionService& productionService;

    // Top Controls
    QComboBox* lineSelectorCombo = nullptr;
    QSpinBox* fiscalYearSpin = nullptr;

    // Numerical SpinBox Inputs matching BalanceSheet & IncomeStatement style
    QDoubleSpinBox* nominalCapacitySpin = nullptr;
    QDoubleSpinBox* actualOutputSpin = nullptr;
    QDoubleSpinBox* downtimeHoursSpin = nullptr;
    QDoubleSpinBox* defectUnitsSpin = nullptr;
    QDoubleSpinBox* directLaborHoursSpin = nullptr;
    QDoubleSpinBox* maintenanceCostSpin = nullptr;

    // Real-time Computed Summary Card
    QLabel* capacityUtilizationLabel = nullptr;
    QLabel* yieldRateLabel = nullptr;
    QLabel* efficiencyIndexLabel = nullptr;
    QLabel* scrapRateLabel = nullptr;
    QLabel* unitCostLabel = nullptr;
    QLabel* oeeBadgeLabel = nullptr;

    // Status Banner & Directory Table
    QLabel* statusLabel = nullptr;
    QLabel* directoryCountLabel = nullptr;
    QTableWidget* recordsTable = nullptr;
};

} // namespace corporate_planning::gui

#endif // CORPORATE_PLANNING_GUI_PRODUCTION_LINE_PAGE_HPP
