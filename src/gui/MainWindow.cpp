#include "corporate_planning/gui/MainWindow.hpp"
#include "corporate_planning/gui/UserManagementPage.hpp"

#include <QApplication>
#include <QFont>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
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
    resize(1200, 800);
    setMinimumSize(1000, 680);

    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(centralWidget);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto* sidebar = new QFrame(centralWidget);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(290);

    auto* sidebarOuterLayout = new QVBoxLayout(sidebar);
    sidebarOuterLayout->setContentsMargins(0, 0, 0, 0);
    sidebarOuterLayout->setSpacing(0);

    auto* appHeader = new QWidget(sidebar);
    appHeader->setObjectName("appHeader");
    auto* appHeaderLayout = new QVBoxLayout(appHeader);
    appHeaderLayout->setContentsMargins(20, 24, 20, 16);

    auto* applicationTitle = new QLabel("Corporate\nPlanning", sidebar);
    applicationTitle->setObjectName("applicationTitle");
    applicationTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    auto* applicationSubtitle = new QLabel("Corporate Planning System", appHeader);
    applicationSubtitle->setObjectName("applicationSubtitle");
    applicationSubtitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    appHeaderLayout->addWidget(applicationTitle);
    appHeaderLayout->addWidget(applicationSubtitle);
    sidebarOuterLayout->addWidget(appHeader);

    // Scroll Area for Navigation
    auto* scrollArea = new QScrollArea(sidebar);
    scrollArea->setObjectName("sidebarScroll");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* navContainer = new QWidget();
    navContainer->setObjectName("navContainer");
    auto* navLayout = new QVBoxLayout(navContainer);
    navLayout->setContentsMargins(12, 10, 12, 15);
    navLayout->setSpacing(4);

    // ==========================================
    // Navigation Modules (12 Templates + Dashboard)
    // ==========================================
    
    // Overview
    addNavButton(navLayout, "📊  Dashboard Overview", 0);

    // 1. Base Data Entry
    addSectionHeader(navLayout, "BASE DATA ENTRY");
    addNavButton(navLayout, "📋  Balance Sheet", 1);
    addNavButton(navLayout, "💰  Income Statement", 2);
    addNavButton(navLayout, "🏭  Production Lines", 3);
    addNavButton(navLayout, "👥  Personnel & Exchange", 4);

    // 2. Financial Analysis
    addSectionHeader(navLayout, "FINANCIAL ANALYSIS");
    addNavButton(navLayout, "📈  Financial Ratios", 5);

    // 3. Modeling & Projections
    addSectionHeader(navLayout, "MODELING & FORECASTING");
    addNavButton(navLayout, "🌐  Exogenous Variables", 6);
    addNavButton(navLayout, "📐  Equations & Formulas", 7);
    addNavButton(navLayout, "⚡  Run Forecast Engine", 8);

    // 4. Reporting & History
    addSectionHeader(navLayout, "REPORTS & ARCHIVES");
    addNavButton(navLayout, "📜  Historical Data", 9);
    addNavButton(navLayout, "🔮  Forecasted Projections", 10);

    // 5. System & Security
    addSectionHeader(navLayout, "SYSTEM & SECURITY");
    addNavButton(navLayout, "👤  User Management", 11);
    addNavButton(navLayout, "🔐  Authentication / Login", 12);

    navLayout->addStretch();
    scrollArea->setWidget(navContainer);
    sidebarOuterLayout->addWidget(scrollArea, 1);

    // Exit Button Footer
    auto* footerWidget = new QWidget(sidebar);
    footerWidget->setObjectName("sidebarFooter");
    auto* footerLayout = new QVBoxLayout(footerWidget);
    footerLayout->setContentsMargins(16, 12, 16, 16);

    auto* exitButton = new QPushButton("Exit System", footerWidget);
    exitButton->setObjectName("exitButton");
    exitButton->setCursor(Qt::PointingHandCursor);
    exitButton->setMinimumHeight(40);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    footerLayout->addWidget(exitButton);
    sidebarOuterLayout->addWidget(footerWidget);

    auto* contentWidget = new QWidget(centralWidget);
    contentWidget->setObjectName("contentWidget");

    auto* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 28, 32, 32);
    contentLayout->setSpacing(16);

    auto* headerBox = new QWidget(contentWidget);
    auto* headerLayout = new QVBoxLayout(headerBox);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(4);

    pageTitleLabel = new QLabel("Dashboard Overview", headerBox);
    pageTitleLabel->setObjectName("pageTitle");

    pageSubtitleLabel = new QLabel("Enterprise Performance & Corporate Planning Overview", headerBox);
    pageSubtitleLabel->setObjectName("pageSubtitle");

    headerLayout->addWidget(pageTitleLabel);
    headerLayout->addWidget(pageSubtitleLabel);
    contentLayout->addWidget(headerBox);

    pageStack = new QStackedWidget(contentWidget);

     // Registering 12 Template Pages + Dashboard
    pageStack->addWidget(createPage("Dashboard Overview", "System Status & Summary", "Comprehensive snapshot of core enterprise indicators, operational outputs, and forecasting status."));
    pageStack->addWidget(createPage("Template 1: Balance Sheet Entry", "Current & Non-Current Assets / Liabilities", "Input and manage current assets, fixed assets, short/long-term liabilities, and equity structure."));
    pageStack->addWidget(createPage("Template 2: Income Statement Entry", "Revenues, Costs & Profitability", "Log operational revenues, cost of goods sold (COGS), operating expenses, taxes, and net profits."));
    pageStack->addWidget(createPage("Template 3: Production Lines Configuration", "Capacity & Unit Economics", "Define nominal vs. actual capacities, per-unit manufacturing costs, product lines, and operational efficiencies."));
    pageStack->addWidget(createPage("Template 4: Personnel, FX & Macro Auxiliaries", "Workforce & Macro Indicators", "Maintain headcount, wage structures, inflation rates, and foreign exchange (FX) market parameters."));
    pageStack->addWidget(createPage("Template 5: Financial Ratios Generation", "Ratio Analysis & Benchmark Indicators", "Automated computation and trend monitoring of Liquidity, Solvency, Turnover, and Profitability (ROA, ROE, ROS)."));
    pageStack->addWidget(createPage("Template 6: Exogenous Variables Definition", "External Economic Drivers", "Configure macro external parameters that are outside firm control (e.g., market growth, commodity prices, tariffs)."));
    pageStack->addWidget(createPage("Template 7: Equations & Econometric Setup", "Structural Model & Production Functions", "Formulate econometric relations, Cobb-Douglas production functions, cost curves, and regression parameters."));
    pageStack->addWidget(createPage("Template 8: Forecast Execution Engine", "Multi-Year Dynamic Simulation", "Run dynamic forward-looking simulations, solve system equations, and compute multi-scenario projections."));
    pageStack->addWidget(createPage("Template 9: Historical Data & Trend Reports", "Past Financial Performance Archive", "Review time-series historical records, longitudinal performance data, and multi-period financial tables."));
    pageStack->addWidget(createPage("Template 10: Forecasted Projections Display", "Future Projected Statements", "Visualize generated future balance sheets, projected income statements, cash flow estimates, and scenario bands."));
    pageStack->addWidget(new UserManagementPage(userService, pageStack));
    pageStack->addWidget(createAuthenticationPage());

    contentLayout->addWidget(pageStack, 1);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentWidget, 1);
    setCentralWidget(centralWidget);

     // ==========================================
    // Dark Modern UI Styling (QSS)
    // ==========================================
    setStyleSheet(R"(
        QMainWindow {
            background-color: #0f172a;
        }

        QWidget#contentWidget {
            background-color: #0f172a;
        }

        QFrame#sidebar {
            background-color: #1e293b;
            border-right: 1px solid #334155;
        }

        QWidget#appHeader {
            background-color: #1e293b;
            border-bottom: 1px solid #334155;
        }

        QWidget#navContainer {
            background-color: #1e293b;
        }

        QScrollArea#sidebarScroll {
            background-color: transparent;
            border: none;
        }

        QScrollBar:vertical {
            background: transparent;
            width: 6px;
            margin: 0;
        }

        QScrollBar::handle:vertical {
            background: #475569;
            min-height: 20px;
            border-radius: 3px;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QLabel#applicationTitle {
            color: #38bdf8;
            font-size: 19px;
            font-weight: bold;
        }

        QLabel#applicationSubtitle {
            color: #94a3b8;
            font-size: 11px;
            font-weight: 500;
        }

        QLabel#sectionHeader {
            color: #64748b;
            font-size: 10px;
            font-weight: bold;
            letter-spacing: 0.8px;
            padding: 14px 10px 4px 10px;
        }

        QLabel#pageTitle {
            color: #f8fafc;
            font-size: 24px;
            font-weight: bold;
        }

        QLabel#pageSubtitle {
            color: #94a3b8;
            font-size: 14px;
        }

        QPushButton#navigationButton {
            color: #cbd5e1;
            background-color: transparent;
            border: none;
            border-radius: 8px;
            padding: 9px 12px;
            text-align: left;
            font-size: 13px;
        }

        QPushButton#navigationButton:hover {
            color: #ffffff;
            background-color: #334155;
        }

        QPushButton#navigationButton:pressed {
            background-color: #0284c7;
            color: #ffffff;
        }

        QWidget#sidebarFooter {
            border-top: 1px solid #334155;
        }

        QPushButton#exitButton {
            color: #fca5a5;
            background-color: rgba(239, 68, 68, 0.15);
            border: 1px solid rgba(239, 68, 68, 0.3);
            border-radius: 8px;
            font-size: 13px;
            font-weight: bold;
        }

        QPushButton#exitButton:hover {
            background-color: #dc2626;
            color: white;
        }

        QFrame#pageCard {
            background-color: #1e293b;
            border: 1px solid #334155;
            border-radius: 14px;
        }

        QLabel#cardTitle {
            color: #f8fafc;
            font-size: 20px;
            font-weight: bold;
        }

        QLabel#cardSubtitle {
            color: #38bdf8;
            font-size: 13px;
            font-weight: 500;
        }

        QLabel#cardDescription {
            color: #94a3b8;
            font-size: 14px;
            line-height: 1.5;
        }
        QLabel#formLabel, QCheckBox#formCheck { color: #cbd5e1; }
        QLineEdit#formInput, QComboBox#formCombo, QTableWidget#userTable {
            color: #f8fafc; background-color: #1e293b;
            border: 1px solid #475569; border-radius: 6px; padding: 6px;
            selection-background-color: #0369a1;
        }
        QHeaderView::section { color: #cbd5e1; background: #334155; padding: 6px; }
        QPushButton#primaryButton, QPushButton#secondaryButton {
            color: #f8fafc; background: #0369a1; border: none;
            border-radius: 6px; padding: 10px;
        }
        QPushButton#secondaryButton { background: #334155; }
        QLabel#statusLabel { color: #cbd5e1; }
        QLabel#statusLabel[state="error"] { color: #fca5a5; }
        QLabel#statusLabel[state="success"] { color: #86efac; }
    )");
}

