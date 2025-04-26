#include "chartwindow.h"
#include "ui_ChartWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>

ChartWindow::ChartWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentChart(nullptr),
    tooltipLabel(new QLabel(this)),
    currentBarSet(nullptr),
    animationGroup(nullptr),
    currentDetailType("")
{
    ui->setupUi(this);

    tooltipLabel->setStyleSheet("QLabel { background-color: white; border: 1px solid black; padding: 5px; border-radius: 5px; }");
    tooltipLabel->hide();

    // Block signals during initialization
    ui->statsTypeComboBox->blockSignals(true);
    ui->chartDetailComboBox->blockSignals(true);

    // Connect signals to slots
    connect(ui->refreshButton, &QPushButton::clicked, this, &ChartWindow::on_refreshButton_clicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &ChartWindow::on_resetButton_clicked);
    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        // Update chart detail options when stats type changes
        QString statsType = ui->statsTypeComboBox->currentText();
        ui->chartDetailComboBox->blockSignals(true);
        ui->chartDetailComboBox->clear();
        
        if (statsType == "Client Statistics") {
            ui->chartDetailComboBox->addItems(getClientDetailOptions());
        } else if (statsType == "Training Statistics") {
            ui->chartDetailComboBox->addItems(getTrainingDetailOptions());
        } else if (statsType == "Meeting Statistics") {
            ui->chartDetailComboBox->addItems(getMeetingDetailOptions());
        }
        
        if (ui->chartDetailComboBox->count() > 0) {
            ui->chartDetailComboBox->setCurrentIndex(0);
        }
        
        ui->chartDetailComboBox->blockSignals(false);
        updateChart();
    });
    
    connect(ui->chartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);
    connect(ui->toggleLegendCheckBox, &QCheckBox::toggled, this, &ChartWindow::updateChart);
    connect(ui->chartDetailComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::on_chartDetailComboBox_currentIndexChanged);

    // Re-enable signals after initialization
    ui->statsTypeComboBox->blockSignals(false);
    ui->chartDetailComboBox->blockSignals(false);
    
    // Set default detail options
    ui->chartDetailComboBox->addItems(getClientDetailOptions());
    
    // Initial chart refresh
    QTimer::singleShot(100, this, &ChartWindow::updateChart);
}

ChartWindow::~ChartWindow()
{
    if (tooltipLabel) {
        delete tooltipLabel;
        tooltipLabel = nullptr;
    }
    
    if (animationGroup) {
        delete animationGroup;
        animationGroup = nullptr;
    }
    
    delete ui;
}

QStringList ChartWindow::getClientDetailOptions()
{
    return {"Sector", "Consultation Month", "Consultant"};
}

QStringList ChartWindow::getTrainingDetailOptions()
{
    return {"Formation Type", "Trainer", "Price Range", "Month"};
}

QStringList ChartWindow::getMeetingDetailOptions()
{
    return {"Duration", "Time of Day", "Organiser", "Day of Week"};
}

void ChartWindow::on_chartDetailComboBox_currentIndexChanged(int index)
{
    if (index >= 0) {
        currentDetailType = ui->chartDetailComboBox->currentText();
        updateChart();
    }
}

void ChartWindow::populateStatsViews()
{
    ui->statsTypeComboBox->blockSignals(true);
    ui->statsTypeComboBox->clear();
    ui->statsTypeComboBox->addItems({"Client Statistics", "Training Statistics", "Meeting Statistics"});
    ui->statsTypeComboBox->blockSignals(false);
    
    qDebug() << "Populated statsTypeComboBox with" << ui->statsTypeComboBox->count() << "items";
    
    // Set default detail options
    ui->chartDetailComboBox->blockSignals(true);
    ui->chartDetailComboBox->clear();
    ui->chartDetailComboBox->addItems(getClientDetailOptions());
    ui->chartDetailComboBox->blockSignals(false);
    
    updateChart();
}

