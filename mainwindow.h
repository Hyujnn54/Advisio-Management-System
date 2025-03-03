#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "qcalendarwidget.h"
#include "qdatetimeedit.h"
#include "updateclientdialog.h"

#include <QMainWindow>
#include <QPalette>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButtonclicked();
    void on_removeButtonClicked();
    void on_updateButtonClicked();
    void toggleSidebar();
    void toggleTheme();
    void on_searchNameButton_clicked();
    void on_searchSectorButton_clicked();
    void on_searchDateTimeButton_clicked();
    void on_resetSearchButton_clicked();
    void tableViewHeaderClicked(int logicalIndex);
    void on_consultationCalendar_selectionChanged();
    void on_consultationCalendar_activated(const QDate &date);
    void updateCalendarConsultations();
    bool isValidDateTime(const QDateTime &dateTime);

private:
    Ui::MainWindow *ui;
    Client Etmp;
    QWidget *sidebarWidget;
    bool isDarkTheme;
    // Store both date and datetime maps for the calendar view
    QMap<QDate, int> consultationCountMap;
    QMap<QDateTime, int> consultationDateTimeMap;
    QDate lastHoveredDate;

    void applyDarkTheme();
    void applyLightTheme();
    bool validateInputs();
    bool isValidName(const QString &name);
    bool isValidDate(const QDate &date);
    bool isValidConsultant(const QString &consultant);
    void setupCalendarView();
    void highlightDatesWithConsultations();
    void updateSelectedDateInfo(const QDate &date);
};

class CalendarHoverItem : public QObject
{
    Q_OBJECT
public:
    CalendarHoverItem(QCalendarWidget *calendar, QMap<QDate, int> *consultationCounts)
        : QObject(static_cast<QObject*>(calendar)), calendar(calendar), consultationCounts(consultationCounts) {}

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QCalendarWidget *calendar;
    QMap<QDate, int> *consultationCounts;
};

#endif // MAINWINDOW_H
