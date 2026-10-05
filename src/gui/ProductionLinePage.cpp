#include "corporate_planning/gui/ProductionLinePage.hpp"

#include <QAbstractItemView>
#include <QAbstractSpinBox>
#include <QColor>
#include <QComboBox>
#include <QDate>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace corporate_planning::gui {

namespace {

constexpr double MAX_AMOUNT = 1.0e12;

const QLocale& numberLocale() {
    static const QLocale locale(QLocale::English, QLocale::UnitedStates);
    return locale;
}

QString formatAmount(double value, int decimals = 2) {
    return numberLocale().toString(value, 'f', decimals);
}

QString formatCurrency(double value) {
    return "$" + numberLocale().toString(value, 'f', 2);
}

QString formatPercent(double value) {
    return numberLocale().toString(value, 'f', 2) + " %";
}

} // namespace

ProductionLinePage::ProductionLinePage(core::ProductionService& service, QWidget* parent)
    : QWidget(parent),
      productionService(service) {
    setLayoutDirection(Qt::LeftToRight);

    auto* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(18);

    auto* columns = new QHBoxLayout();
    columns->setContentsMargins(0, 0, 0, 0);
    columns->setSpacing(18);

    columns->addWidget(buildEntryCard(), 3);
    columns->addWidget(buildDirectoryCard(), 4);

    pageLayout->addLayout(columns, 1);

    populateLineCombo();
    refreshDirectory();

    const int today = QDate::currentDate().year();
    const int startYear = qBound(
        core::ProductionService::MIN_FISCAL_YEAR,
        today,
        core::ProductionService::MAX_FISCAL_YEAR
    );
    {
        const QSignalBlocker blocker(fiscalYearSpin);
        fiscalYearSpin->setValue(startYear);
    }
    handleYearChanged(startYear);
}