void ChartWindow::updateChart()
{
    QString statsType = ui->statsTypeComboBox->currentText();
    QString chartType = ui->chartTypeComboBox->currentText();
    currentDetailType = ui->chartDetailComboBox->currentText();

    qDebug() << "Starting updateChart() with statsType:" << statsType 
             << ", chartType:" << chartType 
             << ", detailType:" << currentDetailType;

    // Update percentage label
    ui->percentageLabel->clear();
    
    // Stop any running animations first
    if (animationGroup) {
        animationGroup->stop();
        disconnect(animationGroup, &QSequentialAnimationGroup::finished, this, &ChartWindow::onAnimationFinished);
    }

    // Handle empty statsType
    if (statsType.isEmpty()) {
        qDebug() << "Error: statsType is empty, defaulting to 'Client Statistics'";
        statsType = "Client Statistics";
        ui->statsTypeComboBox->setCurrentIndex(0);
    }
    
    try {
        // Create a new chart or reuse the existing one
        if (!currentChart) {
            currentChart = new QChart();
        } else {
            // Clear the existing chart completely
            currentChart->removeAllSeries();
            
            // Safely remove axes
            QList<QAbstractAxis*> axes = currentChart->axes();
            for (QAbstractAxis* axis : axes) {
                if (axis) {
                    currentChart->removeAxis(axis);
                    delete axis; // Ensure axis is deleted
                }
            }
            
            // Remove all items (like "No data available" text)
            if (currentChart->scene()) {
                QList<QGraphicsItem*> items = currentChart->scene()->items();
                for (QGraphicsItem *item : items) {
                    if (item && item->parentItem() == nullptr) { // Only remove top-level items
                        currentChart->scene()->removeItem(item);
                        delete item;
                    }
                }
            }
        }

        // Reset current bar set pointer
        currentBarSet = nullptr;

        currentChart->setTitle(statsType + " - " + currentDetailType);
        currentChart->setAnimationOptions(QChart::SeriesAnimations);  // Simplified animation options
        currentChart->setAnimationDuration(800);  // Shorter duration for better stability
        currentChart->setTheme(QChart::ChartThemeLight);
        
        // Create the specific chart based on user selection
        if (statsType == "Client Statistics") {
            createClientStatistics(chartType);
        } else if (statsType == "Training Statistics") {
            createTrainingStatistics(chartType);
        } else if (statsType == "Meeting Statistics") {
            createMeetingStatistics(chartType);
        }

        // Set up legend
        if (currentChart->legend()) {
            currentChart->legend()->setVisible(ui->toggleLegendCheckBox->isChecked());
            currentChart->legend()->setAlignment(Qt::AlignBottom);
        }
        
        // Apply the chart to the view
        if (ui->statsChartView) {
            ui->statsChartView->setChart(currentChart);
            ui->statsChartView->setRenderHint(QPainter::Antialiasing);
            qDebug() << "Set chart to QChartView successfully";
        } else {
            qDebug() << "Error: QChartView is null";
        }
    } catch (const std::exception &e) {
        qDebug() << "Exception in updateChart:" << e.what();
        QMessageBox::warning(this, "Chart Error", "An error occurred while updating the chart: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateChart";
        QMessageBox::warning(this, "Chart Error", "An unknown error occurred while updating the chart");
    }
    
    qDebug() << "Finished updateChart()";
}

void ChartWindow::createClientStatistics(QString chartType)
{
    QSqlQuery query;
    bool dataAvailable = false;
    int total = 0;
    QString queryStr;
    
    try {
        // Build the query based on detail type
        if (currentDetailType == "Sector") {
            // Handle NULL or empty sectors with COALESCE
            queryStr = "SELECT COALESCE(SECTOR, 'Not Specified') as SECTOR, COUNT(*) FROM AHMED.CLIENTS GROUP BY SECTOR";
        } else if (currentDetailType == "Consultation Month") {
            queryStr = "SELECT TO_CHAR(CONSULTATION_DATE, 'Month') as MONTH, COUNT(*) FROM AHMED.CLIENTS GROUP BY TO_CHAR(CONSULTATION_DATE, 'Month') ORDER BY MIN(CONSULTATION_DATE)";
        } else if (currentDetailType == "Consultant") {
            queryStr = "SELECT COALESCE((e.FIRST_NAME || ' ' || e.LAST_NAME), 'Not Assigned') AS CONSULTANT_NAME, COUNT(*) "
                      "FROM AHMED.CLIENTS c "
                      "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                      "GROUP BY (e.FIRST_NAME || ' ' || e.LAST_NAME)";
        } else {
            // Default to sector with NULL handling
            queryStr = "SELECT COALESCE(SECTOR, 'Not Specified') as SECTOR, COUNT(*) FROM AHMED.CLIENTS GROUP BY SECTOR";
        }
        
        qDebug() << "Executing query: " << queryStr;
        
        // Prepare the query before executing to avoid ODBC errors
        if (!query.prepare(queryStr)) {
            throw std::runtime_error("Failed to prepare query: " + query.lastError().text().toStdString());
        }
        
        if (!query.exec()) {
            throw std::runtime_error("Query execution failed: " + query.lastError().text().toStdString());
        }
        
        if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Clients");
            QStringList categories;
            int maxValue = 0;
            
            // Process each row of the query result
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty
                if (category.isEmpty()) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value, using 0 instead";
                    count = 0;
                }
                
                // Trim month names if applicable
                if (currentDetailType == "Consultation Month") {
                    category = category.trimmed();
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Add to bar chart data
                *set << count;
                categories << category;
                maxValue = qMax(maxValue, count);
                total += count;
                dataAvailable = true;
            }
            
            if (dataAvailable) {
                series->append(set);
                currentChart->addSeries(series);
                
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                axisX->append(categories);
                currentChart->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                
                QValueAxis *axisY = new QValueAxis();
                axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1); // Add 10% padding
                axisY->setTitleText("Number of Clients");
                axisY->setLabelFormat("%d");
                currentChart->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                
                currentBarSet = set;
                connect(set, &QBarSet::hovered, this, &ChartWindow::handleBarHovered);
                
                // Animate the bars
                animateBarChart(series);
            }
        } else { // Pie Chart
            QPieSeries *series = new QPieSeries();
            
            query.exec(queryStr); // Re-execute query for pie chart
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty
                if (category.isEmpty()) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value, using 0 instead";
                    count = 0;
                }
                
                // Trim month names if applicable
                if (currentDetailType == "Consultation Month") {
                    category = category.trimmed();
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Only add non-zero slices to avoid division by zero in percentage calculations
                if (count > 0) {
                    series->append(category, count);
                    total += count;
                    dataAvailable = true;
                }
            }
            
            if (dataAvailable) {
                currentChart->addSeries(series);
                
                // Set up pie slices with labels and percentages
                for (QPieSlice *slice : series->slices()) {
                    slice->setLabel(QString("%1: %2 (%3%)")
                                  .arg(slice->label())
                                  .arg(static_cast<int>(slice->value()))
                                  .arg(slice->percentage() * 100, 0, 'f', 1));
                    slice->setLabelVisible(true);
                    slice->setLabelPosition(QPieSlice::LabelOutside);
                    slice->setBorderWidth(1);
                    slice->setBorderColor(Qt::white);
                    
                    connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                        handlePieSliceHovered(slice, state);
                    });
                }
                
                // Animate the pie
                animatePieChart(series);
            }
        }
        
        if (!dataAvailable) {
            qDebug() << "No data available, adding placeholder text";
            QGraphicsTextItem *textItem = new QGraphicsTextItem("No client data available", currentChart);
            textItem->setPos(50, 50);
            if (currentChart->scene()) {
                currentChart->scene()->addItem(textItem);
            } else {
                qDebug() << "Error: Chart scene is null";
            }
        } else {
            // Update total display
            ui->percentageLabel->setText(QString("Total Clients: %1").arg(total));
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error in createClientStatistics:" << e.what();
        QMessageBox::warning(this, "Chart Error", 
            QString("Failed to create client statistics: %1").arg(e.what()));
        
        // Add error message to chart
        QGraphicsTextItem *textItem = new QGraphicsTextItem(
            QString("Error: %1").arg(e.what()), currentChart);
        textItem->setPos(50, 50);
        if (currentChart->scene()) {
            currentChart->scene()->addItem(textItem);
        }
    }
}

