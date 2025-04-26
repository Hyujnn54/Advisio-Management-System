#include "chartwindow.h"
#include "ui_ChartWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QDebug>
#include <QTimer>

ChartWindow::ChartWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentChart(nullptr),
    tooltipLabel(new QLabel(this)),
    currentBarSet(nullptr),
    currentDetailType("")
{
    ui->setupUi(this);

    // Set up tooltip label for hover effects
    tooltipLabel->setStyleSheet("QLabel { background-color: white; border: 1px solid black; padding: 5px; border-radius: 5px; }");
    tooltipLabel->hide();

    // Block signals during initialization
    ui->statsTypeComboBox->blockSignals(true);
    ui->chartDetailComboBox->blockSignals(true);

    // Connect signals to slots
    connect(ui->resetButton, &QPushButton::clicked, this, &ChartWindow::on_resetButton_clicked);
    
    // Update chart detail options when stats type changes
    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
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
    connect(ui->filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::on_filterComboBox_currentIndexChanged);

    // Re-enable signals after initialization
    ui->statsTypeComboBox->blockSignals(false);
    ui->chartDetailComboBox->blockSignals(false);
    
    // Set default detail options
    ui->chartDetailComboBox->addItems(getClientDetailOptions());
    
    // Populate available statistics types
    ui->statsTypeComboBox->clear();
    ui->statsTypeComboBox->addItems({"Client Statistics", "Training Statistics", "Meeting Statistics"});
    
    // Initial chart refresh
    QTimer::singleShot(100, this, &ChartWindow::updateChart);
}