QWidget* ProductionLinePage::buildEntryCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(26, 24, 26, 24);
    cardLayout->setSpacing(12);

    auto* titleLabel = new QLabel("Production Lines Entry", card);
    titleLabel->setObjectName("cardTitle");

    auto* subtitleLabel = new QLabel("Template 3 - Historical Operational & Capacity Records", card);
    subtitleLabel->setObjectName("cardSubtitle");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);

    auto* scrollArea = new QScrollArea(card);
    scrollArea->setObjectName("formScroll");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* formContainer = new QWidget(scrollArea);
    formContainer->setObjectName("formContainer");
    auto* formLayout = new QVBoxLayout(formContainer);
    formLayout->setContentsMargins(0, 2, 8, 2);
    formLayout->setSpacing(14);

    // Top Selectors: Production Line and Fiscal Year SpinBox (Identical to Balance Sheet & Income Statement)
    auto* topForm = new QFormLayout();
    topForm->setContentsMargins(0, 0, 0, 0);
    topForm->setHorizontalSpacing(14);
    topForm->setVerticalSpacing(10);
    topForm->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    topForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    auto* lineLabel = new QLabel("Production Line", formContainer);
    lineLabel->setObjectName("formLabel");

    lineSelectorCombo = new QComboBox(formContainer);
    lineSelectorCombo->setObjectName("formCombo");
    lineSelectorCombo->setMinimumHeight(36);
    topForm->addRow(lineLabel, lineSelectorCombo);

    auto* yearLabel = new QLabel("Fiscal Year", formContainer);
    yearLabel->setObjectName("formLabel");

    fiscalYearSpin = new QSpinBox(formContainer);
    fiscalYearSpin->setObjectName("fiscalYearSpin");
    fiscalYearSpin->setRange(
        core::ProductionService::MIN_FISCAL_YEAR,
        core::ProductionService::MAX_FISCAL_YEAR
    );
    fiscalYearSpin->setMinimumHeight(36);
    fiscalYearSpin->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    fiscalYearSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    fiscalYearSpin->setGroupSeparatorShown(true);
    fiscalYearSpin->setToolTip("Select the fiscal year to record (1990 - 2100)");
    topForm->addRow(yearLabel, fiscalYearSpin);

    formLayout->addLayout(topForm);

    connect(
        lineSelectorCombo,
        qOverload<int>(&QComboBox::currentIndexChanged),
        this,
        &ProductionLinePage::handleLineChanged
    );

    connect(
        fiscalYearSpin,
        qOverload<int>(&QSpinBox::valueChanged),
        this,
        &ProductionLinePage::handleYearChanged
    );

    // Section 1: Capacity & Output
    auto* capacitySection = addSection(formContainer, formLayout, "Capacity & Production Volume");
    nominalCapacitySpin = addAmountField(capacitySection, "Nominal (Design) Capacity (Units)", 0.0, 1.0e9, 1000.0, 2);
    actualOutputSpin = addAmountField(capacitySection, "Actual Output (Units)", 0.0, 1.0e9, 1000.0, 2);

    // Section 2: Losses & Unscheduled Downtime
    auto* lossSection = addSection(formContainer, formLayout, "Losses & Unscheduled Downtime");
    downtimeHoursSpin = addAmountField(lossSection, "Downtime (Hours)", 0.0, 8760.0, 10.0, 2, " h");
    defectUnitsSpin = addAmountField(lossSection, "Defect / Scrap (Units)", 0.0, 1.0e9, 50.0, 2);

    // Section 3: Direct Labor & Maintenance Costs
    auto* costSection = addSection(formContainer, formLayout, "Direct Labor & Maintenance Expenses");
    directLaborHoursSpin = addAmountField(costSection, "Direct Labor Hours", 0.0, 1.0e8, 500.0, 2, " h");
    maintenanceCostSpin = addAmountField(costSection, "Maintenance Cost ($)", 0.0, MAX_AMOUNT, 1000.0, 2, " $");

    // Real-Time Computed Summary Card (matching BalanceSheet summaryPanel)
    auto* summaryPanel = new QFrame(formContainer);
    summaryPanel->setObjectName("summaryPanel");
    auto* summaryLayout = new QVBoxLayout(summaryPanel);
    summaryLayout->setContentsMargins(16, 14, 16, 14);
    summaryLayout->setSpacing(10);

    auto* summaryTitle = new QLabel("Real-time Operational Metrics", summaryPanel);
    summaryTitle->setObjectName("formSectionHeader");
    summaryLayout->addWidget(summaryTitle);

    auto* summaryForm = new QFormLayout();
    summaryForm->setContentsMargins(0, 0, 0, 0);
    summaryForm->setHorizontalSpacing(14);
    summaryForm->setVerticalSpacing(8);
    summaryForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    capacityUtilizationLabel = addSummaryRow(summaryForm, "Capacity Utilization Rate");
    capacityUtilizationLabel->setObjectName("totalValueStrong");

    yieldRateLabel = addSummaryRow(summaryForm, "Production Yield Rate");
    yieldRateLabel->setObjectName("totalValueStrong");

    efficiencyIndexLabel = addSummaryRow(summaryForm, "Efficiency Index (OEE)");
    efficiencyIndexLabel->setObjectName("totalValueStrong");

    scrapRateLabel = addSummaryRow(summaryForm, "Scrap / Defect Rate");
    unitCostLabel = addSummaryRow(summaryForm, "Direct Unit Cost");
    unitCostLabel->setObjectName("totalValueStrong");

    summaryLayout->addLayout(summaryForm);

    oeeBadgeLabel = new QLabel(summaryPanel);
    oeeBadgeLabel->setObjectName("balanceBadge");
    oeeBadgeLabel->setWordWrap(true);
    oeeBadgeLabel->setAlignment(Qt::AlignCenter);
    summaryLayout->addWidget(oeeBadgeLabel);

    formLayout->addWidget(summaryPanel);
    formLayout->addStretch();

    scrollArea->setWidget(formContainer);
    cardLayout->addWidget(scrollArea, 1);

    statusLabel = new QLabel(card);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setWordWrap(true);
    statusLabel->setVisible(false);
    cardLayout->addWidget(statusLabel);

    auto* buttonRow = new QHBoxLayout();
    buttonRow->setContentsMargins(0, 4, 0, 0);
    buttonRow->setSpacing(10);

    auto* saveButton = new QPushButton("Save Record", card);
    saveButton->setObjectName("primaryButton");
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setMinimumHeight(42);
    saveButton->setDefault(true);
    connect(saveButton, &QPushButton::clicked, this, &ProductionLinePage::handleSave);

    auto* clearButton = new QPushButton("Clear / Reset", card);
    clearButton->setObjectName("secondaryButton");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setMinimumHeight(42);
    connect(clearButton, &QPushButton::clicked, this, &ProductionLinePage::handleClear);

    auto* deleteButton = new QPushButton("Delete Year", card);
    deleteButton->setObjectName("dangerButton");
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setMinimumHeight(42);
    connect(deleteButton, &QPushButton::clicked, this, &ProductionLinePage::handleDelete);

    buttonRow->addWidget(saveButton, 2);
    buttonRow->addWidget(clearButton, 1);
    buttonRow->addWidget(deleteButton, 1);
    cardLayout->addLayout(buttonRow);

    return card;
}