void ChartWindow::createTrainingStatistics(QString chartType)
{
    QSqlQuery query;
    bool dataAvailable = false;
    int total = 0;
    QString queryStr;
    
    try {
        // Build the query based on detail type
        if (currentDetailType == "Formation Type") {
            queryStr = "SELECT COALESCE(FORMATION, 'Not Specified') as FORMATION, COUNT(*) FROM AHMED.FORMATIONS GROUP BY FORMATION";
        } else if (currentDetailType == "Trainer") {
            queryStr = "SELECT COALESCE(TRAINER, 'Not Specified') as TRAINER, COUNT(*) FROM AHMED.FORMATIONS GROUP BY TRAINER";
        } else if (currentDetailType == "Price Range") {
            queryStr = "SELECT CASE "
                      "WHEN PRIX <= 50 THEN 'Low (≤ 50)' "
                      "WHEN PRIX <= 100 THEN 'Medium (51-100)' "
                      "ELSE 'High (> 100)' END AS PRICE_RANGE, COUNT(*) "
                      "FROM AHMED.FORMATIONS GROUP BY CASE "
                      "WHEN PRIX <= 50 THEN 'Low (≤ 50)' "
                      "WHEN PRIX <= 100 THEN 'Medium (51-100)' "
                      "ELSE 'High (> 100)' END";
        } else if (currentDetailType == "Month") {
            queryStr = "SELECT TO_CHAR(DATEF, 'Month') as MONTH, COUNT(*) FROM AHMED.FORMATIONS GROUP BY TO_CHAR(DATEF, 'Month') ORDER BY MIN(DATEF)";
        } else {
            // Default to formation type
            queryStr = "SELECT COALESCE(FORMATION, 'Not Specified') as FORMATION, COUNT(*) FROM AHMED.FORMATIONS GROUP BY FORMATION";
        }
        
        qDebug() << "Executing query: " << queryStr;
        
        // Prepare the query before executing to avoid ODBC errors
        if (!query.prepare(queryStr)) {
            throw std::runtime_error("Failed to prepare query: " + query.lastError().text().toStdString());
        }
        
        if (!query.exec()) {
            throw std::runtime_error("Query execution failed: " + query.lastError().text().toStdString());
        }

        if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Trainings");
            QStringList categories;
            int maxValue = 0;
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty
                if (category.isEmpty()) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value, using 0 instead";
                    count = 0;
                }
                
                // Trim month names if applicable
                if (currentDetailType == "Month") {
                    category = category.trimmed();
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                *set << count;
                categories << category;
                maxValue = qMax(maxValue, count);
                total += count;
                dataAvailable = true;
            }
            
            if (dataAvailable) {
                series->append(set);
                currentChart->addSeries(series);
                
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                axisX->append(categories);
                currentChart->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                
                QValueAxis *axisY = new QValueAxis();
                axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1); // Add 10% padding
                axisY->setTitleText("Number of Trainings");
                axisY->setLabelFormat("%d");
                currentChart->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                
                currentBarSet = set;
                connect(set, &QBarSet::hovered, this, &ChartWindow::handleBarHovered);
                
                // Animate the bars
                animateBarChart(series);
            }
        } else { // Pie Chart
            QPieSeries *series = new QPieSeries();
            
            // Re-execute query for pie chart
            if (!query.prepare(queryStr)) {
                throw std::runtime_error("Failed to prepare pie chart query: " + query.lastError().text().toStdString());
            }
            
            if (!query.exec()) {
                throw std::runtime_error("Pie chart query execution failed: " + query.lastError().text().toStdString());
            }
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty
                if (category.isEmpty()) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value, using 0 instead";
                    count = 0;
                }
                
                // Trim month names if applicable
                if (currentDetailType == "Month") {
                    category = category.trimmed();
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Only add non-zero slices to avoid division by zero in percentage calculations
                if (count > 0) {
                    series->append(category, count);
                    total += count;
                    dataAvailable = true;
                }
            }
            
            if (dataAvailable) {
                currentChart->addSeries(series);
                
                // Set up pie slices with labels and percentages
                for (QPieSlice *slice : series->slices()) {
                    slice->setLabel(QString("%1: %2 (%3%)")
                                  .arg(slice->label())
                                  .arg(static_cast<int>(slice->value()))
                                  .arg(slice->percentage() * 100, 0, 'f', 1));
                    slice->setLabelVisible(true);
                    slice->setLabelPosition(QPieSlice::LabelOutside);
                    slice->setBorderWidth(1);
                    slice->setBorderColor(Qt::white);
                    
                    connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                        handlePieSliceHovered(slice, state);
                    });
                }
                
                // Animate the pie
                animatePieChart(series);
            }
        }
        
        if (!dataAvailable) {
            qDebug() << "No data available, adding placeholder text";
            QGraphicsTextItem *textItem = new QGraphicsTextItem("No training data available", currentChart);
            textItem->setPos(50, 50);
            if (currentChart->scene()) {
                currentChart->scene()->addItem(textItem);
            } else {
                qDebug() << "Error: Chart scene is null";
            }
        } else {
            // Update total display
            ui->percentageLabel->setText(QString("Total Trainings: %1").arg(total));
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error in createTrainingStatistics:" << e.what();
        QMessageBox::warning(this, "Chart Error", 
            QString("Failed to create training statistics: %1").arg(e.what()));
        
        // Add error message to chart
        QGraphicsTextItem *textItem = new QGraphicsTextItem(
            QString("Error: %1").arg(e.what()), currentChart);
        textItem->setPos(50, 50);
        if (currentChart->scene()) {
            currentChart->scene()->addItem(textItem);
        }
    }
}

