#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client/clientmanager.h"
#include "training/trainingmanager.h"
#include "notificationmanager.h"  // Add this include
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool dbConnected, NotificationManager *notificationManager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();
    void on_statisticsButton_clicked();
    void toggleSidebar();
    void toggleTheme();

private:
    void setupUiConnections();
    void applyLightTheme();
    void applyDarkTheme();

    Ui::MainWindow *ui;
    bool m_dbConnected;
    ClientManager *clientManager;
    TrainingManager *trainingManager;
    NotificationManager *notificationManager;  // Add this member
};

#endif // MAINWINDOW_H
