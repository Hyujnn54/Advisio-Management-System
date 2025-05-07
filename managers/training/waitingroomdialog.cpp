#include "waitingroomdialog.h"
#include "trainingmanager.h"
#include "ui/waitingArd/arduinoy.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCloseEvent>

// Define static member after includes
const int WaitingRoomDialog::MAX_CHAIRS;

WaitingRoomDialog::WaitingRoomDialog(int waitingCount, bool isDarkTheme, TrainingManager *manager, QWidget *parent)
    : QDialog(parent),
    chairLayout(nullptr),
    chairLabels(),
    isDarkTheme(isDarkTheme),
    clockLabel(nullptr),
    clockTimer(new QTimer(this)),
    countLabel(nullptr),
    self(this),
    trainingManager(manager),
    isClosing(false)
{
    qDebug() << "Entering WaitingRoomDialog constructor";
    if (!manager) {
        qDebug() << "Warning: TrainingManager is null";
    }

    setWindowTitle("Waiting Room Overview");
    setModal(false);
    setMinimumSize(400, 300);

    setupUI(waitingCount);
    applyStylesheet();
    updateLedState(waitingCount);

    connect(clockTimer, &QTimer::timeout, this, [=]() {
        if (isClosing || !self) {
            qDebug() << "Clock timer ignored due to closing or deleted object";
            return;
        }
        if (clockLabel) {
            clockLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
        }
    });
    clockTimer->start(1000);

    if (trainingManager) {
        connect(trainingManager, &TrainingManager::waitingRoomCountChanged,
                this, &WaitingRoomDialog::onWaitingRoomCountChanged, Qt::UniqueConnection);

        // Connect to Arduino serial data for IR sensor detection
        Arduinoy *arduino = trainingManager->getArduino();
        if (arduino && arduino->getSerial()) {
            connect(arduino->getSerial(), &QSerialPort::readyRead, this, &WaitingRoomDialog::onArduinoDataReceived);
            qDebug() << "Connected to Arduino readyRead signal";
        } else {
            qDebug() << "Cannot connect to Arduino readyRead signal: Arduino or serial port is null";
        }
    }

    qDebug() << "Exiting WaitingRoomDialog constructor";
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
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    QLabel *titleLabel = new QLabel("Waiting Room Chairs", this);
    titleLabel->setObjectName("dialogTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    clockLabel = new QLabel(QTime::currentTime().toString("hh:mm:ss"), this);
    clockLabel->setObjectName("clockLabel");
    clockLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(clockLabel);

    chairLayout = new QGridLayout();
    chairLayout->setSpacing(15);
    chairLayout->setAlignment(Qt::AlignCenter);

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

    countLabel = new QLabel(QString("Current Count: %1").arg(waitingCount), this);
    countLabel->setObjectName("countLabel");
    countLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(countLabel);

    QPushButton *wrrButton = new QPushButton("-", this);
    wrrButton->setObjectName("wrr");
    wrrButton->setFixedSize(100, 50);
    connect(wrrButton, &QPushButton::clicked, this, &WaitingRoomDialog::onWrrButtonClicked);
    mainLayout->addWidget(wrrButton, 0, Qt::AlignCenter);

    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setObjectName("closeButton");
    connect(closeButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "Close button clicked";
        isClosing = true;
        accept();
    });
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

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

    clockTimer->stop();
    clockTimer->blockSignals(true);

    QDialog::closeEvent(event);
    qDebug() << "Close event completed";
}

void WaitingRoomDialog::updateChairs(int waitingCount)
{
    qDebug() << "Entering updateChairs with count:" << waitingCount;
    if (isClosing || !self) {
        qDebug() << "Ignoring chair update due to closing or deleted object";
        return;
    }

    if (!countLabel) {
        qDebug() << "Error: countLabel is null";
        return;
    }
    countLabel->setText(QString("Current Count: %1").arg(waitingCount));
    countLabel->update();

    for (int i = 0; i < MAX_CHAIRS; ++i) {
        if (i >= chairLabels.size()) {
            qDebug() << "Error: chairLabels index out of range:" << i;
            return;
        }
        QLabel *chair = chairLabels[i];
        if (!chair) {
            qDebug() << "Error: chair label is null at index:" << i;
            return;
        }
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
        chair->update();
    }

    update();
    qDebug() << "Exiting updateChairs";
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
    updateLedState(count);
    qDebug() << "Completed waitingRoomCountChanged handling";
}

