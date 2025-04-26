// ui/mainwindow/mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "managers/meeting/meeting.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbConnected(dbConnected),
    clientManager(new ClientManager(dbConnected, this)),
    trainingManager(new TrainingManager(dbConnected, this)),
    meetingManager(new MeetingManager(dbConnected, this)),
    notificationManager(new NotificationManager(this)),
    networkManager(new QNetworkAccessManager(this)),
    chartWindow(nullptr) // Initialize to nullptr first
{
    qDebug() << "Entering MainWindow constructor";

    // Block signals during UI setup
    this->blockSignals(true);

    ui->setupUi(this);
    applyLightTheme();
    setAttribute(Qt::WA_DeleteOnClose);

    // Initialize chart widgets with layouts
    if (!ui->clientSectorChartWidget->layout()) {
        ui->clientSectorChartWidget->setLayout(new QVBoxLayout(ui->clientSectorChartWidget));
    }
    if (!ui->clientTimeChartWidget->layout()) {
        ui->clientTimeChartWidget->setLayout(new QVBoxLayout(ui->clientTimeChartWidget));
    }
    if (!ui->clientConsultantChartWidget->layout()) {
        ui->clientConsultantChartWidget->setLayout(new QVBoxLayout(ui->clientConsultantChartWidget));
    }
    if (!ui->trainingTypeChartWidget->layout()) {
        ui->trainingTypeChartWidget->setLayout(new QVBoxLayout(ui->trainingTypeChartWidget));
    }
    if (!ui->trainingPriceChartWidget->layout()) {
        ui->trainingPriceChartWidget->setLayout(new QVBoxLayout(ui->trainingPriceChartWidget));
    }
    if (!ui->trainingTrainerChartWidget->layout()) {
        ui->trainingTrainerChartWidget->setLayout(new QVBoxLayout(ui->trainingTrainerChartWidget));
    }
    if (!ui->meetingAgendaChartWidget->layout()) {
        ui->meetingAgendaChartWidget->setLayout(new QVBoxLayout(ui->meetingAgendaChartWidget));
    }
    if (!ui->meetingMonthChartWidget->layout()) {
        ui->meetingMonthChartWidget->setLayout(new QVBoxLayout(ui->meetingMonthChartWidget));
    }
    if (!ui->meetingOrganizerChartWidget->layout()) {
        ui->meetingOrganizerChartWidget->setLayout(new QVBoxLayout(ui->meetingOrganizerChartWidget));
    }

    clientManager->setNotificationManager(notificationManager);
    trainingManager->setNotificationManager(notificationManager);
    meetingManager->setNotificationManager(notificationManager);

    setupUiConnections();

    // Connect networkManager and AI chat signals after UI setup
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAIResponseReceived);
    connect(ui->trainingNotificationLabel, &QPushButton::clicked, this, &MainWindow::handleNotificationLabelClicked);

    // Move appendChatMessage after all setup
    appendChatMessage("Hello! I'm your Meeting Assistant. How can I help you today?", true);

    if (!m_dbConnected) {
        ui->clientSectionButton->setEnabled(false);
        ui->trainingSectionButton->setEnabled(false);
        ui->meetingSectionButton->setEnabled(false);
        ui->statisticsButton->setEnabled(false);
        statusBar()->showMessage("Database not connected. Some features are disabled.");
    } else {
        clientManager->initialize(ui);
        trainingManager->initialize(ui);
        meetingManager->initialize(ui);
        on_meetingSectionButton_clicked();
    }

    // Re-enable signals after all setup
    this->blockSignals(false);

    qDebug() << "Exiting MainWindow constructor";
}

MainWindow::~MainWindow()
{
    delete clientManager;
    delete trainingManager;
    delete meetingManager;
    delete notificationManager;
    delete networkManager;
    delete chartWindow;
    delete ui;
}

void MainWindow::setupUiConnections()
{
    qDebug() << "Setting up UI connections";
    connect(ui->clientSectionButton, &QPushButton::clicked, this, &MainWindow::on_clientSectionButton_clicked);
    connect(ui->trainingSectionButton, &QPushButton::clicked, this, &MainWindow::on_trainingSectionButton_clicked);
    connect(ui->meetingSectionButton, &QPushButton::clicked, this, &MainWindow::on_meetingSectionButton_clicked);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::on_statisticsButton_clicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    
    // Stats tab change connections for embedded charts
    connect(ui->clientTabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == ui->clientTabWidget->indexOf(ui->clientStatsTab)) {
            updateEmbeddedCharts();
        }
    });
    
    connect(ui->trainingTabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == ui->trainingTabWidget->indexOf(ui->trainingStatsTab)) {
            updateEmbeddedCharts();
        }
    });
    
    connect(ui->meetingTabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == ui->meetingTabWidget->indexOf(ui->meetingStatsTab)) {
            updateEmbeddedCharts();
        }
    });
    
    // Fixed direct connections for chat buttons to avoid "no matching signal" errors
    connect(ui->meetingChatSendButton, &QPushButton::clicked, this, &MainWindow::on_chatSendButton_clicked);
    connect(ui->meetingChatClearButton, &QPushButton::clicked, this, &MainWindow::on_chatClearButton_clicked);
    
    qDebug() << "All UI connections established successfully";
}