void ChartWindow::createMeetingStatistics(QString chartType)
{
    QSqlQuery query;
    bool dataAvailable = false;
    int total = 0;
    QString queryStr;
    
    try {
        // Build the query based on detail type
        if (currentDetailType == "Duration") {
            queryStr = "SELECT CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END AS RANGE, COUNT(*) "
                      "FROM AHMED.MEETING GROUP BY CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END";
        } else if (currentDetailType == "Time of Day") {
            queryStr = "SELECT CASE "
                      "WHEN TO_CHAR(DATEM, 'HH24') < '12' THEN 'Morning' "
                      "WHEN TO_CHAR(DATEM, 'HH24') < '17' THEN 'Afternoon' "
                      "ELSE 'Evening' END AS TIME_OF_DAY, COUNT(*) "
                      "FROM AHMED.MEETING GROUP BY CASE "
                      "WHEN TO_CHAR(DATEM, 'HH24') < '12' THEN 'Morning' "
                      "WHEN TO_CHAR(DATEM, 'HH24') < '17' THEN 'Afternoon' "
                      "ELSE 'Evening' END";
        } else if (currentDetailType == "Organiser") {
            queryStr = "SELECT COALESCE(ORGANISER, 'Not Specified') as ORGANISER, COUNT(*) FROM AHMED.MEETING GROUP BY ORGANISER";
        } else if (currentDetailType == "Day of Week") {
            queryStr = "SELECT TO_CHAR(DATEM, 'Day') as WEEKDAY, COUNT(*) FROM AHMED.MEETING GROUP BY TO_CHAR(DATEM, 'Day') ORDER BY MIN(TO_CHAR(DATEM, 'D'))";
        } else {
            // Default to duration
            queryStr = "SELECT CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END AS RANGE, COUNT(*) "
                      "FROM AHMED.MEETING GROUP BY CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END";
        }
        
        qDebug() << "Executing query: " << queryStr;
        
        // Prepare the query before executing to avoid ODBC errors
        if (!query.prepare(queryStr)) {
            throw std::runtime_error("Failed to prepare query: " + query.lastError().text().toStdString());
        }
        
        if (!query.exec()) {
            throw std::runtime_error("Query execution failed: " + query.lastError().text().toStdString());
        }
        
        if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Meetings");
            QStringList categories;
            int maxValue = 0;
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty
                if (category.isEmpty()) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value, using 0 instead";
                    count = 0;
                }
                
                // Trim day of week names if applicable
                if (currentDetailType == "Day of Week") {
                    category = category.trimmed();
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                *set << count;
                categories << category;
                maxValue = qMax(maxValue, count);
                total += count;
                dataAvailable = true;
            }
            
            if (dataAvailable) {
                series->append(set);
                currentChart->addSeries(series);
                
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                axisX->append(categories);
                currentChart->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                
                QValueAxis *axisY = new QValueAxis();
                axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 1); // Add 10% padding
                axisY->setTitleText("Number of Meetings");
                axisY->setLabelFormat("%d");
                currentChart->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                
                currentBarSet = set;
                connect(set, &QBarSet::hovered, this, &ChartWindow::handleBarHovered);
                
                // Animate the bars
                animateBarChart(series);
            }
        } else { // Pie Chart
            QPieSeries *series = new QPieSeries();
            
            // Re-execute query for pie chart
            if (!query.prepare(queryStr)) {
                throw std::runtime_error("Failed to prepare pie chart query: " + query.lastError().text().toStdString());
            }
            
            if (!query.exec()) {
                throw std::runtime_error("Pie chart query execution failed: " + query.lastError().text().toStdString());
            }
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty
                if (category.isEmpty()) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value, using 0 instead";
                    count = 0;
                }
                
                // Trim day of week names if applicable
                if (currentDetailType == "Day of Week") {
                    category = category.trimmed();
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Only add non-zero slices to avoid division by zero in percentage calculations
                if (count > 0) {
                    series->append(category, count);
                    total += count;
                    dataAvailable = true;
                }
            }
            
            if (dataAvailable) {
                currentChart->addSeries(series);
                
                // Set up pie slices with labels and percentages
                for (QPieSlice *slice : series->slices()) {
                    slice->setLabel(QString("%1: %2 (%3%)")
                                  .arg(slice->label())
                                  .arg(static_cast<int>(slice->value()))
                                  .arg(slice->percentage() * 100, 0, 'f', 1));
                    slice->setLabelVisible(true);
                    slice->setLabelPosition(QPieSlice::LabelOutside);
                    slice->setBorderWidth(1);
                    slice->setBorderColor(Qt::white);
                    
                    connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                        handlePieSliceHovered(slice, state);
                    });
                }
                
                // Animate the pie
                animatePieChart(series);
            }
        }
        
        if (!dataAvailable) {
            qDebug() << "No data available, adding placeholder text";
            QGraphicsTextItem *textItem = new QGraphicsTextItem("No meeting data available", currentChart);
            textItem->setPos(50, 50);
            if (currentChart->scene()) {
                currentChart->scene()->addItem(textItem);
            } else {
                qDebug() << "Error: Chart scene is null";
            }
        } else {
            // Update total display
            ui->percentageLabel->setText(QString("Total Meetings: %1").arg(total));
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error in createMeetingStatistics:" << e.what();
        QMessageBox::warning(this, "Chart Error", 
            QString("Failed to create meeting statistics: %1").arg(e.what()));
        
        // Add error message to chart
        QGraphicsTextItem *textItem = new QGraphicsTextItem(
            QString("Error: %1").arg(e.what()), currentChart);
        textItem->setPos(50, 50);
        if (currentChart->scene()) {
            currentChart->scene()->addItem(textItem);
        }
    }
}

