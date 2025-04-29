#include "waitingroomdialog.h"
#include "formationwindow.h"
#include "arduino.h" // Add Arduino header
#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError> // Added for QSqlError
#include <QMessageBox>
#include <QCloseEvent>

WaitingRoomDialog::WaitingRoomDialog(int waitingCount, bool isDarkTheme, QWidget *parent)
    : QDialog(parent), chairLayout(nullptr), chairLabels(), isDarkTheme(isDarkTheme),
    clockLabel(nullptr), clockTimer(new QTimer(this)), isClosing(false), self(this), countLabel(nullptr)
{
    qDebug() << "Constructing WaitingRoomDialog";
    setWindowTitle("Waiting Room Overview");
    setModal(false); // Changed to non-modal to prevent blocking
    setMinimumSize(400, 300);

    setupUI(waitingCount);
    applyStylesheet();

    // Initialize LED state based on initial count
    updateLedState(waitingCount);

    // Update clock every second
    connect(clockTimer, &QTimer::timeout, this, [=]() {
        if (isClosing || !self) {
            qDebug() << "Clock timer ignored due to closing or deleted object";
            return;
        }
        if (clockLabel) {
            qDebug() << "Clock timer ticked, updating clock";
            clockLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
        }
    });
    clockTimer->start(1000);

    // Connect to parent's waitingRoomCountChanged signal
    FormationWindow *formationWindow = qobject_cast<FormationWindow*>(parent);
    if (formationWindow) {
        qDebug() << "Connecting to waitingRoomCountChanged signal";
        connect(formationWindow, &FormationWindow::waitingRoomCountChanged,
                this, &WaitingRoomDialog::onWaitingRoomCountChanged, Qt::UniqueConnection);
    } else {
        qDebug() << "Failed to cast parent to FormationWindow";
    }
}

WaitingRoomDialog::~WaitingRoomDialog()
{
    qDebug() << "Destructing WaitingRoomDialog";
    isClosing = true;
    if (clockTimer) {
        clockTimer->stop();
        clockTimer->blockSignals(true);
        clockTimer->disconnect();
    }
}

void WaitingRoomDialog::setupUI(int waitingCount)
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Title
    QLabel *titleLabel = new QLabel("Waiting Room Chairs", this);
    titleLabel->setObjectName("dialogTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Clock
    clockLabel = new QLabel(QTime::currentTime().toString("hh:mm:ss"), this);
    clockLabel->setObjectName("clockLabel");
    clockLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(clockLabel);

    // Chair grid
    chairLayout = new QGridLayout();
    chairLayout->setSpacing(15);
    chairLayout->setAlignment(Qt::AlignCenter);

    // Create chair labels
    for (int i = 0; i < MAX_CHAIRS; ++i) {
        QLabel *chair = new QLabel(this);
        chair->setFixedSize(80, 80);
        chair->setScaledContents(true);
        chair->setObjectName("chairLabel");
        chair->setToolTip(QString("Chair %1: %2").arg(i + 1).arg(i < waitingCount ? "Occupied" : "Empty"));
        chairLabels.append(chair);
        chairLayout->addWidget(chair, i / 3, i % 3);
    }
    mainLayout->addLayout(chairLayout);

    // Count display
    countLabel = new QLabel(QString("Current Count: %1").arg(waitingCount), this);
    countLabel->setObjectName("countLabel");
    countLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(countLabel);

    // wrr button
    QPushButton *wrrButton = new QPushButton("-", this);
    wrrButton->setObjectName("wrr");
    wrrButton->setFixedSize(100, 50);
    connect(wrrButton, &QPushButton::clicked, this, &WaitingRoomDialog::onWrrButtonClicked);
    mainLayout->addWidget(wrrButton, 0, Qt::AlignCenter);

    // Close button
    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setObjectName("closeButton");
    connect(closeButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "Close button clicked";
        isClosing = true; // Set closing flag
        accept(); // Accept the dialog
    });
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Update chairs
    updateChairs(waitingCount);
}