void MainWindow::on_clientSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->clientPage);
    clientManager->refresh();
    // Update charts if we're on the stats tab
    if (ui->clientTabWidget->currentWidget() == ui->clientStatsTab) {
        updateEmbeddedCharts();
    }
}

void MainWindow::on_trainingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->trainingPage);
    trainingManager->refresh();
    // Update charts if we're on the stats tab
    if (ui->trainingTabWidget->currentWidget() == ui->trainingStatsTab) {
        updateEmbeddedCharts();
    }
}

void MainWindow::on_meetingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->meetingPage);
    meetingManager->refreshTableWidget();
    // Update charts if we're on the stats tab
    if (ui->meetingTabWidget->currentWidget() == ui->meetingStatsTab) {
        updateEmbeddedCharts();
    }
}

void MainWindow::on_statisticsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    
    try {
        // Create chart window if it doesn't exist yet or if it was closed/deleted
        if (!chartWindow || !chartWindow->isVisible()) {
            // If we had a previous chart window that was closed but not nullified
            if (chartWindow) {
                delete chartWindow;
                chartWindow = nullptr;
            }
            
            // Create a new chart window and track its close event
            chartWindow = new ChartWindow(this);
            
            // Connect the destroyed signal to reset our pointer
            connect(chartWindow, &QObject::destroyed, this, [this]() {
                qDebug() << "ChartWindow destroyed, resetting pointer";
                chartWindow = nullptr;
            });
        }
        
        // Ensure chart window stays on top and is activated
        chartWindow->setWindowFlags(chartWindow->windowFlags() | Qt::WindowStaysOnTopHint);
        chartWindow->show();
        chartWindow->raise();
        chartWindow->activateWindow();
        
        // Find the combo boxes with error handling
        QComboBox* statsComboBox = chartWindow->findChild<QComboBox*>("statsTypeComboBox");
        QComboBox* chartTypeComboBox = chartWindow->findChild<QComboBox*>("chartTypeComboBox");
        QComboBox* detailComboBox = chartWindow->findChild<QComboBox*>("chartDetailComboBox");
        
        if (!statsComboBox || !chartTypeComboBox || !detailComboBox) {
            qDebug() << "Warning: One or more combo boxes not found in ChartWindow";
            return;
        }
        
        // Block signals during configuration
        statsComboBox->blockSignals(true);
        chartTypeComboBox->blockSignals(true);
        detailComboBox->blockSignals(true);
        
        // Set to Meeting Statistics by default which tends to be more reliable
        int meetingStatsIndex = statsComboBox->findText("Meeting Statistics");
        if (meetingStatsIndex >= 0) {
            statsComboBox->setCurrentIndex(meetingStatsIndex);
        }
        
        // Set to Bar Chart which is generally more stable than pie charts
        int barChartIndex = chartTypeComboBox->findText("Bar Chart");
        if (barChartIndex >= 0) {
            chartTypeComboBox->setCurrentIndex(barChartIndex);
        }
        
        // Re-enable signals
        statsComboBox->blockSignals(false);
        chartTypeComboBox->blockSignals(false);
        detailComboBox->blockSignals(false);
        
        // Use reset button to safely refresh with our new settings
        QPushButton* resetButton = chartWindow->findChild<QPushButton*>("resetButton");
        if (resetButton) {
            resetButton->click();
        } else {
            QPushButton* refreshButton = chartWindow->findChild<QPushButton*>("refreshButton");
            if (refreshButton) {
                refreshButton->click();
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in on_statisticsButton_clicked: " << e.what();
        QMessageBox::critical(this, "Chart Error", "An error occurred opening statistics: " + QString(e.what()));
        
        // Clean up in case of exception
        if (chartWindow) {
            delete chartWindow;
            chartWindow = nullptr;
        }
    } catch (...) {
        qDebug() << "Unknown exception in on_statisticsButton_clicked";
        QMessageBox::critical(this, "Chart Error", "An unknown error occurred opening statistics");
        
        // Clean up in case of exception
        if (chartWindow) {
            delete chartWindow;
            chartWindow = nullptr;
        }
    }
}

void MainWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::toggleTheme()
{
    if (ui->themeButton->text() == "Dark Theme") {
        applyDarkTheme();
        ui->themeButton->setText("Light Theme");
    } else {
        applyLightTheme();
        ui->themeButton->setText("Dark Theme");
    }
    
    // Update the charts to match the new theme
    updateEmbeddedCharts();
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #A1B8E6); color: #333333; font-family: 'Segoe UI', Arial, sans-serif; }
        QPushButton { background-color: #3A5DAE; color: white; border: 1px solid #2A4682; border-radius: 5px; padding: 6px; font-weight: bold; }
        QPushButton:hover { background-color: #4A70C2; }
        QPushButton:pressed { background-color: #2A4682; }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox, QDoubleSpinBox { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; padding: 4px; color: #333333; }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #3A5DAE; }
        QTableView, QTableWidget { background-color: #FFFFFF; border: 1px solid #D3DCE6; border-radius: 4px; selection-background-color: #A1B8E6; selection-color: #333333; }
        QHeaderView::section { background-color: #3A5DAE; color: white; padding: 5px; border: none; }
        QCalendarWidget { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; }
        QCalendarWidget QToolButton { background-color: #3A5DAE; color: white; border-radius: 3px; }
        QTabWidget::pane { border: 1px solid #3A5DAE; border-radius: 4px; }
        QTabBar::tab { background-color: #D3DCE6; color: #333333; padding: 6px; border-top-left-radius: 4px; border-top-right-radius: 4px; }
        QTabBar::tab:selected { background-color: #3A5DAE; color: white; }
        QTextEdit { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; color: #333333; }
        QChartView { background-color: #FFFFFF; border: 1px solid #D3DCE6; border-radius: 4px; }
        QLabel { font-size: 10pt; padding: 2px; }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel,
        #meetingTitleLabel, #meetingOrganiserLabel, #meetingParticipantLabel, #meetingAgendaLabel, #meetingDurationLabel, #meetingDateLabel {
            font-size: 12pt; font-weight: bold; color: #3A5DAE; text-decoration: underline; padding: 2px; qproperty-alignment: AlignRight; }
        #headerLabel { font-size: 18pt; font-weight: bold; color: #3A5DAE; qproperty-alignment: AlignCenter; }
        #trainingNotificationLabel { font-size: 10pt; font-weight: bold; color: #3A5DAE; }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { border: 2px solid #3A5DAE; border-radius: 5px; }
        QFrame#sideMenu { background-color: #E6ECF5; }
    )");
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2D2D30, stop:1 #1E1E1E); 
            color: #E0E0E0; 
            font-family: 'Segoe UI', Arial, sans-serif; 
        }
        QPushButton { 
            background-color: #0E639C; 
            color: white; 
            border: 1px solid #0E639C; 
            border-radius: 5px; 
            padding: 6px; 
            font-weight: bold; 
        }
        QPushButton:hover { 
            background-color: #1177BB; 
        }
        QPushButton:pressed { 
            background-color: #094F7E; 
        }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox, QDoubleSpinBox { 
            background-color: #3E3E42; 
            border: 1px solid #555555; 
            border-radius: 4px; 
            padding: 4px; 
            color: #E0E0E0; 
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { 
            border: 2px solid #007ACC; 
        }
        QTableView, QTableWidget { 
            background-color: #252526; 
            border: 1px solid #3E3E42; 
            border-radius: 4px; 
            selection-background-color: #0E639C; 
            selection-color: #FFFFFF; 
            gridline-color: #3E3E42;
            alternate-background-color: #2D2D30;
        }
        QHeaderView::section { 
            background-color: #3E3E42; 
            color: white; 
            padding: 5px; 
            border: none; 
        }
        QCalendarWidget { 
            background-color: #252526; 
            border: 1px solid #3E3E42; 
            border-radius: 4px; 
        }
        QCalendarWidget QToolButton { 
            background-color: #0E639C; 
            color: white; 
            border-radius: 3px; 
        }
        QTabWidget::pane { 
            border: 1px solid #3E3E42; 
            border-radius: 4px; 
        }
        QTabBar::tab { 
            background-color: #2D2D30; 
            color: #E0E0E0; 
            padding: 6px; 
            border-top-left-radius: 4px; 
            border-top-right-radius: 4px; 
        }
        QTabBar::tab:selected { 
            background-color: #0E639C; 
            color: white; 
        }
        QTextEdit { 
            background-color: #252526; 
            border: 1px solid #3E3E42; 
            border-radius: 4px; 
            color: #E0E0E0; 
            selection-background-color: #264F78;
        }
        QChartView { 
            background-color: #252526; 
            border: 1px solid #3E3E42; 
            border-radius: 4px; 
        }
        QLabel { 
            font-size: 10pt; 
            padding: 2px; 
        }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel,
        #meetingTitleLabel, #meetingOrganiserLabel, #meetingParticipantLabel, #meetingAgendaLabel, #meetingDurationLabel, #meetingDateLabel {
            font-size: 12pt; 
            font-weight: bold; 
            color: #007ACC; 
            text-decoration: underline; 
            padding: 2px; 
            qproperty-alignment: AlignRight; 
        }
        #headerLabel { 
            font-size: 18pt; 
            font-weight: bold; 
            color: #007ACC; 
            qproperty-alignment: AlignCenter; 
        }
        #trainingNotificationLabel { 
            font-size: 10pt; 
            font-weight: bold; 
            color: #007ACC; 
        }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { 
            border: 1px solid #3E3E42; 
            border-radius: 5px; 
            background-color: #2D2D30;
        }
        QFrame#sideMenu { 
            background-color: #252526; 
        }
        QScrollBar:vertical {
            background: #2D2D30;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #3E3E42;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            background: #2D2D30;
            height: 10px;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background: #3E3E42;
            min-width: 20px;
            border-radius: 5px;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
        QMenu {
            background-color: #252526;
            border: 1px solid #3E3E42;
        }
        QMenu::item {
            background-color: transparent;
            padding: 5px 20px;
        }
        QMenu::item:selected {
            background-color: #0E639C;
        }
        QToolTip {
            background-color: #252526;
            color: #E0E0E0;
            border: 1px solid #3E3E42;
        }
    )");
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

void MainWindow::handleNotificationLabelClicked()
{
    const QVector<NotificationManager::Notification>& notifications = notificationManager->getNotifications();
    if (notifications.isEmpty()) {
        QMessageBox::information(this, "Notifications", "No new notifications.");
        return;
    }

    // Create a dialog to display notifications
    QDialog dialog(this);
    dialog.setWindowTitle("Notifications");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Add a scrollable area for notifications
    QScrollArea *scrollArea = new QScrollArea(&dialog);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    for (const NotificationManager::Notification &notif : notifications) {
        QString notificationText = QString("%1\n%2\n%3")
        .arg(notif.title)
            .arg(notif.description)
            .arg(notif.details);
        QLabel *label = new QLabel(notificationText, scrollWidget);
        label->setWordWrap(true);
        label->setStyleSheet("QLabel { border-bottom: 1px solid #d3d3d3; padding: 5px; }");
        scrollLayout->addWidget(label);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    // Add a "Clear Notifications" button
    QPushButton *clearButton = new QPushButton("Clear Notifications", &dialog);
    connect(clearButton, &QPushButton::clicked, this, [this, &dialog]() {
        notificationManager->clearNotifications();
        dialog.accept();
    });
    layout->addWidget(clearButton);

    // Add a "Close" button
    QPushButton *closeButton = new QPushButton("Close", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog.setLayout(layout);
    dialog.resize(400, 300);
    dialog.exec();

    // Update the notification label after the dialog is closed
    updateNotificationLabel(notificationManager->getNotificationCount());
}

void MainWindow::updateNotificationLabel(int count)
{
    ui->trainingNotificationLabel->setText(QString("Notifications: %1").arg(count));
}

void MainWindow::on_chatSendButton_clicked()
{
    QString input = ui->meetingChatInputLineEdit->text().trimmed();
    if (input.isEmpty()) {
        return;
    }
    appendChatMessage(input);
    processUserInput(input);
    ui->meetingChatInputLineEdit->clear();
}

void MainWindow::on_chatClearButton_clicked()
{
    ui->meetingChatInputLineEdit->clear();
    ui->meetingChatTextEdit->clear();
}

void MainWindow::appendChatMessage(const QString &message, bool isBot)
{
    qDebug() << "Entering appendChatMessage, message:" << message << ", isBot:" << isBot;
    if (!ui || !ui->meetingChatTextEdit) {
        qDebug() << "Error: ui or meetingChatTextEdit is null";
        return;
    }

    QString formattedMessage = QString("[%1] %2: %3\n")
                                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss"),
                                        isBot ? "Assistant" : "User", message);
    ui->meetingChatTextEdit->append(formattedMessage);

    if (!isBot) {
        qDebug() << "Network request disabled for testing";
        ui->meetingChatTextEdit->append("[System] Network requests are disabled for testing.");
        // Uncomment and configure when API is ready
        /*
        QUrl url("https://api.example.com/chat");
        if (!url.isValid()) {
            qDebug() << "Error: Invalid URL in appendChatMessage:" << url.toString();
            ui->meetingChatTextEdit->append("[Error] Invalid API URL. Please check the configuration.");
            return;
        }

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["message"] = message;
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        QNetworkReply *reply = networkManager->post(request, data);
        if (!reply) {
            qDebug() << "Error: Failed to create network reply";
            ui->meetingChatTextEdit->append("[Error] Failed to send message to API.");
        } else {
            connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError code) {
                qDebug() << "Network error in appendChatMessage:" << reply->errorString();
                ui->meetingChatTextEdit->append("[Error] Network error: " + reply->errorString());
            });
        }
        */
    }
    qDebug() << "Exiting appendChatMessage";
}

void MainWindow::processUserInput(const QString &input)
{
    static const QRegularExpression addMeetingRegex(R"(add meeting\s+(.+?)\s+by\s+(.+?)\s+with\s+(.+?)\s+about\s+(.+?)\s+for\s+(\d+)\s+min\s+on\s+(.+))", QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression deleteMeetingRegex(R"(delete meeting\s+(\d+))", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch addMatch = addMeetingRegex.match(input);
    QRegularExpressionMatch deleteMatch = deleteMeetingRegex.match(input);

    if (addMatch.hasMatch()) {
        QStringList parts = addMatch.capturedTexts();
        QString errorMessage;
        if (!validateMeetingInput(parts, errorMessage)) {
            appendChatMessage("Error: " + errorMessage, true);
            return;
        }

        meeting m = createMeetingFromInput(input);
        if (m.add()) {
            meetingManager->refreshTableWidget();
            appendChatMessage("Meeting added successfully!", true);
            if (notificationManager) {
                notificationManager->addNotification("Meeting Added", "Meeting: " + m.getTitle(), "Added meeting via chat", -1);
            }
        } else {
            appendChatMessage("Failed to add meeting. Check database connection.", true);
        }
    } else if (deleteMatch.hasMatch()) {
        int id = deleteMatch.captured(1).toInt();
        meeting m;
        if (m.deleteMeeting(id)) {
            meetingManager->refreshTableWidget();
            appendChatMessage("Meeting deleted successfully!", true);
            if (notificationManager) {
                notificationManager->addNotification("Meeting Deleted", "Meeting ID: " + QString::number(id), "Deleted meeting via chat", -1);
            }
        } else {
            appendChatMessage("Failed to delete meeting. Check database connection.", true);
        }
    } else {
        QNetworkRequest request(QUrl("http://api.example.com/chatbot"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["message"] = input;
        QJsonDocument doc(json);
        networkManager->post(request, doc.toJson());
    }
}

meeting MainWindow::createMeetingFromInput(const QString &input)
{
    static const QRegularExpression addMeetingRegex(R"(add meeting\s+(.+?)\s+by\s+(.+?)\s+with\s+(.+?)\s+about\s+(.+?)\s+for\s+(\d+)\s+min\s+on\s+(.+))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = addMeetingRegex.match(input);

    QString title = match.captured(1);
    QString organiser = match.captured(2);
    QString participant = match.captured(3);
    QString agenda = match.captured(4);
    int duration = match.captured(5).toInt();
    QDateTime dateTime = QDateTime::fromString(match.captured(6), "yyyy-MM-dd hh:mm");

    // Use -1 as temporary ID, using the constructor compatible with meeting.h
    return meeting(-1, title, organiser, participant, agenda, duration, dateTime);
}

bool MainWindow::validateMeetingInput(const QStringList &parts, QString &errorMessage)
{
    if (parts.size() != 7) {
        errorMessage = "Invalid input format. Use: add meeting <title> by <organiser> with <participant> about <agenda> for <duration> min on <date>";
        return false;
    }

    bool ok;
    int duration = parts[5].toInt(&ok);
    if (!ok || duration <= 0) {
        errorMessage = "Duration must be a positive number.";
        return false;
    }

    QDateTime dateTime = QDateTime::fromString(parts[6], "yyyy-MM-dd hh:mm");
    if (!dateTime.isValid() || dateTime < QDateTime::currentDateTime()) {
        errorMessage = "Date and time must be valid and in the future.";
        return false;
    }

    return true;
}

void MainWindow::onAIResponseReceived(QNetworkReply *reply)
{
    qDebug() << "Entering onAIResponseReceived";
    if (!reply) {
        qDebug() << "Error: Network reply is null";
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error:" << reply->errorString();
        appendChatMessage("Error: " + reply->errorString(), true);
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    if (doc.isNull()) {
        qDebug() << "Error: Failed to parse JSON response";
        appendChatMessage("Error: Failed to parse API response.", true);
        reply->deleteLater();
        return;
    }

    QJsonObject json = doc.object();
    QString botResponse = json.value("response").toString();
    if (botResponse.isEmpty()) {
        qDebug() << "Error: Empty response from API";
        appendChatMessage("Error: Empty response from API.", true);
    } else {
        appendChatMessage(botResponse, true);
    }

    reply->deleteLater();
    qDebug() << "Exiting onAIResponseReceived";
}

ChartWindow* MainWindow::getChartWindow() const
{
    return chartWindow;
}

void MainWindow::updateEmbeddedCharts()
{
    if (!m_dbConnected) {
        return;
    }
    
    // Update charts based on which tab is currently visible
    if (ui->mainStackedWidget->currentWidget() == ui->clientPage && 
        ui->clientTabWidget->currentWidget() == ui->clientStatsTab) {
        createClientCharts();
    }
    else if (ui->mainStackedWidget->currentWidget() == ui->trainingPage && 
             ui->trainingTabWidget->currentWidget() == ui->trainingStatsTab) {
        createTrainingCharts();
    }
    else if (ui->mainStackedWidget->currentWidget() == ui->meetingPage && 
             ui->meetingTabWidget->currentWidget() == ui->meetingStatsTab) {
        createMeetingCharts();
    }
}

void MainWindow::createClientCharts()
{
    // Create and embed client charts
    
    // Sector Chart
    QChart *sectorChart = new QChart();
    sectorChart->setTitle("Client Distribution by Sector");
    sectorChart->setAnimationOptions(QChart::SeriesAnimations);
    
    // Create a pie series for client sectors
    QPieSeries *sectorSeries = new QPieSeries();
    
    // Get client data from database
    QSqlQuery query;
    if (query.exec("SELECT SECTOR, COUNT(*) FROM CLIENTS GROUP BY SECTOR")) {
        while (query.next()) {
            QString sector = query.value(0).toString();
            if (sector.isEmpty() || query.isNull(0)) {
                sector = "Not Specified";
            }
            int count = query.value(1).toInt();
            if (count > 0) {
                sectorSeries->append(sector, count);
            }
        }
        
        sectorChart->addSeries(sectorSeries);
        
        // Configure appearance
        for (QPieSlice *slice : sectorSeries->slices()) {
            slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
            slice->setLabelVisible(true);
        }
    }
    
    // Create chart view and add to widget
    QChartView *sectorChartView = new QChartView(sectorChart);
    sectorChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the client sector chart widget
    QLayout *sectorLayout = ui->clientSectorChartWidget->layout();
    if (!sectorLayout) {
        sectorLayout = new QVBoxLayout(ui->clientSectorChartWidget);
        ui->clientSectorChartWidget->setLayout(sectorLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = sectorLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    sectorLayout->addWidget(sectorChartView);
    
    // Time Chart (client acquisition timeline)
    QChart *timeChart = new QChart();
    timeChart->setTitle("Client Acquisition Timeline");
    timeChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries *timeSeries = new QBarSeries();
    QBarSet *timeSet = new QBarSet("Clients");
    QStringList months;
    
    // Get client data by month
    query.clear();
    if (query.exec("SELECT strftime('%m', CONSULTATION_DATE) as MONTH, COUNT(*) FROM CLIENTS GROUP BY strftime('%m', CONSULTATION_DATE) ORDER BY MONTH")) {
        QStringList monthNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        
        while (query.next()) {
            QString monthNum = query.value(0).toString();
            int count = query.value(1).toInt();
            
            // Convert month number to name
            bool isNumeric;
            int monthIndex = monthNum.toInt(&isNumeric);
            QString monthName = isNumeric && monthIndex >= 1 && monthIndex <= 12 
                ? monthNames[monthIndex - 1] 
                : monthNum;
            
            *timeSet << count;
            months << monthName;
        }
        
        timeSeries->append(timeSet);
        timeChart->addSeries(timeSeries);
        
        // Set up axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(months);
        timeChart->addAxis(axisX, Qt::AlignBottom);
        timeSeries->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (int i = 0; i < timeSet->count(); i++) {
            maxValue = qMax(maxValue, (int)timeSet->at(i));
        }
        axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1);
        axisY->setLabelFormat("%d");
        timeChart->addAxis(axisY, Qt::AlignLeft);
        timeSeries->attachAxis(axisY);
    }
    
    // Create chart view and add to widget
    QChartView *timeChartView = new QChartView(timeChart);
    timeChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the client time chart widget
    QLayout *timeLayout = ui->clientTimeChartWidget->layout();
    if (!timeLayout) {
        timeLayout = new QVBoxLayout(ui->clientTimeChartWidget);
        ui->clientTimeChartWidget->setLayout(timeLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = timeLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    timeLayout->addWidget(timeChartView);
    
    // Consultant Chart
    QChart *consultantChart = new QChart();
    consultantChart->setTitle("Clients by Consultant");
    consultantChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries *consultantSeries = new QBarSeries();
    QBarSet *consultantSet = new QBarSet("Clients");
    QStringList consultants;
    
    // Get client data by consultant
    query.clear();
    if (query.exec("SELECT e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME, COUNT(*) "
                  "FROM CLIENTS c "
                  "LEFT JOIN EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "GROUP BY e.FIRST_NAME, e.LAST_NAME")) {
        
        while (query.next()) {
            QString consultant = query.value(0).toString();
            if (consultant.isEmpty() || query.isNull(0)) {
                consultant = "Not Assigned";
            }
            int count = query.value(1).toInt();
            
            *consultantSet << count;
            consultants << consultant;
        }
        
        consultantSeries->append(consultantSet);
        consultantChart->addSeries(consultantSeries);
        
        // Set up axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(consultants);
        consultantChart->addAxis(axisX, Qt::AlignBottom);
        consultantSeries->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (int i = 0; i < consultantSet->count(); i++) {
            maxValue = qMax(maxValue, (int)consultantSet->at(i));
        }
        axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1);
        axisY->setLabelFormat("%d");
        consultantChart->addAxis(axisY, Qt::AlignLeft);
        consultantSeries->attachAxis(axisY);
    }
    
    // Create chart view and add to widget
    QChartView *consultantChartView = new QChartView(consultantChart);
    consultantChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the client consultant chart widget
    QLayout *consultantLayout = ui->clientConsultantChartWidget->layout();
    if (!consultantLayout) {
        consultantLayout = new QVBoxLayout(ui->clientConsultantChartWidget);
        ui->clientConsultantChartWidget->setLayout(consultantLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = consultantLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    consultantLayout->addWidget(consultantChartView);
}

void MainWindow::createTrainingCharts()
{
    // Create and embed training charts
    
    // Type Chart
    QChart *typeChart = new QChart();
    typeChart->setTitle("Training Distribution by Type");
    typeChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QPieSeries *typeSeries = new QPieSeries();
    
    // Get training data from database
    QSqlQuery query;
    if (query.exec("SELECT FORMATION, COUNT(*) FROM FORMATIONS GROUP BY FORMATION")) {
        while (query.next()) {
            QString type = query.value(0).toString();
            if (type.isEmpty() || query.isNull(0)) {
                type = "Not Specified";
            }
            int count = query.value(1).toInt();
            if (count > 0) {
                typeSeries->append(type, count);
            }
        }
        
        typeChart->addSeries(typeSeries);
        
        // Configure appearance
        for (QPieSlice *slice : typeSeries->slices()) {
            slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
            slice->setLabelVisible(true);
        }
    }
    
    // Create chart view and add to widget
    QChartView *typeChartView = new QChartView(typeChart);
    typeChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the training type chart widget
    QLayout *typeLayout = ui->trainingTypeChartWidget->layout();
    if (!typeLayout) {
        typeLayout = new QVBoxLayout(ui->trainingTypeChartWidget);
        ui->trainingTypeChartWidget->setLayout(typeLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = typeLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    typeLayout->addWidget(typeChartView);
    
    // Price Chart
    QChart *priceChart = new QChart();
    priceChart->setTitle("Price Distribution");
    priceChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries *priceSeries = new QBarSeries();
    QBarSet *priceSet = new QBarSet("Trainings");
    QStringList priceRanges;
    
    // Get training data by price range
    query.clear();
    if (query.exec("SELECT CASE "
                  "WHEN PRIX <= 50 THEN 'Low (≤ 50)' "
                  "WHEN PRIX <= 100 THEN 'Medium (51-100)' "
                  "ELSE 'High (> 100)' END AS PRICE_RANGE, COUNT(*) "
                  "FROM FORMATIONS GROUP BY CASE "
                  "WHEN PRIX <= 50 THEN 'Low (≤ 50)' "
                  "WHEN PRIX <= 100 THEN 'Medium (51-100)' "
                  "ELSE 'High (> 100)' END")) {
        
        while (query.next()) {
            QString range = query.value(0).toString();
            int count = query.value(1).toInt();
            
            *priceSet << count;
            priceRanges << range;
        }
        
        priceSeries->append(priceSet);
        priceChart->addSeries(priceSeries);
        
        // Set up axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(priceRanges);
        priceChart->addAxis(axisX, Qt::AlignBottom);
        priceSeries->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (int i = 0; i < priceSet->count(); i++) {
            maxValue = qMax(maxValue, (int)priceSet->at(i));
        }
        axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1);
        axisY->setLabelFormat("%d");
        priceChart->addAxis(axisY, Qt::AlignLeft);
        priceSeries->attachAxis(axisY);
    }
    
    // Create chart view and add to widget
    QChartView *priceChartView = new QChartView(priceChart);
    priceChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the training price chart widget
    QLayout *priceLayout = ui->trainingPriceChartWidget->layout();
    if (!priceLayout) {
        priceLayout = new QVBoxLayout(ui->trainingPriceChartWidget);
        ui->trainingPriceChartWidget->setLayout(priceLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = priceLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    priceLayout->addWidget(priceChartView);
    
    // Trainer Chart
    QChart *trainerChart = new QChart();
    trainerChart->setTitle("Trainings by Trainer");
    trainerChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries *trainerSeries = new QBarSeries();
    QBarSet *trainerSet = new QBarSet("Trainings");
    QStringList trainers;
    
    // Get training data by trainer
    query.clear();
    if (query.exec("SELECT TRAINER, COUNT(*) FROM FORMATIONS GROUP BY TRAINER")) {
        while (query.next()) {
            QString trainer = query.value(0).toString();
            if (trainer.isEmpty() || query.isNull(0)) {
                trainer = "Not Assigned";
            }
            int count = query.value(1).toInt();
            
            *trainerSet << count;
            trainers << trainer;
        }
        
        trainerSeries->append(trainerSet);
        trainerChart->addSeries(trainerSeries);
        
        // Set up axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(trainers);
        trainerChart->addAxis(axisX, Qt::AlignBottom);
        trainerSeries->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (int i = 0; i < trainerSet->count(); i++) {
            maxValue = qMax(maxValue, (int)trainerSet->at(i));
        }
        axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1);
        axisY->setLabelFormat("%d");
        trainerChart->addAxis(axisY, Qt::AlignLeft);
        trainerSeries->attachAxis(axisY);
    }
    
    // Create chart view and add to widget
    QChartView *trainerChartView = new QChartView(trainerChart);
    trainerChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the training trainer chart widget
    QLayout *trainerLayout = ui->trainingTrainerChartWidget->layout();
    if (!trainerLayout) {
        trainerLayout = new QVBoxLayout(ui->trainingTrainerChartWidget);
        ui->trainingTrainerChartWidget->setLayout(trainerLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = trainerLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    trainerLayout->addWidget(trainerChartView);
}

void MainWindow::createMeetingCharts()
{
    // Create and embed meeting charts
    
    // Agenda Chart
    QChart *agendaChart = new QChart();
    agendaChart->setTitle("Meeting Distribution by Agenda");
    agendaChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QPieSeries *agendaSeries = new QPieSeries();
    
    // Get meeting data from database
    QSqlQuery query;
    if (query.exec("SELECT AGENDA, COUNT(*) FROM MEETING GROUP BY AGENDA")) {
        while (query.next()) {
            QString agenda = query.value(0).toString();
            if (agenda.isEmpty() || query.isNull(0)) {
                agenda = "Not Specified";
            }
            // Truncate long agenda titles
            if (agenda.length() > 20) {
                agenda = agenda.left(17) + "...";
            }
            int count = query.value(1).toInt();
            if (count > 0) {
                agendaSeries->append(agenda, count);
            }
        }
        
        agendaChart->addSeries(agendaSeries);
        
        // Configure appearance
        for (QPieSlice *slice : agendaSeries->slices()) {
            slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
            slice->setLabelVisible(true);
        }
    }
    
    // Create chart view and add to widget
    QChartView *agendaChartView = new QChartView(agendaChart);
    agendaChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the meeting agenda chart widget
    QLayout *agendaLayout = ui->meetingAgendaChartWidget->layout();
    if (!agendaLayout) {
        agendaLayout = new QVBoxLayout(ui->meetingAgendaChartWidget);
        ui->meetingAgendaChartWidget->setLayout(agendaLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = agendaLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    agendaLayout->addWidget(agendaChartView);
    
    // Month Chart
    QChart *monthChart = new QChart();
    monthChart->setTitle("Meeting Distribution by Month");
    monthChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries *monthSeries = new QBarSeries();
    QBarSet *monthSet = new QBarSet("Meetings");
    QStringList months;
    
    // Get meeting data by month
    query.clear();
    if (query.exec("SELECT strftime('%m', DATEM) as MONTH, COUNT(*) FROM MEETING GROUP BY strftime('%m', DATEM) ORDER BY MONTH")) {
        QStringList monthNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        
        while (query.next()) {
            QString monthNum = query.value(0).toString();
            int count = query.value(1).toInt();
            
            // Convert month number to name
            bool isNumeric;
            int monthIndex = monthNum.toInt(&isNumeric);
            QString monthName = isNumeric && monthIndex >= 1 && monthIndex <= 12 
                ? monthNames[monthIndex - 1] 
                : monthNum;
            
            *monthSet << count;
            months << monthName;
        }
        
        monthSeries->append(monthSet);
        monthChart->addSeries(monthSeries);
        
        // Set up axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(months);
        monthChart->addAxis(axisX, Qt::AlignBottom);
        monthSeries->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (int i = 0; i < monthSet->count(); i++) {
            maxValue = qMax(maxValue, (int)monthSet->at(i));
        }
        axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1);
        axisY->setLabelFormat("%d");
        monthChart->addAxis(axisY, Qt::AlignLeft);
        monthSeries->attachAxis(axisY);
    }
    
    // Create chart view and add to widget
    QChartView *monthChartView = new QChartView(monthChart);
    monthChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the meeting month chart widget
    QLayout *monthLayout = ui->meetingMonthChartWidget->layout();
    if (!monthLayout) {
        monthLayout = new QVBoxLayout(ui->meetingMonthChartWidget);
        ui->meetingMonthChartWidget->setLayout(monthLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = monthLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    monthLayout->addWidget(monthChartView);
    
    // Organizer Chart
    QChart *organizerChart = new QChart();
    organizerChart->setTitle("Meetings by Organizer");
    organizerChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries *organizerSeries = new QBarSeries();
    QBarSet *organizerSet = new QBarSet("Meetings");
    QStringList organizers;
    
    // Get meeting data by organizer
    query.clear();
    if (query.exec("SELECT ORGANISER, COUNT(*) FROM MEETING GROUP BY ORGANISER")) {
        while (query.next()) {
            QString organizer = query.value(0).toString();
            if (organizer.isEmpty() || query.isNull(0)) {
                organizer = "Not Assigned";
            }
            int count = query.value(1).toInt();
            
            *organizerSet << count;
            organizers << organizer;
        }
        
        organizerSeries->append(organizerSet);
        organizerChart->addSeries(organizerSeries);
        
        // Set up axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(organizers);
        organizerChart->addAxis(axisX, Qt::AlignBottom);
        organizerSeries->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (int i = 0; i < organizerSet->count(); i++) {
            maxValue = qMax(maxValue, (int)organizerSet->at(i));
        }
        axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1);
        axisY->setLabelFormat("%d");
        organizerChart->addAxis(axisY, Qt::AlignLeft);
        organizerSeries->attachAxis(axisY);
    }
    
    // Create chart view and add to widget
    QChartView *organizerChartView = new QChartView(organizerChart);
    organizerChartView->setRenderHint(QPainter::Antialiasing);
    
    // Find the layout of the meeting organizer chart widget
    QLayout *organizerLayout = ui->meetingOrganizerChartWidget->layout();
    if (!organizerLayout) {
        organizerLayout = new QVBoxLayout(ui->meetingOrganizerChartWidget);
        ui->meetingOrganizerChartWidget->setLayout(organizerLayout);
    } else {
        // Clear any existing widgets in the layout
        QLayoutItem *child;
        while ((child = organizerLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    organizerLayout->addWidget(organizerChartView);
}
