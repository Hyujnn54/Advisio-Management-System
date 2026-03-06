#ifndef WAITINGROOMDIALOG_H
#define WAITINGROOMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QPointer>
#include <QSqlQuery>

class TrainingManager;

class WaitingRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingRoomDialog(int waitingCount, bool isDarkTheme, TrainingManager *manager, QWidget *parent = nullptr);
    ~WaitingRoomDialog();
    void updateChairs(int waitingCount);

public slots:
    void onWaitingRoomCountChanged(int count);
    void onWrrButtonClicked();
    void onArduinoDataReceived(); // Slot to handle IR sensor data

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI(int waitingCount);
    void applyStylesheet();
    bool updateMeetingWR(int count);
    void updateLedState(int count);

    QGridLayout *chairLayout;
    QList<QLabel*> chairLabels;
    bool isDarkTheme;
    QLabel *clockLabel;
    QTimer *clockTimer;
    QLabel *countLabel;
    QPointer<WaitingRoomDialog> self;
    TrainingManager *trainingManager;
    bool isClosing;
    static const int MAX_CHAIRS = 5;
};

#endif // WAITINGROOMDIALOG_H