void ChartWindow::animatePieChart(QPieSeries *series)
{
    // Make sure we cleanly disconnect and delete any previous animation
    if (animationGroup) {
        animationGroup->stop();
        disconnect(animationGroup, &QSequentialAnimationGroup::finished, this, &ChartWindow::onAnimationFinished);
        delete animationGroup;
        animationGroup = nullptr;
    }
    
    // Safety check for empty or null series
    if (!series || series->slices().isEmpty()) {
        qDebug() << "Cannot animate: series is null or empty";
        QTimer::singleShot(100, this, &ChartWindow::onAnimationFinished);
        return;
    }
    
    // Create a parallel animation group instead of sequential for better performance
    animationGroup = new QSequentialAnimationGroup(this);
    
    try {
        for (QPieSlice *slice : series->slices()) {
            if (!slice) continue;
            
            // Store the target angle span for animation
            qreal targetAngleSpan = slice->angleSpan();
            
            // Reset the angle span to 0 for animation starting point
            // Use setProperty instead of direct setter methods
            slice->setProperty("angleSpan", 0.0);
            
            QPropertyAnimation *animation = new QPropertyAnimation(slice, "angleSpan");
            animation->setDuration(300); // Shorter duration for stability
            animation->setStartValue(0.0);
            animation->setEndValue(targetAngleSpan);
            animation->setEasingCurve(QEasingCurve::OutCubic); // More stable curve
            
            animationGroup->addAnimation(animation);
        }
        
        // Use a safer connection and start the animation
        connect(animationGroup, &QSequentialAnimationGroup::finished, 
                this, &ChartWindow::onAnimationFinished, Qt::UniqueConnection);
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    }
    catch (const std::exception& e) {
        qDebug() << "Exception in animatePieChart:" << e.what();
        // Ensure we don't have a partially initialized animation
        if (animationGroup) {
            delete animationGroup;
            animationGroup = nullptr;
        }
        QTimer::singleShot(100, this, &ChartWindow::onAnimationFinished);
    }
}

