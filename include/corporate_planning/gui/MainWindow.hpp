#ifndef CORPORATE_PLANNING_GUI_MAIN_WINDOW_HPP
#define CORPORATE_PLANNING_GUI_MAIN_WINDOW_HPP

#include <QMainWindow>
#include "corporate_planning/core/BalanceSheetService.hpp"
#include "corporate_planning/core/UserService.hpp"

class QLabel;
class QString;
class QStackedWidget;
class QWidget;
class QVBoxLayout;

namespace corporate_planning::gui {

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    core::UserService userService;
    core::BalanceSheetService balanceSheetService;
    QWidget* createAuthenticationPage();
    void addSectionHeader(QVBoxLayout* layout, const QString& title);
    void addNavButton(QVBoxLayout* layout, const QString& text, int pageIndex);
    QWidget* createPage(
        const QString& title,
        const QString& subtitle,
        const QString& description
    );

    QLabel* pageTitleLabel = nullptr;
    QLabel* pageSubtitleLabel = nullptr;
    QStackedWidget* pageStack = nullptr;
};

} // namespace corporate_planning::gui

#endif // CORPORATE_PLANNING_GUI_MAIN_WINDOW_HPP
