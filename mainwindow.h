#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMessageBox>
#include <QtCharts/QChartView>    // For QChartView
#include <QtCharts/QPieSeries>     // For QPieSeries
#include <QtCharts/QBarSeries>     // For QBarSeries
#include <QtCharts/QBarSet>        // For QBarSet
#include <QtCharts/QBarCategoryAxis> // Correct header for QBarCategory
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QGraphicsLayout>
#include "arduino.h"
#include "employee.h"
#include "imageDelegate.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setLoggedInRole(const QString &role);
private slots:
    void on_addButtonclicked();
    void loadEmployees();
    void on_modifyBtnClicked();
    void on_saveUpdateBtnClicked();
    void on_deleteBtnClicked();
    void on_downloadBtnClicked();
    void on_resetSearchButtonclicked();
    void searchEmployees();
    void sortEmployees(int column);
    void toggleTheme();
    void toggleSidebar();
    void on_logoutButtonclicked();
    void on_refreshStatsButtonClicked();
    void on_generateQRCodeBtnClicked();
    void handleSerialData();
private:
    Ui::MainWindow *ui;
    Employee *employee;
    QSqlQueryModel *employeeModel;
    ImageDelegate *imageDelegate;
    Arduino *arduino;
    QString serialBuffer;
    void setupConnections();
    QMap<int, Qt::SortOrder> sortOrders; // Track sort order per column
    void handleHeaderClicked(int column);
    bool isDarkTheme;
    void applyDarkTheme();
    void applyLightTheme();
    QString loggedInRole; // Store the logged-in user's role
    void updateUIBasedOnRole();
    void updateStatistics();
    QChartView *pieChartView;
    QChartView *barChartView;
    QImage generateQRCodeImage(const QString &text, int scale = 10); // Generate QR code as QImage
    QString generateQRCodeForEmployee(int id); // Generate QR code data for employee ID
    void showQRCodeDialog(const QImage &qrImage, int id);
};
#endif // MAINWINDOW_H