void WaitingRoomDialog::applyStylesheet()
{
    QString styleSheet = isDarkTheme ?
                             R"(
            QDialog {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 #2C3E50, stop:1 #1A252F);
                color: #ECF0F1;
                font-family: 'Segoe UI', Arial, sans-serif;
                border: 1px solid #34495E;
                border-radius: 8px;
            }
            QLabel#dialogTitle {
                font-size: 22pt;
                font-weight: bold;
                color: #E74C3C;
                padding: 10px;
                margin-bottom: 10px;
                text-align: center;
                border-bottom: 2px solid rgba(231, 76, 60, 0.3);
            }
            QLabel#clockLabel {
                font-size: 16pt;
                color: #ECF0F1;
                background-color: rgba(44, 62, 80, 0.7);
                border: 1px solid #E74C3C;
                border-radius: 8px;
                padding: 8px;
                margin: 5px;
                min-width: 150px;
                text-align: center;
            }
            QLabel#countLabel {
                font-size: 18pt;
                font-weight: bold;
                color: #F39C12;
                padding: 10px;
                margin: 5px;
                text-align: center;
            }
            QLabel#chairLabel {
                border: 2px solid #E74C3C;
                border-radius: 15px;
                background-color: rgba(44, 62, 80, 0.5);
                padding: 5px;
                margin: 8px;
            }
            QLabel#chairLabel:hover {
                border: 2px solid #F39C12;
                background-color: rgba(44, 62, 80, 0.7);
            }
            QPushButton#closeButton {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #3498DB, stop:1 #2980B9);
                color: white;
                border: 1px solid #2574A9;
                border-radius: 8px;
                padding: 10px 20px;
                font-size: 14pt;
                font-weight: bold;
                min-width: 120px;
                margin: 10px;
            }
            QPushButton#closeButton:hover {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #5DADE2, stop:1 #3498DB);
                border: 1px solid #3498DB;
            }
            QPushButton#closeButton:pressed {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #2980B9, stop:1 #2574A9);
                border: 1px solid #1F618D;
                padding-top: 11px;
                padding-bottom: 9px;
            }
            QPushButton#wrr {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #E74C3C, stop:1 #C0392B);
                color: white;
                border: 2px solid #C0392B;
                border-radius: 20px;
                padding: 12px 25px;
                font-size: 18pt;
                font-weight: bold;
                min-width: 100px;
                min-height: 50px;
                margin: 15px;
            }
            QPushButton#wrr:hover {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #EC7063, stop:1 #E74C3C);
                border: 2px solid #E74C3C;
            }
            QPushButton#wrr:pressed {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #C0392B, stop:1 #A93226);
                border: 2px solid #922B21;
                padding-top: 13px;
                padding-bottom: 11px;
            }
            QPushButton#wrr:disabled {
                background-color: #7F8C8D;
                border: 2px solid #707B7C;
                color: #BDC3C7;
            }
            QGridLayout {
                margin: 15px;
                spacing: 12px;
            }
            QVBoxLayout {
                margin: 20px;
                spacing: 15px;
            }
        )" :
                             R"(
            QDialog {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 #ECF0F1, stop:1 #D6EAF8);
                color: #2C3E50;
                font-family: 'Segoe UI', Arial, sans-serif;
                border: 1px solid #AED6F1;
                border-radius: 8px;
            }
            QLabel#dialogTitle {
                font-size: 22pt;
                font-weight: bold;
                color: #2980B9;
                padding: 10px;
                margin-bottom: 10px;
                text-align: center;
                border-bottom: 2px solid rgba(41, 128, 185, 0.3);
            }
            QLabel#clockLabel {
                font-size: 16pt;
                color: #2C3E50;
                background-color: rgba(236, 240, 241, 0.7);
                border: 1px solid #3498DB;
                border-radius: 8px;
                padding: 8px;
                margin: 5px;
                min-width: 150px;
                text-align: center;
            }
            QLabel#countLabel {
                font-size: 18pt;
                font-weight: bold;
                color: #2980B9;
                padding: 10px;
                margin: 5px;
                text-align: center;
            }
            QLabel#chairLabel {
                border: 2px solid #3498DB;
                border-radius: 15px;
                background-color: rgba(236, 240, 241, 0.5);
                padding: 5px;
                margin: 8px;
            }
            QLabel#chairLabel:hover {
                border: 2px solid #2980B9;
                background-color: rgba(236, 240, 241, 0.7);
            }
            QPushButton#closeButton {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #3498DB, stop:1 #2980B9);
                color: white;
                border: 1px solid #2574A9;
                border-radius: 8px;
                padding: 10px 20px;
                font-size: 14pt;
                font-weight: bold;
                min-width: 120px;
                margin: 10px;
            }
            QPushButton#closeButton:hover {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #5DADE2, stop:1 #3498DB);
                border: 1px solid #3498DB;
            }
            QPushButton#closeButton:pressed {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #2980B9, stop:1 #2574A9);
                border: 1px solid #1F618D;
                padding-top: 11px;
                padding-bottom: 9px;
            }
            QPushButton#wrr {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #E74C3C, stop:1 #C0392B);
                color: white;
                border: 2px solid #C0392B;
                border-radius: 20px;
                padding: 12px 25px;
                font-size: 18pt;
                font-weight: bold;
                min-width: 100px;
                min-height: 50px;
                margin: 15px;
            }
            QPushButton#wrr:hover {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #EC7063, stop:1 #E74C3C);
                border: 2px solid #E74C3C;
            }
            QPushButton#wrr:pressed {
                background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                                 stop:0 #C0392B, stop:1 #A93226);
                border: 2px solid #922B21;
                padding-top: 13px;
                padding-bottom: 11px;
            }
            QPushButton#wrr:disabled {
                background-color: #BDC3C7;
                border: 2px solid #A6ACAF;
                color: #7F8C8D;
            }
            QGridLayout {
                margin: 15px;
                spacing: 12px;
            }
            QVBoxLayout {
                margin: 20px;
                spacing: 15px;
            }
        )";
    setStyleSheet(styleSheet);
}