void ChartWindow::animateBarChart(QBarSeries *series)
{
    Q_UNUSED(series);
    
    // Clean up existing animations
    if (animationGroup) {
        animationGroup->stop();
        disconnect(animationGroup, &QSequentialAnimationGroup::finished, this, &ChartWindow::onAnimationFinished);
        delete animationGroup;
        animationGroup = nullptr;
    }
    
    // Instead of trying to animate bars directly, just let the chart's built-in animation handle it
    // and trigger our callback when it's likely finished
    QTimer::singleShot(500, this, &ChartWindow::onAnimationFinished);
}

void ChartWindow::onAnimationFinished()
{
    qDebug() << "Animation finished";
    updatePercentageLabels();
}

void ChartWindow::updatePercentageLabels()
{
    // Update or show additional percentage information if needed
    // This could be enhanced further
}

void ChartWindow::on_refreshButton_clicked()
{
    updateChart();
}

void ChartWindow::on_resetButton_clicked()
{
    // Reset to default view
    ui->statsTypeComboBox->setCurrentIndex(0);
    ui->chartTypeComboBox->setCurrentIndex(0);
    ui->chartDetailComboBox->blockSignals(true);
    ui->chartDetailComboBox->clear();
    ui->chartDetailComboBox->addItems(getClientDetailOptions());
    if (ui->chartDetailComboBox->count() > 0) {
        ui->chartDetailComboBox->setCurrentIndex(0);
    }
    ui->chartDetailComboBox->blockSignals(false);
    
    updateChart();
}