void MainWindow::addSectionHeader(QVBoxLayout* layout, const QString& title) {
    auto* header = new QLabel(title);
    header->setObjectName("sectionHeader");
    header->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(header);
}

void MainWindow::addNavButton(QVBoxLayout* layout, const QString& text, int pageIndex) {
    auto* button = new QPushButton(text);
    button->setObjectName("navigationButton");
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(38);

    connect(button, &QPushButton::clicked, this, [this, text, pageIndex]() {
        pageTitleLabel->setText(text);
        pageStack->setCurrentIndex(pageIndex);
        pageSubtitleLabel->setText(pageIndex == 11 ? "Create accounts and manage user roles"
            : pageIndex == 12 ? "Sign in with a registered account"
            : "Corporate Planning System");
    });

    layout->addWidget(button);
}

QWidget* MainWindow::createAuthenticationPage() {
    auto* page = new QWidget(pageStack);
    auto* layout = new QVBoxLayout(page);
    auto* card = new QFrame(page);
    card->setObjectName("pageCard");
    auto* form = new QVBoxLayout(card);
    form->setContentsMargins(28, 26, 28, 26);
    form->setSpacing(14);
    auto* title = new QLabel("Authentication / Login", card);
    title->setObjectName("cardTitle");
    form->addWidget(title);
    auto* hint = new QLabel("Create an account in User Management first. Accounts are available for this app session.", card);
    hint->setObjectName("cardDescription");
    hint->setWordWrap(true);
    form->addWidget(hint);
    auto* username = new QLineEdit(card);
    username->setObjectName("formInput");
    username->setPlaceholderText("Username");
    username->setAccessibleName("Username");
    auto* password = new QLineEdit(card);
    password->setObjectName("formInput");
    password->setPlaceholderText("Password");
    password->setAccessibleName("Password");
    password->setEchoMode(QLineEdit::Password);
    form->addWidget(username);
    form->addWidget(password);
    auto* showPassword = new QCheckBox("Show password", card);
    showPassword->setObjectName("formCheck");
    form->addWidget(showPassword);
    connect(showPassword, &QCheckBox::toggled, password, [password](bool checked) {
        password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
    auto* status = new QLabel("Not signed in", card);
    status->setObjectName("statusLabel");
    status->setWordWrap(true);
    form->addWidget(status);
    auto* login = new QPushButton("Sign In", card);
    login->setObjectName("primaryButton");
    auto* logout = new QPushButton("Sign Out", card);
    logout->setObjectName("secondaryButton");
    logout->hide();
    form->addWidget(login);
    form->addWidget(logout);
    const auto signIn = [this, username, password, showPassword, status, login, logout]() {
        const bool success = userService.login(username->text().toStdString(), password->text().toStdString());
        password->clear();
        showPassword->setChecked(false);
        if (!success) {
            status->setText("Invalid username or password.");
            password->setFocus();
            return;
        }
        const auto* user = userService.getCurrentUser();
        status->setText(QString("Signed in as %1 (%2)")
            .arg(QString::fromStdString(user->getUsername()),
                 QString::fromStdString(core::UserService::roleToDisplayName(user->getRole()))));
        username->setEnabled(false);
        password->setEnabled(false);
        login->hide();
        logout->show();
    };
    connect(login, &QPushButton::clicked, this, signIn);
    connect(username, &QLineEdit::returnPressed, this, signIn);
    connect(password, &QLineEdit::returnPressed, this, signIn);
    connect(logout, &QPushButton::clicked, this, [this, username, password, status, login, logout]() {
        userService.logout();
        username->clear();
        password->clear();
        username->setEnabled(true);
        password->setEnabled(true);
        status->setText("Signed out");
        logout->hide();
        login->show();
        username->setFocus();
    });
    layout->addWidget(card);
    layout->addStretch();
    return page;
}

QWidget* MainWindow::createPage(
    const QString& title,
    const QString& subtitle,
    const QString& description
) {
    auto* page = new QWidget();
    auto* pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    auto* card = new QFrame(page);
    card->setObjectName("pageCard");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(35, 35, 35, 35);
    cardLayout->setSpacing(12);

    auto* titleLabel = new QLabel(title, card);
    titleLabel->setObjectName("cardTitle");

    auto* subtitleLabel = new QLabel(subtitle, card);
    subtitleLabel->setObjectName("cardSubtitle");

    auto* descriptionLabel = new QLabel(description, card);
    descriptionLabel->setObjectName("cardDescription");
    descriptionLabel->setWordWrap(true);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addWidget(descriptionLabel);
    cardLayout->addStretch();

    pageLayout->addWidget(card);

    return page;
}

} // namespace corporate_planning::gui
