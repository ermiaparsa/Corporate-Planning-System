#include "corporate_planning/gui/UserManagementPage.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace corporate_planning::gui {

using core::UserRegistrationRequest;
using core::UserRegistrationResult;
using core::UserRole;
using core::UserService;

UserManagementPage::UserManagementPage(core::UserService& service, QWidget* parent)
    : QWidget(parent),
      userService(service) {
    setLayoutDirection(Qt::LeftToRight);

    auto* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(18);

    auto* columns = new QHBoxLayout();
    columns->setContentsMargins(0, 0, 0, 0);
    columns->setSpacing(18);
    columns->addWidget(buildFormCard(), 3);
    columns->addWidget(buildDirectoryCard(), 4);

    pageLayout->addLayout(columns, 1);

    refreshDirectory();
}

QWidget* UserManagementPage::buildFormCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(28, 26, 28, 26);
    cardLayout->setSpacing(14);

    auto* titleLabel = new QLabel("Create User Account", card);
    titleLabel->setObjectName("cardTitle");

    auto* subtitleLabel = new QLabel("Credentials & Permission Level", card);
    subtitleLabel->setObjectName("cardSubtitle");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);

    auto* formLayout = new QFormLayout();
    formLayout->setContentsMargins(0, 8, 0, 0);
    formLayout->setHorizontalSpacing(14);
    formLayout->setVerticalSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    fullNameEdit = addFormField(formLayout, "Full Name", "e.g. Sarah Mitchell");
    usernameEdit = addFormField(formLayout, "Username *", "3-32 chars, letters, digits, _ or .");
    emailEdit = addFormField(formLayout, "Email", "e.g. sarah.mitchell@company.com");

    passwordEdit = addFormField(formLayout, "Password *", "Minimum 8 chars, letters and digits");
    passwordEdit->setEchoMode(QLineEdit::Password);

    confirmPasswordEdit = addFormField(formLayout, "Confirm Password *", "Re-enter the password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    roleCombo = new QComboBox(card);
    roleCombo->setObjectName("formCombo");
    roleCombo->setMinimumHeight(38);
    roleCombo->setCursor(Qt::PointingHandCursor);
    for (const auto role : {UserRole::ADMIN, UserRole::ANALYST, UserRole::PLANNER, UserRole::VIEWER}) {
        roleCombo->addItem(QString::fromStdString(UserService::roleToDisplayName(role)), static_cast<int>(role));
    }
    roleCombo->setCurrentIndex(1);

    auto* roleLabel = new QLabel("Role / Permission Level *", card);
    roleLabel->setObjectName("formLabel");
    formLayout->addRow(roleLabel, roleCombo);

    cardLayout->addLayout(formLayout);

    showPasswordCheck = new QCheckBox("Show password characters", card);
    showPasswordCheck->setObjectName("formCheck");
    showPasswordCheck->setCursor(Qt::PointingHandCursor);
    connect(showPasswordCheck, &QCheckBox::toggled, this, [this](bool checked) {
        const auto mode = checked ? QLineEdit::Normal : QLineEdit::Password;
        passwordEdit->setEchoMode(mode);
        confirmPasswordEdit->setEchoMode(mode);
    });
    cardLayout->addWidget(showPasswordCheck);

    statusLabel = new QLabel(card);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setWordWrap(true);
    statusLabel->setVisible(false);
    cardLayout->addWidget(statusLabel);

    auto* buttonRow = new QHBoxLayout();
    buttonRow->setContentsMargins(0, 6, 0, 0);
    buttonRow->setSpacing(12);

    auto* createButton = new QPushButton("Create Account", card);
    createButton->setObjectName("primaryButton");
    createButton->setCursor(Qt::PointingHandCursor);
    createButton->setMinimumHeight(42);
    createButton->setDefault(true);
    connect(createButton, &QPushButton::clicked, this, &UserManagementPage::handleCreateUser);

    auto* resetButton = new QPushButton("Reset", card);
    resetButton->setObjectName("secondaryButton");
    resetButton->setCursor(Qt::PointingHandCursor);
    resetButton->setMinimumHeight(42);
    connect(resetButton, &QPushButton::clicked, this, &UserManagementPage::handleReset);

    buttonRow->addWidget(createButton, 2);
    buttonRow->addWidget(resetButton, 1);
    cardLayout->addLayout(buttonRow);
    cardLayout->addStretch();

    connect(usernameEdit, &QLineEdit::returnPressed, this, &UserManagementPage::handleCreateUser);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &UserManagementPage::handleCreateUser);
    connect(confirmPasswordEdit, &QLineEdit::returnPressed, this, &UserManagementPage::handleCreateUser);

    return card;
}

