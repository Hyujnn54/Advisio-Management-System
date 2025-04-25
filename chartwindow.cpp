#include "ChartWindow.h"
#include "ui_ChartWindow.h"
#include <QDebug>

ChartWindow::ChartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentBarSet(nullptr),
    hoveredBarIndex(-1)
{
    ui->setupUi(this);

    QSqlQueryModel *testModel = Etmp.afficher();
    qDebug() << "Initial Client Data Row Count:" << testModel->rowCount();
    testModel = Ttmp.afficher();
    qDebug() << "Initial Training Data Row Count:" << testModel->rowCount();

    connect(ui->dataTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::on_dataTypeComboBox_currentIndexChanged);
    connect(ui->refreshChartButton, &QPushButton::clicked, this, &ChartWindow::on_refreshChartButton_clicked);
    connect(ui->statsFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::on_statsFilterComboBox_currentIndexChanged);
    connect(ui->chartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::on_chartTypeComboBox_currentIndexChanged);
    connect(ui->resetChartButton, &QPushButton::clicked, this, &ChartWindow::on_resetChartButton_clicked);

    currentChartType = ui->chartTypeComboBox->currentText();
    populateFilterValues();
    updateChart();
}

ChartWindow::~ChartWindow()
{
    delete ui;
}

void ChartWindow::updateChart()
{
    QString dataType = ui->dataTypeComboBox->currentText();
    QString filterType = ui->statsFilterComboBox->currentText();
    QString filterValue = ui->filterValueComboBox->currentText();
    qDebug() << "Data Type:" << dataType << "Filter Type:" << filterType << "Filter Value:" << filterValue << "Chart Type:" << currentChartType;

    currentDataMap.clear();
    lineDataMap.clear();

    if (dataType == "Clients") {
        if (filterType == "By Sector") {
            QSqlQueryModel *model = filterValue.isEmpty() ? Etmp.afficher() : Etmp.searchBySector(filterValue);
            qDebug() << "Client Sector Model Row Count:" << model->rowCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString sector = model->data(model->index(row, 1)).toString();
                currentDataMap[sector] += 1;
            }
        } else if (filterType == "By Date") {
            QDate date = QDate::fromString(filterValue, "yyyy-MM-dd");
            if (date.isValid()) {
                QSqlQueryModel *model = Etmp.getConsultationsForDate(date);
                qDebug() << "Client Date Model Row Count:" << model->rowCount();
                for (int row = 0; row < model->rowCount(); ++row) {
                    QString name = model->data(model->index(row, 0)).toString();
                    currentDataMap[name] += 1;
                }

                QDate startDate = date.addDays(-7);
                QDate endDate = date;
                for (QDate d = startDate; d <= endDate; d = d.addDays(1)) {
                    QSqlQueryModel *dailyModel = Etmp.getConsultationsForDate(d);
                    lineDataMap[d] = dailyModel->rowCount();
                }
            } else {
                qDebug() << "Invalid date:" << filterValue;
            }
        } else if (filterType == "By Consultant") {
            bool ok;
            int consultantId = filterValue.toInt(&ok);
            if (ok) {
                QSqlQueryModel *model = Etmp.searchByConsultant(QString::number(consultantId));
                qDebug() << "Client Consultant Model Row Count:" << model->rowCount();
                for (int row = 0; row < model->rowCount(); ++row) {
                    QString name = model->data(model->index(row, 0)).toString();
                    currentDataMap[name] += 1;
                }
            } else {
                qDebug() << "Invalid consultant ID:" << filterValue;
            }
        }
    } else if (dataType == "Trainings") {
        if (filterType == "By Formation") {
            QSqlQueryModel *model = filterValue.isEmpty() ? Ttmp.afficher() : Ttmp.searchByType(filterValue);
            qDebug() << "Training Formation Model Row Count:" << model->rowCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString formation = model->data(model->index(row, 1)).toString(); // Column 1 is FORMATION
                currentDataMap[formation] += 1;
            }
        } else if (filterType == "By Date") {
            QDate date = QDate::fromString(filterValue, "yyyy-MM-dd"); // Updated format
            if (date.isValid()) {
                QSqlQueryModel *model = Ttmp.getTrainingsForDate(date);
                qDebug() << "Training Date Model Row Count:" << model->rowCount();
                for (int row = 0; row < model->rowCount(); ++row) {
                    QString formation = model->data(model->index(row, 1)).toString(); // Column 1 is FORMATION
                    currentDataMap[formation] += 1;
                }

                QDate startDate = date.addDays(-7);
                QDate endDate = date;
                for (QDate d = startDate; d <= endDate; d = d.addDays(1)) {
                    QSqlQueryModel *dailyModel = Ttmp.getTrainingsForDate(d);
                    lineDataMap[d] = dailyModel->rowCount();
                }
            } else {
                qDebug() << "Invalid date:" << filterValue;
            }
        } else if (filterType == "By Trainer") {
            QSqlQueryModel *model = Ttmp.searchByTrainer(filterValue); // TRAINER is a QString
            qDebug() << "Training Trainer Model Row Count:" << model->rowCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString formation = model->data(model->index(row, 1)).toString(); // Column 1 is FORMATION
                currentDataMap[formation] += 1;
            }
        }
    }

    qDebug() << "DataMap Contents:" << currentDataMap;
    if (currentDataMap.isEmpty() && lineDataMap.isEmpty()) {
        qDebug() << "Error: DataMap and lineDataMap are empty, no chart will be generated.";
        ui->chartDetailsLabel->setText("No data available");
        return;
    }

    QChart *chart = new QChart();
    QString chartTitle = dataType == "Clients" ? "Number of Consultations per " : "Number of Trainings per ";
    chartTitle += filterType.split(" ").last().toLower();
    chart->setTitle(chartTitle);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor(0, 128, 0, 100)));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(0, 128, 0, 100)));
    chart->setPlotAreaBackgroundVisible(true);

    if (currentChartType == "Bar Chart") {
        qDebug() << "Creating Bar Chart...";
        QBarSeries *series = new QBarSeries();
        currentBarSet = new QBarSet(dataType == "Clients" ? "Consultations" : "Trainings");

        for (auto it = currentDataMap.begin(); it != currentDataMap.end(); ++it) {
            *currentBarSet << it.value();
        }
        currentBarSet->setBrush(QBrush(Qt::blue));
        series->append(currentBarSet);
        qDebug() << "BarSet Count:" << currentBarSet->count();

        chart->addSeries(series);

        QStringList categories = currentDataMap.keys();
        qDebug() << "Categories:" << categories;

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (const int &value : currentDataMap.values()) {
            if (value > maxValue) maxValue = value;
        }
        maxValue = qMax(1, maxValue + 1);
        qDebug() << "Max Value for Y-Axis:" << maxValue;
        axisY->setRange(0, maxValue);
        axisY->setLabelFormat("%d");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        connect(series, &QBarSeries::hovered, this, &ChartWindow::on_barHovered);
    } else if (currentChartType == "Pie Chart") {
        qDebug() << "Creating Pie Chart...";
        QPieSeries *series = new QPieSeries();

        int total = 0;
        for (const int &value : currentDataMap.values()) {
            total += value;
        }

        for (auto it = currentDataMap.begin(); it != currentDataMap.end(); ++it) {
            qreal percentage = (it.value() * 100.0) / total;
            QStringList words = it.key().split(" ");
            QString abbreviation;
            for (const QString &word : words) {
                if (!word.isEmpty()) abbreviation += word[0].toUpper();
            }
            QString label = QString("%1% %2").arg(percentage, 0, 'f', 1).arg(abbreviation);
            QPieSlice *slice = series->append(label, it.value());
            slice->setLabelVisible(true);
        }
        qDebug() << "Pie slices added:" << series->count();

        chart->addSeries(series);

        connect(series, &QPieSeries::hovered, this, &ChartWindow::on_pieSliceHovered);
    } else if (currentChartType == "Line Chart") {
        qDebug() << "Creating Line Chart...";
        QLineSeries *series = new QLineSeries();

        QDateTimeAxis *axisX = new QDateTimeAxis();
        axisX->setFormat("yyyy-MM-dd");
        axisX->setTitleText("Date");

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText(dataType == "Clients" ? "Consultations" : "Trainings");
        axisY->setLabelFormat("%d");

        int maxValue = 0;
        QDateTime minDateTime, maxDateTime;
        bool firstPoint = true;

        for (auto it = lineDataMap.begin(); it != lineDataMap.end(); ++it) {
            QDateTime dateTime(it.key(), QTime(0, 0));
            qint64 timestamp = dateTime.toMSecsSinceEpoch();
            series->append(timestamp, it.value());

            if (it.value() > maxValue) maxValue = it.value();

            if (firstPoint) {
                minDateTime = dateTime;
                maxDateTime = dateTime;
                firstPoint = false;
            } else {
                if (dateTime < minDateTime) minDateTime = dateTime;
                if (dateTime > maxDateTime) maxDateTime = dateTime;
            }
        }

        chart->addSeries(series);

        axisX->setRange(minDateTime, maxDateTime);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        axisY->setRange(0, qMax(1, maxValue + 1));
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        connect(series, &QLineSeries::hovered, this, &ChartWindow::on_lineHovered);
    }

    ui->statsChartView->setChart(chart);
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);
    ui->chartDetailsLabel->setText("Hover over a chart element for details");
    qDebug() << "Chart updated and set to view";
}