QWidget* ProductionLinePage::buildDirectoryCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(26, 24, 26, 24);
    cardLayout->setSpacing(12);

    auto* titleLabel = new QLabel("Historical Directory", card);
    titleLabel->setObjectName("cardTitle");

    directoryCountLabel = new QLabel(card);
    directoryCountLabel->setObjectName("cardSubtitle");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(directoryCountLabel);

    recordsTable = new QTableWidget(0, 7, card);
    recordsTable->setObjectName("balanceSheetTable");
    recordsTable->setHorizontalHeaderLabels({
        "Year", "Actual Output", "Nominal Cap.", "Util. (%)", "Yield Rate", "Efficiency (OEE)", "Unit Cost"
    });
    recordsTable->verticalHeader()->setVisible(false);
    recordsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    recordsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    recordsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recordsTable->setAlternatingRowColors(false);
    recordsTable->setShowGrid(false);
    recordsTable->horizontalHeader()->setHighlightSections(false);
    recordsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    recordsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    recordsTable->setCursor(Qt::PointingHandCursor);

    connect(recordsTable, &QTableWidget::cellClicked, this, &ProductionLinePage::handleDirectorySelection);

    cardLayout->addWidget(recordsTable, 1);

    auto* hintLabel = new QLabel("Select a row to load that fiscal year back into the form.", card);
    hintLabel->setObjectName("cardDescription");
    hintLabel->setWordWrap(true);
    cardLayout->addWidget(hintLabel);

    return card;
}

QFormLayout* ProductionLinePage::addSection(QWidget* parent, QVBoxLayout* container, const QString& title) {
    auto* header = new QLabel(title, parent);
    header->setObjectName("formSectionHeader");
    container->addWidget(header);

    auto* form = new QFormLayout();
    form->setContentsMargins(0, 0, 0, 0);
    form->setHorizontalSpacing(14);
    form->setVerticalSpacing(10);
    form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    form->setRowWrapPolicy(QFormLayout::WrapLongRows);
    container->addLayout(form);

    return form;
}

QDoubleSpinBox* ProductionLinePage::addAmountField(
    QFormLayout* layout,
    const QString& label,
    double minVal,
    double maxVal,
    double step,
    int decimals,
    const QString& suffix)
{
    auto* fieldLabel = new QLabel(label);
    fieldLabel->setObjectName("formLabel");

    auto* spin = new QDoubleSpinBox();
    spin->setObjectName("amountInput");
    spin->setRange(minVal, maxVal);
    spin->setDecimals(decimals);
    spin->setSingleStep(step);
    spin->setGroupSeparatorShown(true);
    spin->setLocale(numberLocale());
    spin->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    spin->setMinimumHeight(36);
    spin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    if (!suffix.isEmpty()) {
        spin->setSuffix(suffix);
    }

    connect(
        spin,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        [this](double) { recalculateSummary(); }
    );

    layout->addRow(fieldLabel, spin);
    return spin;
}