QWidget* UserManagementPage::buildDirectoryCard() {
    auto* card = new QFrame(this);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(28, 26, 28, 26);
    cardLayout->setSpacing(14);

    auto* titleLabel = new QLabel("Registered Accounts", card);
    titleLabel->setObjectName("cardTitle");

    directoryCountLabel = new QLabel(card);
    directoryCountLabel->setObjectName("cardSubtitle");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(directoryCountLabel);

    userTable = new QTableWidget(0, 5, card);
    userTable->setObjectName("userTable");
    userTable->setHorizontalHeaderLabels({"User ID", "Username", "Full Name", "Role", "Status"});
    userTable->verticalHeader()->setVisible(false);
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userTable->setAlternatingRowColors(false);
    userTable->setShowGrid(false);
    userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userTable->horizontalHeader()->setHighlightSections(false);

    cardLayout->addWidget(userTable, 1);

    return card;
}

QLineEdit* UserManagementPage::addFormField(
    QFormLayout* layout,
    const QString& label,
    const QString& placeholder
) {
    auto* fieldLabel = new QLabel(label);
    fieldLabel->setObjectName("formLabel");

    auto* edit = new QLineEdit();
    edit->setObjectName("formInput");
    edit->setPlaceholderText(placeholder);
    edit->setMinimumHeight(38);
    edit->setClearButtonEnabled(false);

    layout->addRow(fieldLabel, edit);
    return edit;
}

void UserManagementPage::handleCreateUser() {
    UserRegistrationRequest request;
    request.fullName = fullNameEdit->text().trimmed().toStdString();
    request.username = usernameEdit->text().trimmed().toStdString();
    request.email = emailEdit->text().trimmed().toStdString();
    request.password = passwordEdit->text().toStdString();
    request.confirmPassword = confirmPasswordEdit->text().toStdString();
    request.role = static_cast<UserRole>(roleCombo->currentData().toInt());

    const UserRegistrationResult result = userService.registerUser(request);
    showStatus(QString::fromStdString(result.message), !result.success);

    if (!result.success) {
        return;
    }

    fullNameEdit->clear();
    usernameEdit->clear();
    emailEdit->clear();
    passwordEdit->clear();
    confirmPasswordEdit->clear();
    usernameEdit->setFocus();
    refreshDirectory();
}

void UserManagementPage::handleReset() {
    fullNameEdit->clear();
    usernameEdit->clear();
    emailEdit->clear();
    passwordEdit->clear();
    confirmPasswordEdit->clear();
    roleCombo->setCurrentIndex(1);
    showPasswordCheck->setChecked(false);
    statusLabel->setVisible(false);
    statusLabel->clear();
    usernameEdit->setFocus();
}

void UserManagementPage::refreshDirectory() {
    const auto& users = userService.getUsers();

    directoryCountLabel->setText(
        users.empty()
            ? "No accounts provisioned yet"
            : QString("%1 account(s) provisioned").arg(users.size())
    );

    userTable->setRowCount(static_cast<int>(users.size()));
    for (int row = 0; row < static_cast<int>(users.size()); ++row) {
        const auto& user = users[static_cast<std::size_t>(row)];
        const QStringList values{
            QString::fromStdString(user.getUserId()),
            QString::fromStdString(user.getUsername()),
            QString::fromStdString(user.getFullName()),
            QString::fromStdString(UserService::roleToDisplayName(user.getRole())),
            user.getIsActive() ? "Active" : "Disabled"
        };

        for (int column = 0; column < values.size(); ++column) {
            auto* item = new QTableWidgetItem(values.at(column));
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            userTable->setItem(row, column, item);
        }
    }
}

void UserManagementPage::showStatus(const QString& message, bool isError) {
    statusLabel->setText(message);
    statusLabel->setProperty("state", isError ? "error" : "success");
    statusLabel->setVisible(!message.isEmpty());
    statusLabel->style()->unpolish(statusLabel);
    statusLabel->style()->polish(statusLabel);
}

} // namespace corporate_planning::gui