void ChartWindow::populateFilterValues()
{
    ui->filterValueComboBox->clear();
    QString dataType = ui->dataTypeComboBox->currentText();
    QString filterType = ui->statsFilterComboBox->currentText();

    if (dataType == "Clients") {
        if (filterType == "By Sector") {
            QSqlQuery query("SELECT DISTINCT SECTOR FROM AHMED.CLIENTS");
            while (query.next()) {
                ui->filterValueComboBox->addItem(query.value(0).toString());
            }
        } else if (filterType == "By Date") {
            QSqlQuery query("SELECT DISTINCT CONSULTATION_TIMESTAMP FROM AHMED.CLIENTS");
            while (query.next()) {
                ui->filterValueComboBox->addItem(query.value(0).toDateTime().toString("yyyy-MM-dd"));
            }
        } else if (filterType == "By Consultant") {
            QSqlQuery query("SELECT DISTINCT CONSULTANT_ID FROM AHMED.CLIENTS");
            while (query.next()) {
                ui->filterValueComboBox->addItem(query.value(0).toString());
            }
        }
    } else if (dataType == "Trainings") {
        if (filterType == "By Formation") {
            QSqlQuery query("SELECT DISTINCT FORMATION FROM AHMED.FORMATIONS");
            while (query.next()) {
                ui->filterValueComboBox->addItem(query.value(0).toString());
            }
        } else if (filterType == "By Date") {
            QSqlQuery query("SELECT DISTINCT DATEF FROM AHMED.FORMATIONS");
            while (query.next()) {
                ui->filterValueComboBox->addItem(query.value(0).toDate().toString("yyyy-MM-dd"));
            }
        } else if (filterType == "By Trainer") {
            QSqlQuery query("SELECT DISTINCT TRAINER FROM AHMED.FORMATIONS");
            while (query.next()) {
                ui->filterValueComboBox->addItem(query.value(0).toString());
            }
        }
    }
    qDebug() << "Filter Values Populated for" << dataType << "-" << filterType << ":" << ui->filterValueComboBox->count() << "items";
}