QLabel* ProductionLinePage::addSummaryRow(QFormLayout* layout, const QString& label) {
    auto* nameLabel = new QLabel(label);
    nameLabel->setObjectName("summaryName");

    auto* valueLabel = new QLabel("0.00 %");
    valueLabel->setObjectName("totalValue");
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    layout->addRow(nameLabel, valueLabel);
    return valueLabel;
}

QList<QDoubleSpinBox*> ProductionLinePage::amountFields() const {
    return {
        nominalCapacitySpin,
        actualOutputSpin,
        downtimeHoursSpin,
        defectUnitsSpin,
        directLaborHoursSpin,
        maintenanceCostSpin
    };
}

void ProductionLinePage::populateLineCombo() {
    const QSignalBlocker blocker(lineSelectorCombo);
    lineSelectorCombo->clear();

    const auto lines = productionService.getAllLines();
    for (const auto& line : lines) {
        const QString itemText = QString("%1: %2")
            .arg(QString::fromStdString(line.getId()), QString::fromStdString(line.getName()));
        lineSelectorCombo->addItem(itemText, QString::fromStdString(line.getId()));
    }
}

core::ProductionLineRecord ProductionLinePage::collectFormValues() const {
    core::ProductionLineRecord record;
    record.fiscalYear = fiscalYearSpin->value();
    record.lineId = lineSelectorCombo->currentData().toString().toStdString();

    const auto lineOpt = productionService.getLine(record.lineId);
    if (lineOpt) {
        record.lineName = lineOpt->getName();
    }

    record.nominalCapacity = nominalCapacitySpin->value();
    record.actualOutput = actualOutputSpin->value();
    record.downtimeHours = downtimeHoursSpin->value();
    record.defectUnits = defectUnitsSpin->value();
    record.directLaborHours = directLaborHoursSpin->value();
    record.maintenanceCost = maintenanceCostSpin->value();

    record.compute();
    return record;
}

void ProductionLinePage::applyRecord(const core::ProductionLineRecord& record) {
    {
        const QSignalBlocker blocker1(nominalCapacitySpin);
        const QSignalBlocker blocker2(actualOutputSpin);
        const QSignalBlocker blocker3(downtimeHoursSpin);
        const QSignalBlocker blocker4(defectUnitsSpin);
        const QSignalBlocker blocker5(directLaborHoursSpin);
        const QSignalBlocker blocker6(maintenanceCostSpin);

        nominalCapacitySpin->setValue(record.nominalCapacity);
        actualOutputSpin->setValue(record.actualOutput);
        downtimeHoursSpin->setValue(record.downtimeHours);
        defectUnitsSpin->setValue(record.defectUnits);
        directLaborHoursSpin->setValue(record.directLaborHours);
        maintenanceCostSpin->setValue(record.maintenanceCost);
    }

    recalculateSummary();
}

void ProductionLinePage::clearAmountFields() {
    // If a line is selected, reset nominal capacity to default line capacity if available
    const std::string lineId = lineSelectorCombo->currentData().toString().toStdString();
    const auto lineOpt = productionService.getLine(lineId);
    const double defaultNominal = lineOpt ? lineOpt->getDefaultNominalCapacity() : 0.0;

    for (auto* spin : amountFields()) {
        const QSignalBlocker blocker(spin);
        spin->setValue(0.0);
    }

    if (defaultNominal > 0.0) {
        const QSignalBlocker blocker(nominalCapacitySpin);
        nominalCapacitySpin->setValue(defaultNominal);
    }

    recalculateSummary();
}

