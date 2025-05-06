#ifndef WAITINGROOMDIALOG_H
#define WAITINGROOMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QPointer>
#include <QSqlQuery> // Added for database

class TrainingManager;

class Arduino; // Forward declaration

class WaitingRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingRoomDialog(int waitingCount, bool isDarkTheme, QWidget *parent = nullptr);
    ~WaitingRoomDialog();
    void updateChairs(int waitingCount);

public slots:
    void onWaitingRoomCountChanged(int count);
    void onWrrButtonClicked(); // New slot for wrr button

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI(int waitingCount);
    void applyStylesheet();
    bool updateMeetingWR(int count); // New method to update database
    void updateLedState(int count); // New method to control LED

    QGridLayout *chairLayout;
    QList<QLabel*> chairLabels;
    bool isDarkTheme;
    QLabel *clockLabel;
    QLabel *countLabel; // Added count label
    QTimer *clockTimer;
    bool isClosing;
    QPointer<WaitingRoomDialog> self;
    static const int MAX_CHAIRS = 5;
};

#endif // WAITINGROOMDIALOG_H