void ChartWindow::on_dataTypeComboBox_currentIndexChanged(int /*index*/)
{
    QString dataType = ui->dataTypeComboBox->currentText();
    ui->statsFilterComboBox->clear();
    if (dataType == "Clients") {
        ui->statsFilterComboBox->addItem("By Sector");
        ui->statsFilterComboBox->addItem("By Date");
        ui->statsFilterComboBox->addItem("By Consultant");
    } else if (dataType == "Trainings") {
        ui->statsFilterComboBox->addItem("By Formation");
        ui->statsFilterComboBox->addItem("By Date");
        ui->statsFilterComboBox->addItem("By Trainer");
    }
    populateFilterValues();
    updateChart();
}

void ChartWindow::on_refreshChartButton_clicked()
{
    updateChart();
}

void ChartWindow::on_statsFilterComboBox_currentIndexChanged(int /*index*/)
{
    populateFilterValues();
    updateChart();
}

void ChartWindow::on_chartTypeComboBox_currentIndexChanged(int /*index*/)
{
    currentChartType = ui->chartTypeComboBox->currentText();
    updateChart();
}

void ChartWindow::on_resetChartButton_clicked()
{
    qDebug() << "Resetting chart to default...";
    ui->dataTypeComboBox->setCurrentIndex(0); // "Clients"
    ui->statsFilterComboBox->setCurrentIndex(0); // "By Sector" or "By Formation"
    ui->filterValueComboBox->setCurrentIndex(-1); // Clear selection
    ui->chartTypeComboBox->setCurrentIndex(0); // "Bar Chart"
    currentChartType = "Bar Chart";
    populateFilterValues();
    updateChart();
}

