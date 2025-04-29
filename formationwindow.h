#ifndef FORMATIONWINDOW_H
#define FORMATIONWINDOW_H
#include "formations.h"
#include "arduino.h" // Include Arduino header
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QStringList>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer> // For periodic polling

class WaitingRoomDialog; // Forward declaration

QT_BEGIN_NAMESPACE
namespace Ui { class formationwindow; }
QT_END_NAMESPACE

class FormationWindow : public QMainWindow
{
    Q_OBJECT

public:
    FormationWindow(QWidget *parent = nullptr);
    ~FormationWindow();
    bool updateMeetingWR(int count);
    void updateWaitingRoomCount(); // Made public so dialog can call it directly
    Arduino *arduino; // Made public so dialog can access it

private slots:
    void on_addButtonclicked();
    void toggleTheme();
    void toggleSidebar();
    void on_deleteButtonClicked();
    void on_updateButtonClicked();
    void on_searchInput_textChanged(const QString &text);
    void on_resetSearchButton_clicked();
    void exportToPdf();
    void onNotificationLabelClicked();
    void on_refreshStatsButton_clicked();
    void onSmsRequestFinished(QNetworkReply *reply);
    void readArduinoData(); // New slot to read Arduino data
    void on_wrr_clicked(); // New slot for wrr button
    void on_viewWaitingRoomButton_clicked();

private:
    Ui::formationwindow *ui;
    formations formation;
    bool isDarkTheme;
    QSqlQueryModel *tableModel;
    QSortFilterProxyModel *proxyModel;
    int notificationCount;
    QLabel *notificationLabel;
    void applyDarkTheme();
    void applyLightTheme();
    QWidget *sidebarWidget;
    void updateNotificationCount(int change, const QString &changeDescription = "");
    void resetNotificationCount();
    void refreshTableView();
    QStringList changeHistory;
    QNetworkAccessManager *networkManager;
    int totalFormations;
    double totalCost;
    double avgCost;
    void updateStatistics();
    void setupArduino(); // Initialize Arduino connection
    QString arduinoDataBuffer;
    QSerialPort *serialPort;
    QLabel *clockLabel;  // Added
    QTimer *clockTimer;  // Added
    QTimer *arduinoTimer; // Timer for polling Arduino
    WaitingRoomDialog *waitingRoomDialog; // Store dialog instance

signals:
    void waitingRoomCountChanged(int count);
};

#endif // FORMATIONWINDOW_H
