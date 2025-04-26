// ui/chartwindow/chartwindow.h
#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QtCharts>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWindow; }
QT_END_NAMESPACE

class ChartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr);
    ~ChartWindow();

private slots:
    void updateChart();
    void handlePieSliceHovered(QPieSlice *slice, bool state);
    void handleBarHovered(bool status, int index);
    void populateStatsViews();
    void on_refreshButton_clicked();
    void on_resetButton_clicked();
    
    // New slots for enhanced functionality
    void onAnimationFinished();
    void animatePieChart(QPieSeries *series);
    void animateBarChart(QBarSeries *series);
    void updatePercentageLabels();
    void on_chartDetailComboBox_currentIndexChanged(int index);

private:
    Ui::ChartWindow *ui;
    QChart *currentChart;
    QLabel *tooltipLabel;
    QBarSet *currentBarSet;
    QSequentialAnimationGroup *animationGroup;
    
    // Helper methods for specific chart types
    void createClientStatistics(QString chartType);
    void createTrainingStatistics(QString chartType);
    void createMeetingStatistics(QString chartType);
    
    // Detail selectors for each chart type
    QStringList getClientDetailOptions();
    QStringList getTrainingDetailOptions();
    QStringList getMeetingDetailOptions();
    
    // Additional data for statistics
    QString currentDetailType;
};

#endif // CHARTWINDOW_H
