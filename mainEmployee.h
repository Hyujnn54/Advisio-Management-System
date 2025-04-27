#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMessageBox>
#include <QtCharts/QChartView>    // Pour QChartView
#include <QtCharts/QPieSeries>     // Pour QPieSeries
#include <QtCharts/QBarSeries>     // Pour QBarSeries
#include <QtCharts/QBarSet>        // Pour QBarSet
#include <QtCharts/QBarCategoryAxis> // Header correct pour QBarCategory
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QGraphicsLayout>
#include "arduino.h"
#include "managers/employee/Employee.h"
#include "managers/employee/employeemanager.h"
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
    EmployeeManager *employeeManager;
    QSqlQueryModel *employeeModel;
    ImageDelegate *imageDelegate;
    Arduino *arduino;
    QString serialBuffer;
    void setupConnections();
    QMap<int, Qt::SortOrder> sortOrders; // Suivi de l'ordre de tri par colonne
    void handleHeaderClicked(int column);
    bool isDarkTheme;
    void applyDarkTheme();
    void applyLightTheme();
    QString loggedInRole; // Stockage du rôle de l'utilisateur connecté
    void updateUIBasedOnRole();
    void updateStatistics();
    QChartView *pieChartView;
    QChartView *barChartView;
    QImage generateQRCodeImage(const QString &text, int scale = 10); // Génération de QR code en QImage
    QString generateQRCodeForEmployee(int id); // Génération des données de QR code pour un ID d'employé
    void showQRCodeDialog(const QImage &qrImage, int id);
};
#endif // MAINWINDOW_H