void ChartWindow::handlePieSliceHovered(QPieSlice *slice, bool state)
{
    if (slice) {
        slice->setExploded(state);
        slice->setLabelVisible(true);
        
        if (state) {
            tooltipLabel->setText(QString("%1: %2 (%3%)")
                                  .arg(slice->label())
                                  .arg(static_cast<int>(slice->value()))
                                  .arg(slice->percentage() * 100, 0, 'f', 1));
            QPoint pos = QCursor::pos();
            pos.setY(pos.y() + 20);
            tooltipLabel->move(pos);
            tooltipLabel->adjustSize();
            tooltipLabel->show();
        } else {
            tooltipLabel->hide();
        }
    }
}

void ChartWindow::handleBarHovered(bool status, int index)
{
    if (status && currentBarSet) {
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(currentChart->axes(Qt::Horizontal).first());
        if (axisX && index < axisX->categories().size()) {
            QString category = axisX->categories().at(index);
            qreal value = currentBarSet->at(index);
            
            // Calculate percentage
            qreal total = 0;
            for (int i = 0; i < currentBarSet->count(); i++) {
                total += currentBarSet->at(i);
            }
            
            qreal percentage = (value / total) * 100;
            
            tooltipLabel->setText(QString("%1: %2 (%3%)")
                                .arg(category)
                                .arg(value)
                                .arg(percentage, 0, 'f', 1));
            QPoint pos = QCursor::pos();
            pos.setY(pos.y() + 20);
            tooltipLabel->move(pos);
            tooltipLabel->adjustSize();
            tooltipLabel->show();
        }
    } else {
        tooltipLabel->hide();
    }
}
