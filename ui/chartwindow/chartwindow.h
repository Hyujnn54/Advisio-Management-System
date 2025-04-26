// ui/chartwindow/chartwindow.h
#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QtCharts>
#include <QLabel>
#include <QSqlQuery>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWindow; }
QT_END_NAMESPACE

class ChartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr);
    ~ChartWindow();

    // New method to apply theme
    void applyTheme(bool isDarkTheme);

private slots:
    void updateChart();
    void handlePieSliceHovered(QPieSlice *slice, bool state);
    void handleBarHovered(bool status, int index);
    void on_resetButton_clicked();
    void on_chartDetailComboBox_currentIndexChanged(int index);
    void on_filterComboBox_currentIndexChanged(int index);
    void updateFilterOptions();

protected:
    // Override resize event for better responsiveness
    void resizeEvent(QResizeEvent *event) override;
    // Override show event to apply theme when shown
    void showEvent(QShowEvent *event) override;

private:
    Ui::ChartWindow *ui;
    QChart *currentChart;
    QLabel *tooltipLabel;
    QBarSet *currentBarSet;
    QString currentDetailType;
    QString currentFilter;
    bool isDarkTheme;
    QPropertyAnimation *hoverAnimation;
    QGraphicsOpacityEffect *hoverEffect;
    
    // Functions for different statistics types
    void createClientStatistics(QString chartType);
    void createTrainingStatistics(QString chartType);
    void createMeetingStatistics(QString chartType);
    
    // Detail options for each stats type
    QStringList getClientDetailOptions();
    QStringList getTrainingDetailOptions();
    QStringList getMeetingDetailOptions();
    
    // Filter options based on type and detail
    QStringList getFilterOptions(QString statsType, QString detailType);
    
    // Apply filter to query
    QString applyFilterToQuery(QString baseQuery, QString statsType, QString detailType, QString filter);
    
    // Detect if parent is using dark theme
    bool detectDarkTheme();
    // Update tooltip style based on theme
    void updateTooltipStyle();
    // Setup animations for hover effects
    void setupHoverAnimations();
    // Apply hover animation to chart element
    void animateChartElement(QGraphicsItem *item, bool hovered);
};

#endif // CHARTWINDOW_H