void ProductionLinePage::recalculateSummary() {
    const core::ProductionLineRecord record = collectFormValues();

    capacityUtilizationLabel->setText(formatPercent(record.capacityUtilization));
    yieldRateLabel->setText(formatPercent(record.yieldRate));
    efficiencyIndexLabel->setText(formatPercent(record.oee));
    scrapRateLabel->setText(formatPercent(record.defectRate));
    unitCostLabel->setText(formatCurrency(record.unitProductionCost) + " / unit");

    // Dynamic Badge Status based on OEE and Operational Indicators
    if (record.actualOutput <= 0.0 && record.nominalCapacity <= 0.0) {
        oeeBadgeLabel->setText("No operational activity entered");
        oeeBadgeLabel->setProperty("state", "info");
    } else if (record.oee >= 75.0) {
        oeeBadgeLabel->setText(
            QString("World-Class Operational Efficiency - OEE %1 (Utilization %2, Yield %3)")
                .arg(formatPercent(record.oee), formatPercent(record.capacityUtilization), formatPercent(record.yieldRate))
        );
        oeeBadgeLabel->setProperty("state", "success");
    } else if (record.oee >= 60.0) {
        oeeBadgeLabel->setText(
            QString("Acceptable Operating Effectiveness - OEE %1 (Monitor downtime and defects)")
                .arg(formatPercent(record.oee))
        );
        oeeBadgeLabel->setProperty("state", "info");
    } else {
        oeeBadgeLabel->setText(
            QString("Sub-optimal Performance - OEE %1 (High downtime of %2 hrs or defect rate %3)")
                .arg(formatPercent(record.oee), formatAmount(record.downtimeHours, 1), formatPercent(record.defectRate))
        );
        oeeBadgeLabel->setProperty("state", "error");
    }

    oeeBadgeLabel->style()->unpolish(oeeBadgeLabel);
    oeeBadgeLabel->style()->polish(oeeBadgeLabel);
}