void ChartWindow::on_pieSliceHovered(QPieSlice *slice, bool state)
{
    if (!slice) return;

    slice->setExploded(state);
    if (state) {
        QString fullName;
        int value = slice->value();
        for (auto it = currentDataMap.begin(); it != currentDataMap.end(); ++it) {
            if (it.value() == value && slice->label().contains(it.key().split(" ")[0][0])) {
                fullName = it.key();
                break;
            }
        }
        QString dataType = ui->dataTypeComboBox->currentText();
        QString label = dataType == "Clients" ? "Consultations" : "Trainings";
        ui->chartDetailsLabel->setText(QString("Category: %1\n%2: %3").arg(fullName).arg(label).arg(value));
    } else {
        ui->chartDetailsLabel->setText("Hover over a chart element for details");
    }
}

void ChartWindow::on_barHovered(bool status, int index)
{
    if (!currentBarSet || index < 0 || index >= currentBarSet->count()) return;

    QChart *chart = ui->statsChartView->chart();
    if (status) {
        chart->setAnimationOptions(QChart::NoAnimation);

        qreal originalValue = currentBarSet->at(index);
        currentBarSet->replace(index, originalValue + 0.2);

        QPen pen = currentBarSet->pen();
        pen.setColor(Qt::cyan);
        pen.setWidth(2);
        currentBarSet->setPen(pen);

        hoveredBarIndex = index;

        QString category = currentDataMap.keys().at(index);
        int value = qRound(originalValue);
        QString dataType = ui->dataTypeComboBox->currentText();
        QString label = dataType == "Clients" ? "Consultations" : "Trainings";
        ui->chartDetailsLabel->setText(QString("Category: %1\n%2: %3").arg(category).arg(label).arg(value));
    } else {
        if (hoveredBarIndex != -1) {
            qreal originalValue = currentBarSet->at(hoveredBarIndex) - 0.2;
            currentBarSet->replace(hoveredBarIndex, originalValue);

            QPen pen = currentBarSet->pen();
            pen.setColor(Qt::black);
            pen.setWidth(0);
            currentBarSet->setPen(pen);

            hoveredBarIndex = -1;
        }

        chart->setAnimationOptions(QChart::SeriesAnimations);

        ui->chartDetailsLabel->setText("Hover over a chart element for details");
    }
}

void ChartWindow::on_lineHovered(QPointF point, bool state)
{
    if (state) {
        QDate closestDate;
        int value = 0;
        qreal minDistance = std::numeric_limits<qreal>::max();
        QDateTime hoveredDateTime = QDateTime::fromMSecsSinceEpoch(point.x());
        QDate hoveredDate = hoveredDateTime.date();

        for (auto it = lineDataMap.begin(); it != lineDataMap.end(); ++it) {
            qreal distance = qAbs(it.key().daysTo(hoveredDate));
            if (distance < minDistance) {
                minDistance = distance;
                closestDate = it.key();
                value = it.value();
            }
        }

        if (minDistance <= 1) {
            QString dataType = ui->dataTypeComboBox->currentText();
            QString label = dataType == "Clients" ? "Consultations" : "Trainings";
            ui->chartDetailsLabel->setText(QString("Date: %1\n%2: %3")
                                               .arg(closestDate.toString("yyyy-MM-dd"))
                                               .arg(label)
                                               .arg(value));
        } else {
            ui->chartDetailsLabel->setText("Hover over a point for details");
        }
    } else {
        ui->chartDetailsLabel->setText("Hover over a point for details");
    }
}