ChartWindow::~ChartWindow()
{
    if (tooltipLabel) {
        delete tooltipLabel;
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

void ChartWindow::on_filterComboBox_currentIndexChanged(int index)
{
    if (index >= 0) {
        currentFilter = ui->filterComboBox->currentText();
        qDebug() << "Filter changed to:" << currentFilter;
        updateChart();
    }
}

void ChartWindow::updateFilterOptions()
{
    QString statsType = ui->statsTypeComboBox->currentText();
    QString detailType = ui->chartDetailComboBox->currentText();
    
    // Block signals during update
    ui->filterComboBox->blockSignals(true);
    ui->filterComboBox->clear();
    
    // Always add "All Data" as the first option
    ui->filterComboBox->addItem("All Data");
    
    // Get filter options based on the current stats type and detail type
    QStringList filterOptions = getFilterOptions(statsType, detailType);
    if (!filterOptions.isEmpty()) {
        ui->filterComboBox->addItems(filterOptions);
    }
    
    // Re-enable signals and set default selection
    ui->filterComboBox->blockSignals(false);
    ui->filterComboBox->setCurrentIndex(0);
}

QStringList ChartWindow::getFilterOptions(QString statsType, QString detailType)
{
    QStringList options;
    QSqlQuery query;
    
    try {
        // Build query to get filter options based on stats and detail type
        QString queryStr;
        
        if (statsType == "Client Statistics") {
            if (detailType == "Sector") {
                queryStr = "SELECT DISTINCT CONSULTANT_ID FROM CLIENTS";
            } else if (detailType == "Consultation Month") {
                queryStr = "SELECT DISTINCT strftime('%Y', CONSULTATION_DATE) FROM CLIENTS ORDER BY strftime('%Y', CONSULTATION_DATE)";
            } else if (detailType == "Consultant") {
                queryStr = "SELECT DISTINCT SECTOR FROM CLIENTS WHERE SECTOR IS NOT NULL AND SECTOR <> ''";
            }
        } else if (statsType == "Training Statistics") {
            if (detailType == "Formation Type") {
                queryStr = "SELECT DISTINCT TRAINER FROM FORMATIONS WHERE TRAINER IS NOT NULL AND TRAINER <> ''";
            } else if (detailType == "Trainer") {
                queryStr = "SELECT DISTINCT FORMATION FROM FORMATIONS WHERE FORMATION IS NOT NULL AND FORMATION <> ''";
            } else if (detailType == "Price Range" || detailType == "Month") {
                queryStr = "SELECT DISTINCT strftime('%Y', DATEF) FROM FORMATIONS ORDER BY strftime('%Y', DATEF)";
            }
        } else if (statsType == "Meeting Statistics") {
            if (detailType == "Duration" || detailType == "Time of Day" || detailType == "Day of Week") {
                queryStr = "SELECT DISTINCT ORGANISER FROM MEETING WHERE ORGANISER IS NOT NULL AND ORGANISER <> ''";
            } else if (detailType == "Organiser") {
                queryStr = "SELECT DISTINCT strftime('%Y', DATEM) FROM MEETING ORDER BY strftime('%Y', DATEM)";
            }
        }
        
        // If we have a valid query, execute it and get filter options
        if (!queryStr.isEmpty()) {
            if (query.exec(queryStr)) {
                while (query.next()) {
                    QString option = query.value(0).toString();
                    if (!option.isEmpty()) {
                        options << option;
                    }
                }
            } else {
                qDebug() << "Filter options query error:" << query.lastError().text();
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Error getting filter options:" << e.what();
    }
    
    return options;
}

QString ChartWindow::applyFilterToQuery(QString baseQuery, QString statsType, QString detailType, QString filter)
{
    // If no filter or "All Data" is selected, return the base query
    if (filter.isEmpty() || filter == "All Data") {
        return baseQuery;
    }
    
    // Add WHERE clause or AND condition based on the stats type and detail type
    if (statsType == "Client Statistics") {
        if (detailType == "Sector") {
            return baseQuery + " WHERE CONSULTANT_ID = '" + filter + "'";
        } else if (detailType == "Consultation Month") {
            return baseQuery + " WHERE strftime('%Y', CONSULTATION_DATE) = '" + filter + "'";
        } else if (detailType == "Consultant") {
            return baseQuery + " WHERE SECTOR = '" + filter + "'";
        }
    } else if (statsType == "Training Statistics") {
        if (detailType == "Formation Type") {
            return baseQuery + " WHERE TRAINER = '" + filter + "'";
        } else if (detailType == "Trainer") {
            return baseQuery + " WHERE FORMATION = '" + filter + "'";
        } else if (detailType == "Price Range" || detailType == "Month") {
            return baseQuery + " WHERE strftime('%Y', DATEF) = '" + filter + "'";
        }
    } else if (statsType == "Meeting Statistics") {
        if (detailType == "Duration" || detailType == "Time of Day" || detailType == "Day of Week") {
            return baseQuery + " WHERE ORGANISER = '" + filter + "'";
        } else if (detailType == "Organiser") {
            return baseQuery + " WHERE strftime('%Y', DATEM) = '" + filter + "'";
        }
    }
    
    // If no specific filter conditions match, return the base query
    return baseQuery;
}

void ChartWindow::updateChart()
{
    QString statsType = ui->statsTypeComboBox->currentText();
    QString chartType = ui->chartTypeComboBox->currentText();
    currentDetailType = ui->chartDetailComboBox->currentText();

    qDebug() << "======== CHART DEBUG ========";
    qDebug() << "Starting updateChart() with statsType:" << statsType 
             << ", chartType:" << chartType 
             << ", detailType:" << currentDetailType;

    // Clear percentage label
    ui->percentageLabel->clear();
    
    try {
        // Verify database connection first
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isOpen()) {
            qDebug() << "Error: Database connection is not open, attempting to reconnect";
            if (!db.open()) {
                throw std::runtime_error("Database connection is closed and could not be reopened");
            }
        }
        
        // Handle empty statsType
        if (statsType.isEmpty()) {
            qDebug() << "Warning: statsType is empty, defaulting to 'Client Statistics'";
            statsType = "Client Statistics";
            ui->statsTypeComboBox->setCurrentIndex(0);
        }
        
        qDebug() << "Creating/clearing chart";
        
        // Create a new chart or reuse the existing one with proper cleanup
        if (!currentChart) {
            currentChart = new QChart();
            qDebug() << "Created new chart instance";
        } else {
            // Properly clear the existing chart
            qDebug() << "Clearing existing chart";
            
            // Disconnect any existing hover signals first
            for (QAbstractSeries *series : currentChart->series()) {
                if (QBarSeries *barSeries = qobject_cast<QBarSeries*>(series)) {
                    for (QBarSet *set : barSeries->barSets()) {
                        set->disconnect(); // Disconnect all signals from this barset
                    }
                } else if (QPieSeries *pieSeries = qobject_cast<QPieSeries*>(series)) {
                    for (QPieSlice *slice : pieSeries->slices()) {
                        slice->disconnect(); // Disconnect all signals from this slice
                    }
                }
            }
            
            currentChart->removeAllSeries();
            
            // Safely remove all axes
            QList<QAbstractAxis*> axes = currentChart->axes();
            qDebug() << "Removing" << axes.size() << "axes";
            for (QAbstractAxis* axis : axes) {
                currentChart->removeAxis(axis);
                delete axis;
            }
            
            // Clear any custom items safely
            if (currentChart->scene()) {
                QList<QGraphicsItem*> items = currentChart->scene()->items();
                qDebug() << "Removing" << items.size() << "scene items";
                for (QGraphicsItem *item : items) {
                    if (item && item->parentItem() == nullptr && 
                        item != currentChart && !qgraphicsitem_cast<QGraphicsProxyWidget*>(item)) {
                        currentChart->scene()->removeItem(item);
                        delete item;
                    }
                }
            }
        }

        // Reset current bar set pointer
        currentBarSet = nullptr;

        qDebug() << "Setting chart properties";
        
        // Set basic chart properties
        currentChart->setTitle(statsType + " - " + currentDetailType);
        currentChart->setAnimationOptions(QChart::SeriesAnimations);
        currentChart->setAnimationDuration(500);
        
        // Check for dark theme
        bool isDarkTheme = detectDarkTheme();
        
        // Apply appropriate theme
        applyTheme(isDarkTheme);
        
        qDebug() << "Creating specific chart type:" << statsType;
        
        // Create the specific chart based on user selection
        if (statsType == "Client Statistics") {
            createClientStatistics(chartType);
        } else if (statsType == "Training Statistics") {
            createTrainingStatistics(chartType);
        } else if (statsType == "Meeting Statistics") {
            createMeetingStatistics(chartType);
        }

        qDebug() << "Setting up chart legend";
        
        // Set up legend with appropriate styling
        if (currentChart->legend()) {
            currentChart->legend()->setVisible(ui->toggleLegendCheckBox->isChecked());
            currentChart->legend()->setAlignment(Qt::AlignBottom);
            
            // Style the legend based on theme
            if (isDarkTheme) {
                currentChart->legend()->setLabelBrush(QBrush(QColor("#E0E0E0")));
                currentChart->legend()->setBorderColor(QColor("#3E3E42"));
            } else {
                currentChart->legend()->setLabelBrush(QBrush(QColor("#333333")));
                currentChart->legend()->setBorderColor(QColor("#D3DCE6"));
            }
        }
        
        // Apply the chart to the view
        if (!ui->statsChartView) {
            qDebug() << "Error: statsChartView is null";
            throw std::runtime_error("Chart view is not available");
        }
        
        qDebug() << "Applying chart to chart view";
        
        // Apply the chart to the view and set rendering options
        ui->statsChartView->setChart(currentChart);
        ui->statsChartView->setRenderHint(QPainter::Antialiasing);
        ui->statsChartView->setRubberBand(QChartView::RectangleRubberBand);
        ui->statsChartView->setRenderHint(QPainter::SmoothPixmapTransform);
        
        qDebug() << "Set chart to QChartView successfully";
        qDebug() << "======== CHART UPDATE COMPLETED ========";
    } catch (const std::exception &e) {
        qDebug() << "EXCEPTION in updateChart:" << e.what();
        // Display error message on chart instead of using QMessageBox to avoid further issues
        if (currentChart && currentChart->scene()) {
            QGraphicsTextItem* errorText = new QGraphicsTextItem("Error updating chart: " + QString(e.what()), currentChart);
            errorText->setPos(50, 50);
            currentChart->scene()->addItem(errorText);
            
            // Make sure it's visible in the chart view
            if (ui->statsChartView) {
                ui->statsChartView->setChart(currentChart);
            }
        } else {
            // Only use message box as a last resort
            QMessageBox::warning(this, "Chart Error", "An error occurred while updating the chart: " + QString(e.what()));
        }
    } catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in updateChart";
        if (currentChart && currentChart->scene()) {
            QGraphicsTextItem* errorText = new QGraphicsTextItem("An unknown error occurred while updating the chart", currentChart);
            errorText->setPos(50, 50);
            currentChart->scene()->addItem(errorText);
            
            if (ui->statsChartView) {
                ui->statsChartView->setChart(currentChart);
            }
        } else {
            QMessageBox::warning(this, "Chart Error", "An unknown error occurred while updating the chart");
        }
    }
}

void ChartWindow::createClientStatistics(QString chartType)
{
    QSqlQuery query;
    bool dataAvailable = false;
    int total = 0;
    QString queryStr;
    
    try {
        // Build the query based on detail type and database compatibility
        if (currentDetailType == "Sector") {
            // Use a more database-agnostic approach for handling NULL values
            queryStr = "SELECT SECTOR, COUNT(*) FROM CLIENTS GROUP BY SECTOR";
        } else if (currentDetailType == "Consultation Month") {
            // Simplify date extraction for better compatibility
            queryStr = "SELECT strftime('%m', CONSULTATION_DATE) as MONTH, COUNT(*) FROM CLIENTS GROUP BY strftime('%m', CONSULTATION_DATE) ORDER BY MONTH";
        } else if (currentDetailType == "Consultant") {
            // Simplify the join query
            queryStr = "SELECT e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME, COUNT(*) "
                       "FROM CLIENTS c "
                       "LEFT JOIN EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                       "GROUP BY e.FIRST_NAME, e.LAST_NAME";
        } else {
            // Default to sector
            queryStr = "SELECT SECTOR, COUNT(*) FROM CLIENTS GROUP BY SECTOR";
        }
        
        qDebug() << "Executing query: " << queryStr;
        
        // Execute the query directly with error checking
        if (!query.exec(queryStr)) {
            qDebug() << "Query error: " << query.lastError().text();
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
                
                // Provide a default if category is empty or null
                if (category.isEmpty() || query.isNull(0)) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value for category " << category << ", using 0 instead";
                    count = 0;
                }
                
                // Add month names for better readability if needed
                if (currentDetailType == "Consultation Month") {
                    QStringList monthNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                    bool isNumeric;
                    int monthNum = category.toInt(&isNumeric);
                    if (isNumeric && monthNum >= 1 && monthNum <= 12) {
                        category = monthNames[monthNum - 1];
                    }
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
            }
        } else { // Pie Chart
            QPieSeries *series = new QPieSeries();
            
            // Reset the query to start from the beginning
            query.exec(queryStr);
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty or null
                if (category.isEmpty() || query.isNull(0)) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value for category " << category << ", using 0 instead";
                    count = 0;
                }
                
                // Add month names for better readability if needed
                if (currentDetailType == "Consultation Month") {
                    QStringList monthNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                    bool isNumeric;
                    int monthNum = category.toInt(&isNumeric);
                    if (isNumeric && monthNum >= 1 && monthNum <= 12) {
                        category = monthNames[monthNum - 1];
                    }
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Only add non-zero slices
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
                    
                    connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                        handlePieSliceHovered(slice, state);
                    });
                }
            }
        }
        
        if (!dataAvailable) {
            qDebug() << "No data available, adding placeholder text";
            QGraphicsTextItem *textItem = new QGraphicsTextItem("No client data available", currentChart);
            textItem->setPos(50, 50);
            if (currentChart->scene()) {
                currentChart->scene()->addItem(textItem);
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
        // Build the query based on detail type and database compatibility
        if (currentDetailType == "Formation Type") {
            queryStr = "SELECT FORMATION, COUNT(*) FROM FORMATIONS GROUP BY FORMATION";
        } else if (currentDetailType == "Trainer") {
            queryStr = "SELECT TRAINER, COUNT(*) FROM FORMATIONS GROUP BY TRAINER";
        } else if (currentDetailType == "Price Range") {
            queryStr = "SELECT CASE "
                      "WHEN PRIX <= 50 THEN 'Low (≤ 50)' "
                      "WHEN PRIX <= 100 THEN 'Medium (51-100)' "
                      "ELSE 'High (> 100)' END AS PRICE_RANGE, COUNT(*) "
                      "FROM FORMATIONS GROUP BY CASE "
                      "WHEN PRIX <= 50 THEN 'Low (≤ 50)' "
                      "WHEN PRIX <= 100 THEN 'Medium (51-100)' "
                      "ELSE 'High (> 100)' END";
        } else if (currentDetailType == "Month") {
            queryStr = "SELECT strftime('%m', DATEF) as MONTH, COUNT(*) FROM FORMATIONS GROUP BY strftime('%m', DATEF) ORDER BY MONTH";
        } else {
            // Default to formation type
            queryStr = "SELECT FORMATION, COUNT(*) FROM FORMATIONS GROUP BY FORMATION";
        }
        
        qDebug() << "Executing query: " << queryStr;
        
        // Execute the query directly with error checking
        if (!query.exec(queryStr)) {
            qDebug() << "Query error: " << query.lastError().text();
            throw std::runtime_error("Query execution failed: " + query.lastError().text().toStdString());
        }

        if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Trainings");
            QStringList categories;
            int maxValue = 0;
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty or null
                if (category.isEmpty() || query.isNull(0)) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value for category " << category << ", using 0 instead";
                    count = 0;
                }
                
                // Add month names for better readability if needed
                if (currentDetailType == "Month") {
                    QStringList monthNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                    bool isNumeric;
                    int monthNum = category.toInt(&isNumeric);
                    if (isNumeric && monthNum >= 1 && monthNum <= 12) {
                        category = monthNames[monthNum - 1];
                    }
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
            }
        } else { // Pie Chart
            QPieSeries *series = new QPieSeries();
            
            // Reset the query to start from the beginning
            query.exec(queryStr);
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty or null
                if (category.isEmpty() || query.isNull(0)) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value for category " << category << ", using 0 instead";
                    count = 0;
                }
                
                // Add month names for better readability if needed
                if (currentDetailType == "Month") {
                    QStringList monthNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                    bool isNumeric;
                    int monthNum = category.toInt(&isNumeric);
                    if (isNumeric && monthNum >= 1 && monthNum <= 12) {
                        category = monthNames[monthNum - 1];
                    }
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Only add non-zero slices
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
                    
                    connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                        handlePieSliceHovered(slice, state);
                    });
                }
            }
        }
        
        if (!dataAvailable) {
            qDebug() << "No data available, adding placeholder text";
            QGraphicsTextItem *textItem = new QGraphicsTextItem("No training data available", currentChart);
            textItem->setPos(50, 50);
            if (currentChart->scene()) {
                currentChart->scene()->addItem(textItem);
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
        // Build the query based on detail type and database compatibility
        if (currentDetailType == "Duration") {
            queryStr = "SELECT CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END AS RANGE, COUNT(*) "
                      "FROM MEETING GROUP BY CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END";
        } else if (currentDetailType == "Time of Day") {
            queryStr = "SELECT CASE "
                      "WHEN strftime('%H', DATEM) < '12' THEN 'Morning' "
                      "WHEN strftime('%H', DATEM) < '17' THEN 'Afternoon' "
                      "ELSE 'Evening' END AS TIME_OF_DAY, COUNT(*) "
                      "FROM MEETING GROUP BY CASE "
                      "WHEN strftime('%H', DATEM) < '12' THEN 'Morning' "
                      "WHEN strftime('%H', DATEM) < '17' THEN 'Afternoon' "
                      "ELSE 'Evening' END";
        } else if (currentDetailType == "Organiser") {
            queryStr = "SELECT ORGANISER, COUNT(*) FROM MEETING GROUP BY ORGANISER";
        } else if (currentDetailType == "Day of Week") {
            queryStr = "SELECT strftime('%w', DATEM) as WEEKDAY, COUNT(*) FROM MEETING GROUP BY strftime('%w', DATEM) ORDER BY WEEKDAY";
        } else {
            // Default to duration
            queryStr = "SELECT CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END AS RANGE, COUNT(*) "
                      "FROM MEETING GROUP BY CASE "
                      "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                      "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                      "ELSE 'Long (>60 mins)' END";
        }
        
        qDebug() << "Executing query: " << queryStr;
        
        // Execute the query directly with error checking
        if (!query.exec(queryStr)) {
            qDebug() << "Query error: " << query.lastError().text();
            throw std::runtime_error("Query execution failed: " + query.lastError().text().toStdString());
        }
        
        if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Meetings");
            QStringList categories;
            int maxValue = 0;
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty or null
                if (category.isEmpty() || query.isNull(0)) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value for category " << category << ", using 0 instead";
                    count = 0;
                }
                
                // Add day names for better readability if needed
                if (currentDetailType == "Day of Week") {
                    QStringList dayNames = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
                    bool isNumeric;
                    int dayNum = category.toInt(&isNumeric);
                    if (isNumeric && dayNum >= 0 && dayNum <= 6) {
                        category = dayNames[dayNum];
                    }
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
            }
        } else { // Pie Chart
            QPieSeries *series = new QPieSeries();
            
            // Reset the query to start from the beginning
            query.exec(queryStr);
            
            while (query.next()) {
                QString category = query.value(0).toString();
                
                // Provide a default if category is empty or null
                if (category.isEmpty() || query.isNull(0)) {
                    category = "Not Specified";
                }
                
                // Process count values safely
                bool ok;
                int count = query.value(1).toInt(&ok);
                if (!ok) {
                    qDebug() << "Warning: Invalid count value for category " << category << ", using 0 instead";
                    count = 0;
                }
                
                // Add day names for better readability if needed
                if (currentDetailType == "Day of Week") {
                    QStringList dayNames = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
                    bool isNumeric;
                    int dayNum = category.toInt(&isNumeric);
                    if (isNumeric && dayNum >= 0 && dayNum <= 6) {
                        category = dayNames[dayNum];
                    }
                }
                
                qDebug() << "Category:" << category << "Count:" << count;
                
                // Only add non-zero slices
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
                    
                    connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                        handlePieSliceHovered(slice, state);
                    });
                }
            }
        }
        
        if (!dataAvailable) {
            qDebug() << "No data available, adding placeholder text";
            QGraphicsTextItem *textItem = new QGraphicsTextItem("No meeting data available", currentChart);
            textItem->setPos(50, 50);
            if (currentChart->scene()) {
                currentChart->scene()->addItem(textItem);
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

void ChartWindow::handlePieSliceHovered(QPieSlice *slice, bool state)
{
    qDebug() << "Pie slice hover - state:" << state << "slice:" << (slice ? slice->label() : "nullptr");
    
    // Safety check for null slice
    if (!slice) {
        qDebug() << "Warning: null slice in handlePieSliceHovered";
        tooltipLabel->hide();
        return;
    }
    
    // Set the exploded state of the slice
    slice->setExploded(state);
    
    if (state) {
        // Format the tooltip text with slice information
        tooltipLabel->setText(QString("%1: %2 (%3%)")
                          .arg(slice->label())
                          .arg(static_cast<int>(slice->value()))
                          .arg(slice->percentage() * 100, 0, 'f', 1));
        
        // Position tooltip near the cursor
        QPoint pos = QCursor::pos();
        pos.setY(pos.y() + 20);
        tooltipLabel->move(pos);
        tooltipLabel->adjustSize();
        tooltipLabel->show();
        
        qDebug() << "Showing tooltip for slice:" << slice->label() << "value:" << slice->value();
    } else {
        tooltipLabel->hide();
    }
}

void ChartWindow::handleBarHovered(bool status, int index)
{
    qDebug() << "Bar hover - status:" << status << "index:" << index;
    
    // Safety check for null/invalid currentBarSet
    if (!currentBarSet) {
        qDebug() << "Warning: currentBarSet is null in handleBarHovered";
        tooltipLabel->hide();
        return;
    }
    
    // Only show tooltip when hovering over a bar (status=true)
    if (status) {
        // Check for valid horizontal axis
        QList<QAbstractAxis*> horizontalAxes = currentChart->axes(Qt::Horizontal);
        if (horizontalAxes.isEmpty()) {
            qDebug() << "Warning: No horizontal axis found";
            return;
        }
        
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(horizontalAxes.first());
        if (!axisX) {
            qDebug() << "Warning: Horizontal axis is not a QBarCategoryAxis";
            return;
        }
        
        // Validate index is within bounds
        if (index < 0 || index >= axisX->categories().size() || index >= currentBarSet->count()) {
            qDebug() << "Warning: Bar hover index out of bounds:" << index 
                     << "categories:" << axisX->categories().size()
                     << "barset size:" << currentBarSet->count();
            return;
        }
        
        QString category = axisX->categories().at(index);
        qreal value = currentBarSet->at(index);
        
        // Calculate percentage with safeguards
        qreal total = 0;
        for (int i = 0; i < currentBarSet->count(); i++) {
            total += currentBarSet->at(i);
        }
        
        qreal percentage = (total > 0) ? ((value / total) * 100) : 0;
        
        // Update tooltip with formatted information
        tooltipLabel->setText(QString("%1: %2 (%3%)")
                            .arg(category)
                            .arg(value)
                            .arg(percentage, 0, 'f', 1));
        
        // Position tooltip near the cursor
        QPoint pos = QCursor::pos();
        pos.setY(pos.y() + 20);
        tooltipLabel->move(pos);
        tooltipLabel->adjustSize();
        tooltipLabel->show();
        
        qDebug() << "Showing tooltip for bar:" << category << "value:" << value;
    } else {
        tooltipLabel->hide();
    }
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

bool ChartWindow::detectDarkTheme()
{
    // Check if parent window is using dark theme
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(parentWidget());
    if (mainWindow) {
        // Look for the theme button in the parent window
        QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
        for (auto button : buttons) {
            if (button->objectName() == "themeButton" && button->text() == "Light Theme") {
                return true;
            }
        }
    }
    return false;
}

void ChartWindow::updateTooltipStyle()
{
    if (isDarkTheme) {
        tooltipLabel->setStyleSheet("QLabel { background-color: #252526; color: #E0E0E0; border: 1px solid #3E3E42; padding: 5px; border-radius: 5px; }");
    } else {
        tooltipLabel->setStyleSheet("QLabel { background-color: white; color: #333333; border: 1px solid #d3d3d3; padding: 5px; border-radius: 5px; }");
    }
}

void ChartWindow::applyTheme(bool darkTheme)
{
    isDarkTheme = darkTheme;
    
    // Apply theme to the chart window
    if (isDarkTheme) {
        // Dark theme
        setStyleSheet(R"(
            QMainWindow { background-color: #252526; }
            QGroupBox { 
                font-weight: bold; 
                border: 1px solid #3E3E42; 
                border-radius: 5px; 
                margin-top: 10px; 
                color: #E0E0E0;
            }
            QGroupBox::title { 
                subcontrol-origin: margin; 
                subcontrol-position: top left; 
                padding: 0 5px; 
                color: #E0E0E0; 
            }
            QComboBox { 
                padding: 5px; 
                border: 1px solid #3E3E42; 
                border-radius: 3px; 
                background-color: #3E3E42; 
                color: #E0E0E0;
            }
            QComboBox:hover { border: 1px solid #007ACC; }
            QPushButton { 
                padding: 5px 10px; 
                border: 1px solid #0E639C; 
                border-radius: 3px; 
                background-color: #0E639C; 
                color: white;
            }
            QPushButton:hover { background-color: #1177BB; }
            QCheckBox { 
                padding: 5px; 
                color: #E0E0E0;
            }
            QLabel { 
                color: #E0E0E0;
            }
            QLabel#hoverDescriptionLabel { 
                background-color: #2D2D30; 
                border-radius: 3px; 
                padding: 5px; 
                font-style: italic; 
                color: #E0E0E0;
            }
            QLabel#percentageLabel {
                font-weight: bold;
                font-size: 14px;
                color: #E0E0E0;
                background-color: rgba(45, 45, 48, 0.7);
                border-radius: 5px;
                padding: 5px;
            }
        )");
        
        // Apply dark theme to the chart if it exists
        if (currentChart) {
            currentChart->setTheme(QChart::ChartThemeDark);
            currentChart->setBackgroundBrush(QBrush(QColor("#252526")));
            currentChart->setTitleBrush(QBrush(QColor("#E0E0E0")));
            
            if (currentChart->legend()) {
                currentChart->legend()->setLabelBrush(QBrush(QColor("#E0E0E0")));
                currentChart->legend()->setBorderColor(QColor("#3E3E42"));
            }
        }
    } else {
        // Light theme
        setStyleSheet(R"(
            QMainWindow { background-color: #F5F5F5; }
            QGroupBox { 
                font-weight: bold; 
                border: 1px solid #D3D3D3; 
                border-radius: 5px; 
                margin-top: 10px; 
                color: #333333;
            }
            QGroupBox::title { 
                subcontrol-origin: margin; 
                subcontrol-position: top left; 
                padding: 0 5px; 
                color: #333333; 
            }
            QComboBox { 
                padding: 5px; 
                border: 1px solid #D3D3D3; 
                border-radius: 3px; 
                background-color: white; 
                color: #333333;
            }
            QComboBox:hover { border: 1px solid #A0A0A0; }
            QPushButton { 
                padding: 5px 10px; 
                border: 1px solid #D3D3D3; 
                border-radius: 3px; 
                background-color: #3A5DAE; 
                color: white;
            }
            QPushButton:hover { background-color: #4A70C2; }
            QCheckBox { 
                padding: 5px; 
                color: #333333;
            }
            QLabel { 
                color: #333333;
            }
            QLabel#hoverDescriptionLabel { 
                background-color: #E8E8E8; 
                border-radius: 3px; 
                padding: 5px; 
                font-style: italic; 
                color: #333333;
            }
            QLabel#percentageLabel {
                font-weight: bold;
                font-size: 14px;
                color: #333333;
                background-color: rgba(255, 255, 255, 0.7);
                border-radius: 5px;
                padding: 5px;
            }
        )");
        
        // Apply light theme to the chart if it exists
        if (currentChart) {
            currentChart->setTheme(QChart::ChartThemeLight);
            currentChart->setBackgroundBrush(QBrush(QColor("#FFFFFF")));
            currentChart->setTitleBrush(QBrush(QColor("#333333")));
            
            if (currentChart->legend()) {
                currentChart->legend()->setLabelBrush(QBrush(QColor("#333333")));
                currentChart->legend()->setBorderColor(QColor("#D3DCE6"));
            }
        }
    }
    
    // Update tooltip style
    updateTooltipStyle();
    
    // Force chart update if needed
    if (currentChart) {
        updateChart();
    }
}

void ChartWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    
    // When window is resized, ensure chart adapts properly
    if (currentChart && ui->statsChartView) {
        ui->statsChartView->resize(ui->statsChartView->parentWidget()->size());
        
        // If window is very large, adjust chart margins for better readability
        if (event->size().width() > 1200) {
            currentChart->setMargins(QMargins(20, 20, 20, 20));
            // QLegend doesn't have setMargins method, using proper spacing instead
            if (currentChart->legend()) {
                currentChart->legend()->setAlignment(Qt::AlignBottom);
            }
        } else {
            currentChart->setMargins(QMargins(10, 10, 10, 10));
            if (currentChart->legend()) {
                currentChart->legend()->setAlignment(Qt::AlignBottom);
            }
        }
    }
    
    // Adjust tooltip size based on window size
    if (tooltipLabel) {
        int fontSize = event->size().width() > 1200 ? 12 : 10;
        tooltipLabel->setStyleSheet(tooltipLabel->styleSheet() + QString("font-size: %1pt;").arg(fontSize));
    }
}

void ChartWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    
    // Detect and apply the appropriate theme when shown
    isDarkTheme = detectDarkTheme();
    applyTheme(isDarkTheme);
    
    // Update the chart
    QTimer::singleShot(100, this, &ChartWindow::updateChart);
}
