#ifndef CORPORATE_PLANNING_GUI_USER_MANAGEMENT_PAGE_HPP
#define CORPORATE_PLANNING_GUI_USER_MANAGEMENT_PAGE_HPP

#include "corporate_planning/core/UserService.hpp"

#include <QWidget>

class QCheckBox;
class QComboBox;
class QFormLayout;
class QLabel;
class QLineEdit;
class QTableWidget;

namespace corporate_planning::gui {

// Template 11: account provisioning form plus a live list of registered users.
class UserManagementPage final : public QWidget {

public:
    explicit UserManagementPage(core::UserService& service, QWidget* parent = nullptr);

private:
    QWidget* buildFormCard();
    QWidget* buildDirectoryCard();
    QLineEdit* addFormField(QFormLayout* layout, const QString& label, const QString& placeholder);

    void handleCreateUser();
    void handleReset();
    void refreshDirectory();
    void showStatus(const QString& message, bool isError);

    core::UserService& userService;

    QLineEdit* fullNameEdit = nullptr;
    QLineEdit* usernameEdit = nullptr;
    QLineEdit* emailEdit = nullptr;
    QLineEdit* passwordEdit = nullptr;
    QLineEdit* confirmPasswordEdit = nullptr;
    QComboBox* roleCombo = nullptr;
    QCheckBox* showPasswordCheck = nullptr;
    QLabel* statusLabel = nullptr;
    QLabel* directoryCountLabel = nullptr;
    QTableWidget* userTable = nullptr;
};

} // namespace corporate_planning::gui

#endif // CORPORATE_PLANNING_GUI_USER_MANAGEMENT_PAGE_HPP
