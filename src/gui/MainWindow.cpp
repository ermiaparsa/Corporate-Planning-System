#include "corporate_planning/gui/MainWindow.hpp"

#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace corporate_planning::gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      pageTitleLabel(nullptr),
      pageStack(nullptr) {
    setWindowTitle("Corporate Planning System");
    resize(1100, 700);
    setMinimumSize(900, 600);

    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(centralWidget);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto* sidebar = new QFrame(centralWidget);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(230);

    auto* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(18, 25, 18, 25);
    sidebarLayout->setSpacing(10);

    auto* applicationTitle = new QLabel("Corporate\nPlanning", sidebar);
    applicationTitle->setObjectName("applicationTitle");
    applicationTitle->setAlignment(Qt::AlignCenter);

    sidebarLayout->addWidget(applicationTitle);
    sidebarLayout->addSpacing(25);

    auto* contentWidget = new QWidget(centralWidget);
    contentWidget->setObjectName("contentWidget");

    auto* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 25, 30, 30);
    contentLayout->setSpacing(20);

    pageTitleLabel = new QLabel("Dashboard", contentWidget);
    pageTitleLabel->setObjectName("pageTitle");

    pageStack = new QStackedWidget(contentWidget);

    pageStack->addWidget(
        createPage(
            "Dashboard",
            "Overview of the company's planning information."
        )
    );

    pageStack->addWidget(
        createPage(
            "Production Lines",
            "Manage production lines, costs, revenue and efficiency."
        )
    );

    pageStack->addWidget(
        createPage(
            "Personnel",
            "Manage production, distribution and administrative personnel."
        )
    );

    pageStack->addWidget(
        createPage(
            "Financial Ratios",
            "View assets, liabilities, equity and financial ratios."
        )
    );

    pageStack->addWidget(
        createPage(
            "Equations",
            "Create and evaluate planning equations."
        )
    );

    pageStack->addWidget(
        createPage(
            "User Profile",
            "View and edit the current user profile."
        )
    );

    contentLayout->addWidget(pageTitleLabel);
    contentLayout->addWidget(pageStack, 1);

    const auto addNavigationButton =
        [this, sidebarLayout](const QString& text, int pageIndex) {
            auto* button = new QPushButton(text);
            button->setObjectName("navigationButton");
            button->setCursor(Qt::PointingHandCursor);
            button->setMinimumHeight(45);

            connect(
                button,
                &QPushButton::clicked,
                this,
                [this, text, pageIndex]() {
                    pageTitleLabel->setText(text);
                    pageStack->setCurrentIndex(pageIndex);
                }
            );

            sidebarLayout->addWidget(button);
        };

    addNavigationButton("Dashboard", 0);
    addNavigationButton("Production Lines", 1);
    addNavigationButton("Personnel", 2);
    addNavigationButton("Financial Ratios", 3);
    addNavigationButton("Equations", 4);
    addNavigationButton("User Profile", 5);

    sidebarLayout->addStretch();

    auto* exitButton = new QPushButton("Exit", sidebar);
    exitButton->setObjectName("exitButton");
    exitButton->setCursor(Qt::PointingHandCursor);
    exitButton->setMinimumHeight(45);

    connect(
        exitButton,
        &QPushButton::clicked,
        qApp,
        &QApplication::quit
    );

    sidebarLayout->addWidget(exitButton);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentWidget, 1);

    setCentralWidget(centralWidget);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #f4f6f9;
        }

        QWidget#contentWidget {
            background-color: #f4f6f9;
        }

        QFrame#sidebar {
            background-color: #18212f;
        }

        QLabel#applicationTitle {
            color: white;
            font-size: 23px;
            font-weight: bold;
        }

        QLabel#pageTitle {
            color: #18212f;
            font-size: 27px;
            font-weight: bold;
        }

        QPushButton#navigationButton {
            color: #d8dee9;
            background-color: transparent;
            border: none;
            border-radius: 7px;
            padding: 10px 14px;
            text-align: left;
            font-size: 14px;
        }

        QPushButton#navigationButton:hover {
            color: white;
            background-color: #2d3b50;
        }

        QPushButton#navigationButton:pressed {
            background-color: #3b82f6;
        }

        QPushButton#exitButton {
            color: white;
            background-color: #dc3545;
            border: none;
            border-radius: 7px;
            font-size: 14px;
            font-weight: bold;
        }

        QPushButton#exitButton:hover {
            background-color: #bb2d3b;
        }

        QFrame#pageCard {
            background-color: white;
            border: 1px solid #dde2e8;
            border-radius: 12px;
        }

        QLabel#cardTitle {
            color: #1f2937;
            font-size: 22px;
            font-weight: bold;
        }

        QLabel#cardDescription {
            color: #667085;
            font-size: 15px;
        }
    )");
}

QWidget* MainWindow::createPage(
    const QString& title,
    const QString& description
) {
    auto* page = new QWidget();
    auto* pageLayout = new QVBoxLayout(page);

    pageLayout->setContentsMargins(0, 0, 0, 0);

    auto* card = new QFrame(page);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 30, 30, 30);
    cardLayout->setSpacing(15);

    auto* titleLabel = new QLabel(title, card);
    titleLabel->setObjectName("cardTitle");

    auto* descriptionLabel = new QLabel(description, card);
    descriptionLabel->setObjectName("cardDescription");
    descriptionLabel->setWordWrap(true);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(descriptionLabel);
    cardLayout->addStretch();

    pageLayout->addWidget(card);

    return page;
}

} // namespace corporate_planning::gui
