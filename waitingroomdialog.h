#ifndef WAITINGROOMDIALOG_H
#define WAITINGROOMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QPointer>

class WaitingRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingRoomDialog(int waitingCount, bool isDarkTheme, QWidget *parent = nullptr);
    ~WaitingRoomDialog();
    void updateChairs(int waitingCount);

public slots:
    void onWaitingRoomCountChanged(int count);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI(int waitingCount);
    void applyStylesheet();

    QGridLayout *chairLayout;
    QList<QLabel*> chairLabels;
    bool isDarkTheme;
    QLabel *clockLabel;
    QTimer *clockTimer;
    bool isClosing; // Flag to prevent signal processing during closure
    QPointer<WaitingRoomDialog> self; // Guard against deleted object access
    static const int MAX_CHAIRS = 5;
};

#endif // WAITINGROOMDIALOG_H
