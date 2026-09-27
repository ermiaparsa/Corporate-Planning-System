#include "corporate_planning/gui/IncomeStatementPage.hpp"

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

IncomeStatementPage::IncomeStatementPage(
    core::IncomeStatementService& service,
    QWidget* parent)
    : QWidget(parent),
      incomeStatementService(service)
{
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
        core::IncomeStatementService::MIN_FISCAL_YEAR,
        today,
        core::IncomeStatementService::MAX_FISCAL_YEAR
    );
    {
        const QSignalBlocker blocker(fiscalYearSpin);
        fiscalYearSpin->setValue(startYear);
    }
    handleYearChanged(startYear);
}

QWidget* IncomeStatementPage::buildEntryCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(26, 24, 26, 24);
    cardLayout->setSpacing(12);

    auto* titleLabel = new QLabel("Income Statement Entry", card);
    titleLabel->setObjectName("cardTitle");

    auto* subtitleLabel = new QLabel("Template 2 - Historical P&L Data", card);
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
        core::IncomeStatementService::MIN_FISCAL_YEAR,
        core::IncomeStatementService::MAX_FISCAL_YEAR
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
        &IncomeStatementPage::handleYearChanged
    );

    auto* revenueSection = addSection(formContainer, formLayout, "Revenue");
    grossRevenueSpin = addAmountField(revenueSection, "Gross Revenue / Sales");

    auto* cogsSection = addSection(formContainer, formLayout, "Cost of Goods Sold");
    cogsSpin = addAmountField(cogsSection, "Cost of Goods Sold (COGS)");

    auto* opexSection = addSection(formContainer, formLayout, "Operating Expenses");
    sgaSpin = addAmountField(opexSection, "Selling, General & Admin (SG&A)");
    rdSpin = addAmountField(opexSection, "Research & Development (R&D)");
    daSpin = addAmountField(opexSection, "Depreciation & Amortization (D&A)");

    auto* interestSection = addSection(formContainer, formLayout, "Interest & Tax");
    interestExpenseSpin = addAmountField(interestSection, "Interest Expense");
    taxRateSpin = addAmountField(interestSection, "Tax Rate (%)");

    auto* summaryPanel = new QFrame(formContainer);
    summaryPanel->setObjectName("summaryPanel");

    auto* summaryLayout = new QVBoxLayout(summaryPanel);
    summaryLayout->setContentsMargins(16, 14, 16, 14);
    summaryLayout->setSpacing(10);

    auto* summaryTitle = new QLabel("Computed Summary", summaryPanel);
    summaryTitle->setObjectName("formSectionHeader");
    summaryLayout->addWidget(summaryTitle);

    auto* summaryForm = new QFormLayout();
    summaryForm->setContentsMargins(0, 0, 0, 0);
    summaryForm->setHorizontalSpacing(14);
    summaryForm->setVerticalSpacing(8);
    summaryForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    summaryForm->setRowWrapPolicy(QFormLayout::WrapLongRows);

    grossProfitLabel = addSummaryRow(summaryForm, "Gross Profit");
    ebitLabel = addSummaryRow(summaryForm, "Operating Income (EBIT)");
    ebtLabel = addSummaryRow(summaryForm, "Earnings Before Tax (EBT)");
    netIncomeLabel = addSummaryRow(summaryForm, "Net Income");
    netIncomeLabel->setObjectName("totalValueStrong");
    netProfitMarginLabel = addSummaryRow(summaryForm, "Net Profit Margin");

    summaryLayout->addLayout(summaryForm);

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
    connect(saveButton, &QPushButton::clicked, this, &IncomeStatementPage::handleSave);

    auto* clearButton = new QPushButton("Clear / Reset", card);
    clearButton->setObjectName("secondaryButton");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setMinimumHeight(42);
    connect(clearButton, &QPushButton::clicked, this, &IncomeStatementPage::handleClear);

    auto* deleteButton = new QPushButton("Delete Year", card);
    deleteButton->setObjectName("dangerButton");
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setMinimumHeight(42);
    connect(deleteButton, &QPushButton::clicked, this, &IncomeStatementPage::handleDelete);

    buttonRow->addWidget(saveButton, 2);
    buttonRow->addWidget(clearButton, 1);
    buttonRow->addWidget(deleteButton, 1);
    cardLayout->addLayout(buttonRow);

    return card;
}

