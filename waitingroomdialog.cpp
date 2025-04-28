#include "waitingroomdialog.h"
#include "formationwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QDebug>

WaitingRoomDialog::WaitingRoomDialog(int waitingCount, bool isDarkTheme, QWidget *parent)
    : QDialog(parent), chairLayout(nullptr), chairLabels(), isDarkTheme(isDarkTheme),
    clockLabel(nullptr), clockTimer(new QTimer(this)), isClosing(false), self(this)
{
    qDebug() << "Constructing WaitingRoomDialog";
    setWindowTitle("Waiting Room Overview");
    setModal(true);
    setMinimumSize(400, 300);

    setupUI(waitingCount);
    applyStylesheet();

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

    // Connect to parent’s waitingRoomCountChanged signal
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
    // Qt's parent-child system handles deletion of clockTimer and chairLabels
}

void WaitingRoomDialog::setupUI(int waitingCount)
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

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
    chairLayout->setSpacing(10);
    chairLayout->setAlignment(Qt::AlignCenter);

    // Create chair labels
    for (int i = 0; i < MAX_CHAIRS; ++i) {
        QLabel *chair = new QLabel(this);
        chair->setFixedSize(60, 60);
        chair->setScaledContents(true);
        chair->setObjectName("chairLabel");
        chair->setToolTip(QString("Chair %1: %2").arg(i + 1).arg(i < waitingCount ? "Occupied" : "Empty"));
        chairLabels.append(chair);
        chairLayout->addWidget(chair, i / 3, i % 3);
    }
    mainLayout->addLayout(chairLayout);

    // Close button
    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setObjectName("closeButton");
    connect(closeButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "Close button clicked";
        accept();
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
                                            stop:0 #F28C6F, stop:1 #5C5C5C);
                color: #F0F0F0;
                font-family: 'Segoe UI', Arial, sans-serif;
            }
            QLabel#dialogTitle {
                font-size: 16pt;
                font-weight: bold;
                color: #F28C6F;
                padding: 5px;
            }
            QLabel#clockLabel {
                font-size: 14pt;
                color: #F0F0F0;
                background-color: #6A6A6A;
                border: 1px solid #F28C6F;
                border-radius: 4px;
                padding: 4px;
            }
            QLabel#chairLabel {
                border: 1px solid #F28C6F;
                border-radius: 10px;
            }
            QPushButton#closeButton {
                background-color: #F28C6F;
                color: white;
                border: 1px solid #D96C53;
                border-radius: 5px;
                padding: 6px;
                font-weight: bold;
                box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
            }
            QPushButton#closeButton:hover {
                background-color: #F5A38A;
                box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.4);
            }
            QPushButton#closeButton:pressed {
                background-color: #D96C53;
            }
        )" :
                             R"(
            QDialog {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 #FFFFFF, stop:1 #A1B8E6);
                color: #333333;
                font-family: 'Segoe UI', Arial, sans-serif;
            }
            QLabel#dialogTitle {
                font-size: 16pt;
                font-weight: bold;
                color: #3A5DAE;
                padding: 5px;
            }
            QLabel#clockLabel {
                font-size: 14pt;
                color: #333333;
                background-color: #F5F7FA;
                border: 1px solid #3A5DAE;
                border-radius: 4px;
                padding: 4px;
            }
            QLabel#chairLabel {
                border: 1px solid #3A5DAE;
                border-radius: 10px;
            }
            QPushButton#closeButton {
                background-color: #3A5DAE;
                color: white;
                border: 1px solid #2A4682;
                border-radius: 5px;
                padding: 6px;
                font-weight: bold;
            }
            QPushButton#closeButton:hover {
                background-color: #4A70C2;
            }
            QPushButton#closeButton:pressed {
                background-color: #2A4682;
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

    // Disconnect from parent’s signal
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
    for (int i = 0; i < MAX_CHAIRS; ++i) {
        QLabel *chair = chairLabels[i];
        bool isOccupied = i < waitingCount;
        QPixmap pixmap(isOccupied ? ":/imgs/chair_red.png" : ":/imgs/chair_green.png");
        if (pixmap.isNull()) {
            qDebug() << "Failed to load pixmap:" << (isOccupied ? "chair_red.png" : "chair_green.png");
            chair->setStyleSheet(isOccupied ?
                                     "background-color: #F44336; border: 1px solid #B71C1C; border-radius: 10px;" :
                                     "background-color: #4CAF50; border: 1px solid #2E7D32; border-radius: 10px;");
            chair->setText(isOccupied ? "Occupied" : "Empty");
            chair->setAlignment(Qt::AlignCenter);
            chair->setFont(QFont("Segoe UI", 10, QFont::Bold));
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
}