void WaitingRoomDialog::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event triggered for WaitingRoomDialog";
    isClosing = true;

    // Stop and disconnect timer
    clockTimer->stop();
    clockTimer->blockSignals(true);
    clockTimer->disconnect();

    // Block and disconnect all signals to this dialog
    blockSignals(true);
    disconnect();

    // Disconnect from parent's signal
    FormationWindow *formationWindow = qobject_cast<FormationWindow*>(parent());
    if (formationWindow) {
        qDebug() << "Disconnecting waitingRoomCountChanged signal";
        disconnect(formationWindow, &FormationWindow::waitingRoomCountChanged,
                   this, &WaitingRoomDialog::onWaitingRoomCountChanged);
    }

    QDialog::closeEvent(event);
    qDebug() << "Close event completed";
}

void WaitingRoomDialog::updateChairs(int waitingCount)
{
    qDebug() << "Updating chairs with count:" << waitingCount;
    if (isClosing || !self) {
        qDebug() << "Ignoring chair update due to closing or deleted object";
        return;
    }

    // Update count label
    if (countLabel) {
        countLabel->setText(QString("Current Count: %1").arg(waitingCount));
    }

    for (int i = 0; i < MAX_CHAIRS; ++i) {
        QLabel *chair = chairLabels[i];
        bool isOccupied = i < waitingCount;
        QPixmap pixmap(isOccupied ? ":/imgs/chair_red.png" : ":/imgs/chair_green.png");
        if (pixmap.isNull()) {
            qDebug() << "Failed to load pixmap:" << (isOccupied ? "chair_red.png" : "chair_green.png");
            chair->setStyleSheet(isOccupied ?
                                     "background-color: #E74C3C; border: 2px solid #C0392B; border-radius: 15px;" :
                                     "background-color: #2ECC71; border: 2px solid #27AE60; border-radius: 15px;");
            chair->setText(isOccupied ? "Occupied" : "Empty");
            chair->setAlignment(Qt::AlignCenter);
            chair->setFont(QFont("Segoe UI", 12, QFont::Bold));
        } else {
            chair->setPixmap(pixmap);
            chair->setText("");
        }
        chair->setToolTip(QString("Chair %1: %2").arg(i + 1).arg(isOccupied ? "Occupied" : "Empty"));
    }
}

