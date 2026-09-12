#include "corporate_planning/gui/BalanceSheetPage.hpp"

#include <QAbstractItemView>
#include <QAbstractSpinBox>
#include <QColor>
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

const QLocale& moneyLocale() {
    static const QLocale locale(QLocale::English, QLocale::UnitedStates);
    return locale;
}

QString formatAmount(double value) {
    return moneyLocale().toString(value, 'f', 2);
}

} // namespace

BalanceSheetPage::BalanceSheetPage(core::BalanceSheetService& service, QWidget* parent)
    : QWidget(parent),
      balanceSheetService(service) {
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

    refreshDirectory();

    const int today = QDate::currentDate().year();
    const int startYear = qBound(
        core::BalanceSheetService::MIN_FISCAL_YEAR,
        today,
        core::BalanceSheetService::MAX_FISCAL_YEAR
    );
    {
        const QSignalBlocker blocker(fiscalYearSpin);
        fiscalYearSpin->setValue(startYear);
    }
    handleYearChanged(startYear);
}

QWidget* BalanceSheetPage::buildEntryCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(26, 24, 26, 24);
    cardLayout->setSpacing(12);

    auto* titleLabel = new QLabel("Balance Sheet Entry", card);
    titleLabel->setObjectName("cardTitle");

    auto* subtitleLabel = new QLabel("Template 1 - Historical Baseline Data", card);
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

    auto* yearForm = new QFormLayout();
    yearForm->setContentsMargins(0, 0, 0, 0);
    yearForm->setHorizontalSpacing(14);
    yearForm->setVerticalSpacing(10);
    yearForm->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    yearForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    auto* yearLabel = new QLabel("Fiscal Year", formContainer);
    yearLabel->setObjectName("formLabel");

    fiscalYearSpin = new QSpinBox(formContainer);
    fiscalYearSpin->setObjectName("fiscalYearSpin");
    fiscalYearSpin->setRange(
        core::BalanceSheetService::MIN_FISCAL_YEAR,
        core::BalanceSheetService::MAX_FISCAL_YEAR
    );
    fiscalYearSpin->setMinimumHeight(36);
    fiscalYearSpin->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    fiscalYearSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    fiscalYearSpin->setGroupSeparatorShown(true);
    fiscalYearSpin->setToolTip("Select the fiscal year to record (1990 - 2100)");
    yearForm->addRow(yearLabel, fiscalYearSpin);
    formLayout->addLayout(yearForm);

    connect(
        fiscalYearSpin,
        qOverload<int>(&QSpinBox::valueChanged),
        this,
        &BalanceSheetPage::handleYearChanged
    );

    auto* currentAssetsForm = addSection(formContainer, formLayout, "Current Assets");
    cashAndEquivalentsSpin = addAmountField(currentAssetsForm, "Cash & Equivalents");
    accountsReceivableSpin = addAmountField(currentAssetsForm, "Accounts Receivable");
    inventorySpin = addAmountField(currentAssetsForm, "Inventory");
    shortTermInvestmentsSpin = addAmountField(currentAssetsForm, "Short-term Investments");
    otherCurrentAssetsSpin = addAmountField(currentAssetsForm, "Other Current Assets");

    auto* nonCurrentAssetsForm = addSection(formContainer, formLayout, "Non-Current Assets");
    propertyPlantEquipmentSpin = addAmountField(nonCurrentAssetsForm, "Property, Plant & Equipment");
    intangibleAssetsSpin = addAmountField(nonCurrentAssetsForm, "Intangible Assets");
    longTermInvestmentsSpin = addAmountField(nonCurrentAssetsForm, "Long-term Investments");

    auto* currentLiabilitiesForm = addSection(formContainer, formLayout, "Current Liabilities");
    accountsPayableSpin = addAmountField(currentLiabilitiesForm, "Accounts Payable");
    shortTermDebtSpin = addAmountField(currentLiabilitiesForm, "Short-term Debt");
    accruedExpensesSpin = addAmountField(currentLiabilitiesForm, "Accrued Expenses");

    auto* nonCurrentLiabilitiesForm = addSection(formContainer, formLayout, "Non-Current Liabilities");
    longTermDebtSpin = addAmountField(nonCurrentLiabilitiesForm, "Long-term Debt");
    deferredTaxLiabilitiesSpin = addAmountField(nonCurrentLiabilitiesForm, "Deferred Tax Liabilities");

    auto* equityForm = addSection(formContainer, formLayout, "Equity");
    commonStockSpin = addAmountField(equityForm, "Common Stock");
    retainedEarningsSpin = addAmountField(equityForm, "Retained Earnings");
    additionalPaidInCapitalSpin = addAmountField(equityForm, "Additional Paid-in Capital");

    auto* summaryPanel = new QFrame(formContainer);
    summaryPanel->setObjectName("summaryPanel");
    auto* summaryLayout = new QVBoxLayout(summaryPanel);
    summaryLayout->setContentsMargins(16, 14, 16, 14);
    summaryLayout->setSpacing(10);

    auto* summaryTitle = new QLabel("Computed Totals", summaryPanel);
    summaryTitle->setObjectName("formSectionHeader");
    summaryLayout->addWidget(summaryTitle);

    auto* summaryForm = new QFormLayout();
    summaryForm->setContentsMargins(0, 0, 0, 0);
    summaryForm->setHorizontalSpacing(14);
    summaryForm->setVerticalSpacing(8);
    summaryForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    summaryForm->setRowWrapPolicy(QFormLayout::WrapLongRows);

    totalCurrentAssetsLabel = addSummaryRow(summaryForm, "Total Current Assets");
    totalNonCurrentAssetsLabel = addSummaryRow(summaryForm, "Total Non-Current Assets");
    totalAssetsLabel = addSummaryRow(summaryForm, "Total Assets");
    totalAssetsLabel->setObjectName("totalValueStrong");

    totalCurrentLiabilitiesLabel = addSummaryRow(summaryForm, "Total Current Liabilities");
    totalNonCurrentLiabilitiesLabel = addSummaryRow(summaryForm, "Total Non-Current Liabilities");
    totalLiabilitiesLabel = addSummaryRow(summaryForm, "Total Liabilities");
    totalLiabilitiesLabel->setObjectName("totalValueStrong");

    totalEquityLabel = addSummaryRow(summaryForm, "Total Equity");
    totalEquityLabel->setObjectName("totalValueStrong");

    summaryLayout->addLayout(summaryForm);

    balanceIndicatorLabel = new QLabel(summaryPanel);
    balanceIndicatorLabel->setObjectName("balanceBadge");
    balanceIndicatorLabel->setWordWrap(true);
    balanceIndicatorLabel->setAlignment(Qt::AlignCenter);
    summaryLayout->addWidget(balanceIndicatorLabel);

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
    connect(saveButton, &QPushButton::clicked, this, &BalanceSheetPage::handleSave);

    auto* clearButton = new QPushButton("Clear / Reset", card);
    clearButton->setObjectName("secondaryButton");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setMinimumHeight(42);
    connect(clearButton, &QPushButton::clicked, this, &BalanceSheetPage::handleClear);

    auto* deleteButton = new QPushButton("Delete Year", card);
    deleteButton->setObjectName("dangerButton");
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setMinimumHeight(42);
    connect(deleteButton, &QPushButton::clicked, this, &BalanceSheetPage::handleDelete);

    buttonRow->addWidget(saveButton, 2);
    buttonRow->addWidget(clearButton, 1);
    buttonRow->addWidget(deleteButton, 1);
    cardLayout->addLayout(buttonRow);

    return card;
}