QWidget* IncomeStatementPage::buildDirectoryCard() {
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

    recordsTable = new QTableWidget(0, 5, card);
    recordsTable->setObjectName("balanceSheetTable");
    recordsTable->setHorizontalHeaderLabels(
        {"Year", "Revenue", "Gross Profit", "Net Income", "Margin"}
    );
    recordsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    recordsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    recordsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recordsTable->verticalHeader()->setVisible(false);
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
        &IncomeStatementPage::handleDirectorySelection
    );

    cardLayout->addWidget(recordsTable, 1);

    auto* hintLabel = new QLabel(
        "Select a row to load that fiscal year back into the form.",
        card
    );
    hintLabel->setObjectName("cardDescription");
    hintLabel->setWordWrap(true);
    cardLayout->addWidget(hintLabel);

    return card;
}

QFormLayout* IncomeStatementPage::addSection(
    QWidget* parent,
    QVBoxLayout* container,
    const QString& title)
{
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

QDoubleSpinBox* IncomeStatementPage::addAmountField(
    QFormLayout* layout,
    const QString& label)
{
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

    connect(spin, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &IncomeStatementPage::recalculateSummary);

    layout->addRow(fieldLabel, spin);
    return spin;
}

QLabel* IncomeStatementPage::addSummaryRow(QFormLayout* layout, const QString& label) {
    auto* nameLabel = new QLabel(label);
    nameLabel->setObjectName("summaryName");

    auto* valueLabel = new QLabel("0.00");
    valueLabel->setObjectName("totalValue");
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    layout->addRow(nameLabel, valueLabel);
    return valueLabel;
}

QList<QDoubleSpinBox*> IncomeStatementPage::amountFields() const {
    return {
        grossRevenueSpin,
        cogsSpin,
        sgaSpin,
        rdSpin,
        daSpin,
        interestExpenseSpin,
        taxRateSpin
    };
}

core::IncomeStatementRecord IncomeStatementPage::collectFormValues() const {
    core::IncomeStatementRecord record;
    record.fiscalYear = fiscalYearSpin->value();

    record.grossRevenue = grossRevenueSpin->value();
    record.costOfGoodsSold = cogsSpin->value();
    record.sellingGeneralAndAdministrative = sgaSpin->value();
    record.researchAndDevelopment = rdSpin->value();
    record.depreciationAndAmortization = daSpin->value();
    record.interestExpense = interestExpenseSpin->value();
    record.taxRate = taxRateSpin->value();

    record.compute();
    return record;
}

void IncomeStatementPage::applyRecord(const core::IncomeStatementRecord& record) {
    QSignalBlocker blockGross(grossRevenueSpin);
    QSignalBlocker blockCog (cogsSpin);
    QSignalBlocker blockSga (sgaSpin);
    QSignalBlocker blockRd  (rdSpin);
    QSignalBlocker blockDa  (daSpin);
    QSignalBlocker blockInt (interestExpenseSpin);
    QSignalBlocker blockTax (taxRateSpin);

    grossRevenueSpin->setValue(record.grossRevenue);
    cogsSpin->setValue(record.costOfGoodsSold);
    sgaSpin->setValue(record.sellingGeneralAndAdministrative);
    rdSpin->setValue(record.researchAndDevelopment);
    daSpin->setValue(record.depreciationAndAmortization);
    interestExpenseSpin->setValue(record.interestExpense);
    taxRateSpin->setValue(record.taxRate);

    recalculateSummary();
}

void IncomeStatementPage::clearAmountFields() {
    QSignalBlocker blockGross(grossRevenueSpin);
    QSignalBlocker blockCog (cogsSpin);
    QSignalBlocker blockSga (sgaSpin);
    QSignalBlocker blockRd  (rdSpin);
    QSignalBlocker blockDa  (daSpin);
    QSignalBlocker blockInt (interestExpenseSpin);
    QSignalBlocker blockTax (taxRateSpin);

    grossRevenueSpin->setValue(0.0);
    cogsSpin->setValue(0.0);
    sgaSpin->setValue(0.0);
    rdSpin->setValue(0.0);
    daSpin->setValue(0.0);
    interestExpenseSpin->setValue(0.0);
    taxRateSpin->setValue(0.0);

    recalculateSummary();
}

void IncomeStatementPage::recalculateSummary() {
    core::IncomeStatementRecord temp;
    temp.grossRevenue = grossRevenueSpin->value();
    temp.costOfGoodsSold = cogsSpin->value();
    temp.sellingGeneralAndAdministrative = sgaSpin->value();
    temp.researchAndDevelopment = rdSpin->value();
    temp.depreciationAndAmortization = daSpin->value();
    temp.interestExpense = interestExpenseSpin->value();
    temp.taxRate = taxRateSpin->value();
    temp.compute();

    grossProfitLabel->setText(formatAmount(temp.grossProfit));
    ebitLabel->setText(formatAmount(temp.operatingIncome));
    ebtLabel->setText(formatAmount(temp.earningsBeforeTax));
    netIncomeLabel->setText(formatAmount(temp.netIncome));
    netProfitMarginLabel->setText(
        QString::number(temp.netProfitMargin, 'f', 2) + " %"
    );
}

void IncomeStatementPage::refreshDirectory() {
    const auto records = incomeStatementService.getAllRecords();

    recordsTable->setRowCount(static_cast<int>(records.size()));

    for (int row = 0; row < static_cast<int>(records.size()); ++row) {
        const auto& record = records[row];

        QStringList values;
        values << QString::number(record.fiscalYear);
        values << formatAmount(record.grossRevenue);
        values << formatAmount(record.grossProfit);
        values << formatAmount(record.netIncome);
        values << QString::number(record.netProfitMargin, 'f', 2) + " %";

        for (int column = 0; column < values.size(); ++column) {
            auto* item = new QTableWidgetItem(values.at(column));
            if (column == 0) {
                item->setData(Qt::UserRole, record.fiscalYear);
                item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            } else {
                item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            recordsTable->setItem(row, column, item);
        }
    }

    directoryCountLabel->setText(
        QString::number(records.size()) + " record" +
        (records.size() != 1 ? "s" : "") + " saved"
    );
}

void IncomeStatementPage::selectDirectoryYear(int year) {
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

void IncomeStatementPage::handleYearChanged(int year) {
    const auto record = incomeStatementService.getRecordByYear(year);
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

void IncomeStatementPage::handleDirectorySelection(int row, int column) {
    Q_UNUSED(column);

    if (row < 0) return;

    const auto* yearItem = recordsTable->item(row, 0);
    if (yearItem == nullptr) return;

    const int year = yearItem->data(Qt::UserRole).toInt();
    const auto record = incomeStatementService.getRecordByYear(year);
    if (!record) return;

    {
        const QSignalBlocker blocker(fiscalYearSpin);
        fiscalYearSpin->setValue(year);
    }
    applyRecord(*record);
    showStatus(
        QString("Loaded the saved record for fiscal year %1.").arg(year),
        StatusKind::Info
    );
}

void IncomeStatementPage::handleSave() {
    const core::IncomeStatementRecord record = collectFormValues();
    const core::IncomeStatementSaveResult result =
        incomeStatementService.saveRecord(record);

    if (!result.success) {
        showStatus(QString::fromStdString(result.message), StatusKind::Error);
        return;
    }

    refreshDirectory();
    selectDirectoryYear(record.fiscalYear);
    showStatus(QString::fromStdString(result.message), StatusKind::Success);
}

void IncomeStatementPage::handleClear() {
    clearAmountFields();
    showStatus(
        QString("Form cleared for fiscal year %1. Enter new figures and save.")
            .arg(fiscalYearSpin->value()),
        StatusKind::Info
    );
}

void IncomeStatementPage::handleDelete() {
    const int year = fiscalYearSpin->value();

    if (!incomeStatementService.hasRecord(year)) {
        showStatus(
            QString("No saved record exists for fiscal year %1.").arg(year),
            StatusKind::Error
        );
        return;
    }

    const auto choice = QMessageBox::question(
        this,
        "Delete Fiscal Year",
        QString("Delete the saved income statement record for fiscal year %1?")
            .arg(year),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    if (choice != QMessageBox::Yes) return;

    if (incomeStatementService.deleteRecord(year)) {
        refreshDirectory();
        clearAmountFields();
        showStatus(
            QString("Record for fiscal year %1 deleted.").arg(year),
            StatusKind::Info
        );
    }
}

void IncomeStatementPage::showStatus(const QString& message, StatusKind kind) {
    const char* state = "info";
    if (kind == StatusKind::Success) state = "success";
    else if (kind == StatusKind::Error) state = "error";

    statusLabel->setText(message);
    statusLabel->setProperty("state", state);
    statusLabel->setVisible(!message.isEmpty());
    statusLabel->style()->unpolish(statusLabel);
    statusLabel->style()->polish(statusLabel);
}

} // namespace corporate_planning::gui