void WaitingRoomDialog::onWaitingRoomCountChanged(int count)
{
    qDebug() << "Received waitingRoomCountChanged signal with count:" << count;
    if (isClosing || !self) {
        qDebug() << "Ignoring waitingRoomCountChanged due to closing or deleted object";
        return;
    }
    if (!isVisible()) {
        qDebug() << "Dialog not visible, ignoring count update";
        return;
    }
    updateChairs(count);
    updateLedState(count); // Update LED state when count changes
}

void WaitingRoomDialog::onWrrButtonClicked()
{
    qDebug() << "wrr button clicked in WaitingRoomDialog";

    QSqlQuery query;
    query.prepare("SELECT WR FROM AHMED.MEETING WHERE ID = 1");
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to retrieve WR count for ID 1:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to retrieve waiting room count: " + query.lastError().text());
        return;
    }

    int currentCount = query.value("WR").toInt();
    qDebug() << "Current waiting room count:" << currentCount;

    if (currentCount <= 0) {
        QMessageBox::information(this, "Waiting Room", "The waiting room is already empty!");
        return;
    }

    int newCount = currentCount - 1;

    // Block signals temporarily to prevent recursive updates
    blockSignals(true);

    if (updateMeetingWR(newCount)) {
        updateChairs(newCount);
        updateLedState(newCount); // Update LED state after decreasing count

        // Use direct method call instead of signal
        FormationWindow *formationWindow = qobject_cast<FormationWindow*>(parent());
        if (formationWindow) {
            formationWindow->updateWaitingRoomCount();
            qDebug() << "Called updateWaitingRoomCount directly";
        }
        qDebug() << "Waiting room count updated to:" << newCount;
    } else {
        qDebug() << "Failed to update waiting room count in database";
        QMessageBox::critical(this, "Error", "Failed to update waiting room count in database.");
    }

    // Unblock signals
    blockSignals(false);
}

bool WaitingRoomDialog::updateMeetingWR(int count)
{
    QSqlQuery query;
    query.prepare("UPDATE AHMED.MEETING SET WR = :count WHERE ID = 1");
    query.bindValue(":count", count);
    if (!query.exec()) {
        qDebug() << "Database update error:" << query.lastError().text();
        return false;
    }
    return true;
}

void WaitingRoomDialog::updateLedState(int count)
{
    FormationWindow *formationWindow = qobject_cast<FormationWindow*>(parent());
    if (formationWindow && formationWindow->arduino && formationWindow->arduino->getSerial()->isOpen()) {
        if (count < 5) {
            QString ledCommand = "GREENLED:ON\n";
            if (formationWindow->arduino->writeToArduino(ledCommand.toUtf8())) {
                qDebug() << "Sent command to turn ON green LED from dialog (WR=" << count << ")";
            } else {
                qDebug() << "Failed to send green LED ON command to Arduino from dialog";
            }
        } else {
            QString ledCommand = "GREENLED:OFF\n";
            if (formationWindow->arduino->writeToArduino(ledCommand.toUtf8())) {
                qDebug() << "Sent command to turn OFF green LED from dialog (WR=" << count << ")";
            } else {
                qDebug() << "Failed to send green LED OFF command to Arduino from dialog";
            }
        }
    } else {
        qDebug() << "Cannot update LED state: Arduino not available or not connected";
    }
}