QWidget* BalanceSheetPage::buildDirectoryCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 24, 24, 24);
    cardLayout->setSpacing(12);

    auto* titleLabel = new QLabel("Historical Directory", card);
    titleLabel->setObjectName("cardTitle");

    directoryCountLabel = new QLabel(card);
    directoryCountLabel->setObjectName("cardSubtitle");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(directoryCountLabel);

    recordsTable = new QTableWidget(0, 5, card);
    recordsTable->setObjectName("balanceSheetTable");
    recordsTable->setHorizontalHeaderLabels(
        {"Year", "Total Assets", "Total Liabilities", "Total Equity", "Balance Status"}
    );
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

    connect(
        recordsTable,
        &QTableWidget::cellClicked,
        this,
        &BalanceSheetPage::handleDirectorySelection
    );

    cardLayout->addWidget(recordsTable, 1);

    auto* hintLabel = new QLabel("Select a row to load that fiscal year back into the form.", card);
    hintLabel->setObjectName("cardDescription");
    hintLabel->setWordWrap(true);
    cardLayout->addWidget(hintLabel);

    return card;
}

QFormLayout* BalanceSheetPage::addSection(
    QWidget* parent,
    QVBoxLayout* container,
    const QString& title
) {
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

QDoubleSpinBox* BalanceSheetPage::addAmountField(QFormLayout* layout, const QString& label) {
    auto* fieldLabel = new QLabel(label);
    fieldLabel->setObjectName("formLabel");

    auto* spin = new QDoubleSpinBox();
    spin->setObjectName("amountInput");
    spin->setRange(-MAX_AMOUNT, MAX_AMOUNT);
    spin->setDecimals(2);
    spin->setSingleStep(1000.0);
    spin->setGroupSeparatorShown(true);
    spin->setLocale(moneyLocale());
    spin->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    spin->setMinimumHeight(36);
    spin->setButtonSymbols(QAbstractSpinBox::NoButtons);

    connect(
        spin,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        [this](double) { recalculateTotals(); }
    );

    layout->addRow(fieldLabel, spin);
    return spin;
}

QLabel* BalanceSheetPage::addSummaryRow(QFormLayout* layout, const QString& label) {
    auto* nameLabel = new QLabel(label);
    nameLabel->setObjectName("summaryName");

    auto* valueLabel = new QLabel("0.00");
    valueLabel->setObjectName("totalValue");
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    layout->addRow(nameLabel, valueLabel);
    return valueLabel;
}

QList<QDoubleSpinBox*> BalanceSheetPage::amountFields() const {
    return {
        cashAndEquivalentsSpin,
        accountsReceivableSpin,
        inventorySpin,
        shortTermInvestmentsSpin,
        otherCurrentAssetsSpin,
        propertyPlantEquipmentSpin,
        intangibleAssetsSpin,
        longTermInvestmentsSpin,
        accountsPayableSpin,
        shortTermDebtSpin,
        accruedExpensesSpin,
        longTermDebtSpin,
        deferredTaxLiabilitiesSpin,
        commonStockSpin,
        retainedEarningsSpin,
        additionalPaidInCapitalSpin
    };
}

core::BalanceSheetRecord BalanceSheetPage::collectFormValues() const {
    core::BalanceSheetRecord record;
    record.fiscalYear = fiscalYearSpin->value();

    record.cashAndEquivalents = cashAndEquivalentsSpin->value();
    record.accountsReceivable = accountsReceivableSpin->value();
    record.inventory = inventorySpin->value();
    record.shortTermInvestments = shortTermInvestmentsSpin->value();
    record.otherCurrentAssets = otherCurrentAssetsSpin->value();

    record.propertyPlantEquipment = propertyPlantEquipmentSpin->value();
    record.intangibleAssets = intangibleAssetsSpin->value();
    record.longTermInvestments = longTermInvestmentsSpin->value();

    record.accountsPayable = accountsPayableSpin->value();
    record.shortTermDebt = shortTermDebtSpin->value();
    record.accruedExpenses = accruedExpensesSpin->value();

    record.longTermDebt = longTermDebtSpin->value();
    record.deferredTaxLiabilities = deferredTaxLiabilitiesSpin->value();

    record.commonStock = commonStockSpin->value();
    record.retainedEarnings = retainedEarningsSpin->value();
    record.additionalPaidInCapital = additionalPaidInCapitalSpin->value();

    return record;
}

void BalanceSheetPage::applyRecord(const core::BalanceSheetRecord& record) {
    {
        const QSignalBlocker blocker0(cashAndEquivalentsSpin);
        const QSignalBlocker blocker1(accountsReceivableSpin);
        const QSignalBlocker blocker2(inventorySpin);
        const QSignalBlocker blocker3(shortTermInvestmentsSpin);
        const QSignalBlocker blocker4(otherCurrentAssetsSpin);
        const QSignalBlocker blocker5(propertyPlantEquipmentSpin);
        const QSignalBlocker blocker6(intangibleAssetsSpin);
        const QSignalBlocker blocker7(longTermInvestmentsSpin);
        const QSignalBlocker blocker8(accountsPayableSpin);
        const QSignalBlocker blocker9(shortTermDebtSpin);
        const QSignalBlocker blocker10(accruedExpensesSpin);
        const QSignalBlocker blocker11(longTermDebtSpin);
        const QSignalBlocker blocker12(deferredTaxLiabilitiesSpin);
        const QSignalBlocker blocker13(commonStockSpin);
        const QSignalBlocker blocker14(retainedEarningsSpin);
        const QSignalBlocker blocker15(additionalPaidInCapitalSpin);

        cashAndEquivalentsSpin->setValue(record.cashAndEquivalents);
        accountsReceivableSpin->setValue(record.accountsReceivable);
        inventorySpin->setValue(record.inventory);
        shortTermInvestmentsSpin->setValue(record.shortTermInvestments);
        otherCurrentAssetsSpin->setValue(record.otherCurrentAssets);

        propertyPlantEquipmentSpin->setValue(record.propertyPlantEquipment);
        intangibleAssetsSpin->setValue(record.intangibleAssets);
        longTermInvestmentsSpin->setValue(record.longTermInvestments);

        accountsPayableSpin->setValue(record.accountsPayable);
        shortTermDebtSpin->setValue(record.shortTermDebt);
        accruedExpensesSpin->setValue(record.accruedExpenses);

        longTermDebtSpin->setValue(record.longTermDebt);
        deferredTaxLiabilitiesSpin->setValue(record.deferredTaxLiabilities);

        commonStockSpin->setValue(record.commonStock);
        retainedEarningsSpin->setValue(record.retainedEarnings);
        additionalPaidInCapitalSpin->setValue(record.additionalPaidInCapital);
    }

    recalculateTotals();
}

void BalanceSheetPage::clearAmountFields() {
    for (auto* spin : amountFields()) {
        const QSignalBlocker blocker(spin);
        spin->setValue(0.0);
    }
    recalculateTotals();
}

void BalanceSheetPage::recalculateTotals() {
    const core::BalanceSheetRecord record = collectFormValues();

    totalCurrentAssetsLabel->setText(formatAmount(record.totalCurrentAssets()));
    totalNonCurrentAssetsLabel->setText(formatAmount(record.totalNonCurrentAssets()));
    totalAssetsLabel->setText(formatAmount(record.totalAssets()));

    totalCurrentLiabilitiesLabel->setText(formatAmount(record.totalCurrentLiabilities()));
    totalNonCurrentLiabilitiesLabel->setText(formatAmount(record.totalNonCurrentLiabilities()));
    totalLiabilitiesLabel->setText(formatAmount(record.totalLiabilities()));

    totalEquityLabel->setText(formatAmount(record.totalEquity()));

    if (record.isBalanced()) {
        balanceIndicatorLabel->setText(
            "Balanced - Total Assets equal Total Liabilities + Total Equity"
        );
        balanceIndicatorLabel->setProperty("state", "success");
    } else {
        balanceIndicatorLabel->setText(
            QString("Out of Balance - discrepancy of %1").arg(formatAmount(record.balanceDifference()))
        );
        balanceIndicatorLabel->setProperty("state", "error");
    }

    balanceIndicatorLabel->style()->unpolish(balanceIndicatorLabel);
    balanceIndicatorLabel->style()->polish(balanceIndicatorLabel);
}

void BalanceSheetPage::refreshDirectory() {
    const auto records = balanceSheetService.getAllRecords();

    directoryCountLabel->setText(
        records.empty()
            ? "No fiscal years recorded yet"
            : QString("%1 fiscal year(s) recorded").arg(records.size())
    );

    recordsTable->setRowCount(static_cast<int>(records.size()));
    for (int row = 0; row < static_cast<int>(records.size()); ++row) {
        const auto& record = records[static_cast<std::size_t>(row)];
        const bool balanced = record.isBalanced();

        const QStringList values{
            QString::number(record.fiscalYear),
            formatAmount(record.totalAssets()),
            formatAmount(record.totalLiabilities()),
            formatAmount(record.totalEquity()),
            balanced ? "Balanced" : "Out of Balance"
        };

        for (int column = 0; column < values.size(); ++column) {
            auto* item = new QTableWidgetItem(values.at(column));
            if (column == 0) {
                item->setData(Qt::UserRole, record.fiscalYear);
                item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            } else if (column == 4) {
                item->setForeground(QColor(balanced ? "#22c55e" : "#ef4444"));
                item->setTextAlignment(Qt::AlignCenter);
            } else {
                item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            recordsTable->setItem(row, column, item);
        }
    }
}

void BalanceSheetPage::selectDirectoryYear(int year) {
    for (int row = 0; row < recordsTable->rowCount(); ++row) {
        const auto* item = recordsTable->item(row, 0);
        if (item != nullptr && item->data(Qt::UserRole).toInt() == year) {
            recordsTable->selectRow(row);
            recordsTable->scrollToItem(
                recordsTable->item(row, 0),
                QAbstractItemView::PositionAtCenter
            );
            return;
        }
    }
    recordsTable->clearSelection();
}

void BalanceSheetPage::handleYearChanged(int year) {
    const auto record = balanceSheetService.getRecordByYear(year);
    if (record) {
        applyRecord(*record);
        showStatus(
            QString("Loaded the saved record for fiscal year %1.").arg(year),
            StatusKind::Info
        );
        return;
    }

    clearAmountFields();
    showStatus(
        QString("No saved record for fiscal year %1 yet. Enter the figures and choose Save Record.").arg(year),
        StatusKind::Info
    );
}

void BalanceSheetPage::handleDirectorySelection(int row, int column) {
    Q_UNUSED(column);

    if (row < 0) {
        return;
    }

    const auto* yearItem = recordsTable->item(row, 0);
    if (yearItem == nullptr) {
        return;
    }

    const int year = yearItem->data(Qt::UserRole).toInt();
    const auto record = balanceSheetService.getRecordByYear(year);
    if (!record) {
        return;
    }

    {
        const QSignalBlocker blocker(fiscalYearSpin);
        fiscalYearSpin->setValue(year);
    }
    applyRecord(*record);
    showStatus(QString("Loaded the saved record for fiscal year %1.").arg(year), StatusKind::Info);
}

void BalanceSheetPage::handleSave() {
    const core::BalanceSheetRecord record = collectFormValues();
    const core::BalanceSheetSaveResult result = balanceSheetService.saveRecord(record);

    if (!result.success) {
        showStatus(QString::fromStdString(result.message), StatusKind::Error);
        return;
    }

    refreshDirectory();
    selectDirectoryYear(record.fiscalYear);

    QString message = QString::fromStdString(result.message);
    if (!record.isBalanced()) {
        message += QString(" Out of balance by %1 - review the accounting equation.")
            .arg(formatAmount(record.balanceDifference()));
        showStatus(message, StatusKind::Error);
        return;
    }

    showStatus(message, StatusKind::Success);
}

void BalanceSheetPage::handleClear() {
    clearAmountFields();
    showStatus(
        QString("Form cleared for fiscal year %1. Enter new figures and save.").arg(fiscalYearSpin->value()),
        StatusKind::Info
    );
}

void BalanceSheetPage::handleDelete() {
    const int year = fiscalYearSpin->value();

    if (!balanceSheetService.hasRecord(year)) {
        showStatus(
            QString("No saved record exists for fiscal year %1.").arg(year),
            StatusKind::Error
        );
        return;
    }

    const auto choice = QMessageBox::question(
        this,
        "Delete Fiscal Year",
        QString("Delete the saved balance sheet record for fiscal year %1?").arg(year),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    if (choice != QMessageBox::Yes) {
        return;
    }

    if (balanceSheetService.deleteRecord(year)) {
        refreshDirectory();
        clearAmountFields();
        showStatus(
            QString("Record for fiscal year %1 deleted.").arg(year),
            StatusKind::Info
        );
    }
}

void BalanceSheetPage::showStatus(const QString& message, StatusKind kind) {
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
