#ifndef CORPORATE_PLANNING_GUI_MAIN_WINDOW_HPP
#define CORPORATE_PLANNING_GUI_MAIN_WINDOW_HPP

#include <QMainWindow>

class QLabel;
class QString;
class QStackedWidget;
class QWidget;

namespace corporate_planning::gui {

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWidget* createPage(
        const QString& title,
        const QString& description
    );

    QLabel* pageTitleLabel;
    QStackedWidget* pageStack;
};

} // namespace corporate_planning::gui

#endif // CORPORATE_PLANNING_GUI_MAIN_WINDOW_HPP