void WaitingRoomDialog::onWrrButtonClicked()
{
    qDebug() << "wrr button clicked in WaitingRoomDialog";

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not connected";
        QMessageBox::critical(this, "Error", "Database not connected.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT WR FROM AHMED.MEETING WHERE ID = 1");
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to retrieve WR count for ID 1:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to retrieve waiting room count: " + query.lastError().text());
        return;
    }

    int currentCount = query.value("WR").toInt();
    qDebug() << "Current waiting room count for ID 1:" << currentCount;

    if (currentCount <= 0) {
        QMessageBox::information(this, "Waiting Room", "The waiting room is already empty!");
        return;
    }

    int newCount = currentCount - 1;

    blockSignals(true);
    if (updateMeetingWR(newCount)) {
        updateChairs(newCount);
        updateLedState(newCount);

        if (trainingManager) {
            emit trainingManager->waitingRoomCountChanged(newCount);
            qDebug() << "Emitted waitingRoomCountChanged with count:" << newCount;
        } else {
            qDebug() << "TrainingManager is null, cannot emit signal";
        }
    } else {
        qDebug() << "Failed to update waiting room count in database for ID 1";
        QMessageBox::critical(this, "Error", "Failed to update waiting room count in database.");
    }
    blockSignals(false);
}

bool WaitingRoomDialog::updateMeetingWR(int count)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    if (!db.transaction()) {
        qDebug() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE AHMED.MEETING SET WR = :count WHERE ID = 1");
    query.bindValue(":count", qMin(count, MAX_CHAIRS)); // Ensure count doesn't exceed 5

    if (!query.exec()) {
        qDebug() << "Database update error for ID 1:" << query.lastError().text();
        db.rollback();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No rows updated for ID 1";
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        qDebug() << "Failed to commit transaction for ID 1";
        db.rollback();
        return false;
    }

    qDebug() << "Updated WR to" << qMin(count, MAX_CHAIRS) << "for ID 1";
    return true;
}

void WaitingRoomDialog::updateLedState(int count)
{
    if (!trainingManager) {
        qDebug() << "Skipping updateLedState: trainingManager is null";
        return;
    }
    if (!trainingManager->getArduino()) {
        qDebug() << "Skipping updateLedState: Arduino is null";
        return;
    }
    Arduinoy *arduino = trainingManager->getArduino();
    if (!arduino->getSerial()) {
        qDebug() << "Skipping updateLedState: Serial port is null";
        return;
    }
    if (!arduino->getSerial()->isOpen()) {
        qDebug() << "Arduino serial port not open, attempting to reconnect";
        if (arduino->connectArduino() != 0) {
            qDebug() << "Failed to reconnect to Arduino";
            return;
        }
    }
    QString ledCommand = (count < 5) ? "GREENLED:ON\n" : "GREENLED:OFF\n";
    if (arduino->writeToArduino(ledCommand.toUtf8())) {
        qDebug() << "Sent command to Arduino:" << ledCommand.trimmed();
    } else {
        qDebug() << "Failed to send command to Arduino:" << ledCommand.trimmed();
    }
}

void WaitingRoomDialog::onArduinoDataReceived()
{
    if (!trainingManager) {
        qDebug() << "Skipping onArduinoDataReceived: trainingManager is null";
        return;
    }
    Arduinoy *arduino = trainingManager->getArduino();
    if (!arduino || !arduino->getSerial()) {
        qDebug() << "Skipping onArduinoDataReceived: Arduino or serial port is null";
        return;
    }

    QByteArray data = arduino->readFromArduino();
    if (data.isEmpty()) {
        qDebug() << "No data received from Arduino";
        return;
    }

    QString received = QString::fromUtf8(data).trimmed();
    qDebug() << "Received from Arduino:" << received;

    // Parse Arduino message for IR sensor detection
    if (received.startsWith("Person detected. Total: ")) {
        QStringList parts = received.split("Total: ");
        if (parts.size() > 1) {
            QString countStr = parts[1].split(" ")[0];
            bool ok;
            int count = countStr.toInt(&ok);
            if (ok && count >= 0) {
                // Ensure count doesn't exceed MAX_CHAIRS (5) and update only ID = 1
                if (count <= MAX_CHAIRS) {
                    if (updateMeetingWR(count)) {
                        qDebug() << "Updated WR in database to:" << count << "for ID 1";
                        updateChairs(count); // Update UI to show red chairs
                        updateLedState(count);
                        if (trainingManager) {
                            emit trainingManager->waitingRoomCountChanged(count);
                        }
                    } else {
                        qDebug() << "Failed to update WR in database for ID 1";
                        QMessageBox::critical(this, "Error", "Failed to update waiting room count in database.");
                    }
                } else {
                    qDebug() << "Count exceeds maximum (" << MAX_CHAIRS << "), not updating database for ID 1";
                }
            } else {
                qDebug() << "Invalid count received from Arduino:" << countStr;
            }
        }
    }
}