void ProductionLinePage::refreshDirectory() {
    const std::string lineId = lineSelectorCombo->currentData().toString().toStdString();
    const auto records = productionService.getRecordsByLine(lineId);

    const auto lineOpt = productionService.getLine(lineId);
    const QString lineName = lineOpt ? QString::fromStdString(lineOpt->getName()) : "Selected Line";

    directoryCountLabel->setText(
        records.empty()
            ? QString("No records found for %1").arg(lineName)
            : QString("%1 historical record(s) recorded for %2").arg(records.size()).arg(lineName)
    );

    recordsTable->setRowCount(static_cast<int>(records.size()));
    for (int row = 0; row < static_cast<int>(records.size()); ++row) {
        const auto& rec = records[static_cast<std::size_t>(row)];

        const QStringList values{
            QString::number(rec.fiscalYear),
            formatAmount(rec.actualOutput, 0),
            formatAmount(rec.nominalCapacity, 0),
            formatPercent(rec.capacityUtilization),
            formatPercent(rec.yieldRate),
            formatPercent(rec.oee),
            formatCurrency(rec.unitProductionCost)
        };

        for (int col = 0; col < values.size(); ++col) {
            auto* item = new QTableWidgetItem(values.at(col));
            if (col == 0) {
                item->setData(Qt::UserRole, rec.fiscalYear);
                item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            } else if (col == 5) {
                item->setForeground(QColor(rec.oee >= 75.0 ? "#22c55e" : (rec.oee >= 60.0 ? "#38bdf8" : "#ef4444")));
                item->setTextAlignment(Qt::AlignCenter);
            } else {
                item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            recordsTable->setItem(row, col, item);
        }
    }
}

void ProductionLinePage::selectDirectoryYear(int year) {
    for (int row = 0; row < recordsTable->rowCount(); ++row) {
        const auto* item = recordsTable->item(row, 0);
        if (item != nullptr && item->data(Qt::UserRole).toInt() == year) {
            recordsTable->selectRow(row);
            recordsTable->scrollToItem(item, QAbstractItemView::PositionAtCenter);
            return;
        }
    }
    recordsTable->clearSelection();
}

void ProductionLinePage::handleLineChanged(int index) {
    Q_UNUSED(index);
    refreshDirectory();
    handleYearChanged(fiscalYearSpin->value());
}

void ProductionLinePage::handleYearChanged(int year) {
    const std::string lineId = lineSelectorCombo->currentData().toString().toStdString();

    const auto recOpt = productionService.getRecord(lineId, year);
    if (recOpt) {
        applyRecord(*recOpt);
        selectDirectoryYear(year);
        showStatus(
            QString("Loaded operational record for %1 (FY %2).")
                .arg(QString::fromStdString(lineId)).arg(year),
            StatusKind::Info
        );
    } else {
        clearAmountFields();
        recordsTable->clearSelection();
        showStatus(
            QString("No saved record for %1 in FY %2 yet. Enter the figures and choose Save Record.")
                .arg(QString::fromStdString(lineId)).arg(year),
            StatusKind::Info
        );
    }
}

void ProductionLinePage::handleDirectorySelection(int row, int column) {
    Q_UNUSED(column);
    if (row < 0) return;

    const auto* yearItem = recordsTable->item(row, 0);
    if (yearItem == nullptr) return;

    const int year = yearItem->data(Qt::UserRole).toInt();

    {
        const QSignalBlocker blocker(fiscalYearSpin);
        fiscalYearSpin->setValue(year);
    }

    const std::string lineId = lineSelectorCombo->currentData().toString().toStdString();
    const auto recOpt = productionService.getRecord(lineId, year);
    if (recOpt) {
        applyRecord(*recOpt);
        showStatus(
            QString("Loaded operational record for %1 (FY %2).")
                .arg(QString::fromStdString(lineId)).arg(year),
            StatusKind::Info
        );
    }
}

void ProductionLinePage::handleSave() {
    const core::ProductionLineRecord record = collectFormValues();

    if (record.actualOutput > record.nominalCapacity && record.nominalCapacity > 0.0) {
        showStatus("Note: Actual output exceeds nominal design capacity (overtime/surge mode).", StatusKind::Info);
    }

    const core::ProductionSaveResult result = productionService.saveRecord(record);
    if (!result.success) {
        showStatus(QString::fromStdString(result.message), StatusKind::Error);
        return;
    }

    refreshDirectory();
    selectDirectoryYear(record.fiscalYear);
    showStatus(QString::fromStdString(result.message), StatusKind::Success);
}

void ProductionLinePage::handleClear() {
    clearAmountFields();
    showStatus(
        QString("Form cleared for %1 (FY %2). Enter new operational parameters.")
            .arg(lineSelectorCombo->currentData().toString())
            .arg(fiscalYearSpin->value()),
        StatusKind::Info
    );
}

void ProductionLinePage::handleDelete() {
    const std::string lineId = lineSelectorCombo->currentData().toString().toStdString();
    const int year = fiscalYearSpin->value();

    if (!productionService.hasRecord(lineId, year)) {
        showStatus(
            QString("No saved record exists for %1 in FY %2.")
                .arg(QString::fromStdString(lineId)).arg(year),
            StatusKind::Error
        );
        return;
    }

    const auto choice = QMessageBox::question(
        this,
        "Delete Historical Record",
        QString("Are you sure you want to delete the operational record for %1 in FY %2?")
            .arg(QString::fromStdString(lineId)).arg(year),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (choice != QMessageBox::Yes) {
        return;
    }

    if (productionService.deleteRecord(lineId, year)) {
        refreshDirectory();
        clearAmountFields();
        showStatus(
            QString("Operational record for %1 (FY %2) deleted successfully.")
                .arg(QString::fromStdString(lineId)).arg(year),
            StatusKind::Info
        );
    }
}

void ProductionLinePage::showStatus(const QString& message, StatusKind kind) {
    const char* state = "info";
    if (kind == StatusKind::Success) {
        state = "success";
    } else if (kind == StatusKind::Error) {
        state = "error";
    }

    statusLabel->setText(message);
    statusLabel->setProperty("state", state);
    statusLabel->setVisible(!message.isEmpty());
    statusLabel->style()->unpolish(statusLabel);
    statusLabel->style()->polish(statusLabel);
}

} // namespace corporate_planning::gui
