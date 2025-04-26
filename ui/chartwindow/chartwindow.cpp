// ui/chartwindow/chartwindow.cpp
#include "chartwindow.h"
#include "ui_chartwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QGraphicsTextItem>

ChartWindow::ChartWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentChart(nullptr),
    tooltipLabel(new QLabel(this)),
    currentBarSet(nullptr)
{
    ui->setupUi(this);

    tooltipLabel->setStyleSheet("QLabel { background-color: white; border: 1px solid black; padding: 5px; }");
    tooltipLabel->hide();

    connect(ui->refreshChartButton, &QPushButton::clicked, this, &ChartWindow::on_refreshChartButton_clicked);
    connect(ui->resetChartButton, &QPushButton::clicked, this, &ChartWindow::on_resetChartButton_clicked);
    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);
    connect(ui->filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);

    updateFilterComboBox();
    populateStatsViews();
}

ChartWindow::~ChartWindow()
{
    delete currentChart;
    delete tooltipLabel;
    delete ui;
}

void ChartWindow::populateStatsViews()
{
    ui->statsTypeComboBox->clear();
    ui->statsTypeComboBox->addItems({"Client Statistics", "Training Statistics", "Meeting Statistics"});
    updateChart();
}

void ChartWindow::updateFilterComboBox()
{
    ui->filterComboBox->clear();
    QString statsType = ui->statsTypeComboBox->currentText();

    if (statsType == "Client Statistics") {
        ui->filterComboBox->addItems({"All Clients", "By Gender", "By Registration Year"});
    } else if (statsType == "Training Statistics") {
        ui->filterComboBox->addItems({"All Trainings", "By Type", "By Year"});
    } else if (statsType == "Meeting Statistics") {
        ui->filterComboBox->addItems({"All Meetings", "By Agenda", "By Duration"});
    }
}

void ChartWindow::updateChart()
{
    QString statsType = ui->statsTypeComboBox->currentText();
    QString filter = ui->filterComboBox->currentText();

    delete currentChart;
    currentChart = new QChart();
    currentChart->setTitle(statsType);
    currentChart->setAnimationOptions(QChart::SeriesAnimations);

    QSqlQuery query;

    if (statsType == "Client Statistics") {
        if (filter == "All Clients") {
            query.exec("SELECT COUNT(*) FROM client");
            if (query.next()) {
                int count = query.value(0).toInt();
                QPieSeries *series = new QPieSeries();
                series->append("Clients", count);
                series->append("Others", count > 0 ? 100 - count : 100);
                currentChart->addSeries(series);
            }
        } else if (filter == "By Gender") {
            query.exec("SELECT gender, COUNT(*) FROM client GROUP BY gender");
            QPieSeries *series = new QPieSeries();
            while (query.next()) {
                QString gender = query.value(0).toString();
                int count = query.value(1).toInt();
                series->append(gender, count);
            }
            currentChart->addSeries(series);
        } else if (filter == "By Registration Year") {
            query.exec("SELECT strftime('%Y', registration_date) as year, COUNT(*) FROM client GROUP BY year");
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Clients");
            QStringList categories;
            while (query.next()) {
                QString year = query.value(0).toString();
                int count = query.value(1).toInt();
                *set << count;
                categories << year;
            }
            series->append(set);
            currentChart->addSeries(series);
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            currentChart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            QValueAxis *axisY = new QValueAxis();
            currentChart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
        }
    } else if (statsType == "Training Statistics") {
        if (filter == "All Trainings") {
            query.exec("SELECT COUNT(*) FROM formations");
            if (query.next()) {
                int count = query.value(0).toInt();
                QPieSeries *series = new QPieSeries();
                series->append("Trainings", count);
                series->append("Others", count > 0 ? 100 - count : 100);
                currentChart->addSeries(series);
            }
        } else if (filter == "By Type") {
            query.exec("SELECT type, COUNT(*) FROM formations GROUP BY type");
            QPieSeries *series = new QPieSeries();
            while (query.next()) {
                QString type = query.value(0).toString();
                int count = query.value(1).toInt();
                series->append(type, count);
            }
            currentChart->addSeries(series);
        } else if (filter == "By Year") {
            query.exec("SELECT strftime('%Y', date_formation) as year, COUNT(*) FROM formations GROUP BY year");
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Trainings");
            QStringList categories;
            while (query.next()) {
                QString year = query.value(0).toString();
                int count = query.value(1).toInt();
                *set << count;
                categories << year;
            }
            series->append(set);
            currentChart->addSeries(series);
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            currentChart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            QValueAxis *axisY = new QValueAxis();
            currentChart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
        }
    } else if (statsType == "Meeting Statistics") {
        if (filter == "All Meetings") {
            query.exec("SELECT COUNT(*) FROM meeting");
            if (query.next()) {
                int count = query.value(0).toInt();
                QPieSeries *series = new QPieSeries();
                series->append("Meetings", count);
                series->append("Others", count > 0 ? 100 - count : 100);
                currentChart->addSeries(series);
            }
        } else if (filter == "By Agenda") {
            query.exec("SELECT agenda, COUNT(*) FROM meeting GROUP BY agenda");
            QPieSeries *series = new QPieSeries();
            while (query.next()) {
                QString agenda = query.value(0).toString();
                int count = query.value(1).toInt();
                series->append(agenda, count);
            }
            currentChart->addSeries(series);
        } else if (filter == "By Duration") {
            query.exec("SELECT CASE "
                       "WHEN duration <= 30 THEN 'Short (0-30 mins)' "
                       "WHEN duration <= 60 THEN 'Medium (31-60 mins)' "
                       "ELSE 'Long (>60 mins)' END as range, COUNT(*) "
                       "FROM meeting GROUP BY range");
            QPieSeries *series = new QPieSeries();
            while (query.next()) {
                QString range = query.value(0).toString();
                int count = query.value(1).toInt();
                series->append(range, count);
            }
            currentChart->addSeries(series);
        }
    }

    for (QAbstractSeries *series : currentChart->series()) {
        if (QPieSeries *pieSeries = dynamic_cast<QPieSeries*>(series)) {
            for (QPieSlice *slice : pieSeries->slices()) {
                slice->setLabel(QString("%1: %2 (%3%)")
                                    .arg(slice->label())
                                    .arg(static_cast<int>(slice->value()))
                                    .arg(slice->percentage() * 100, 0, 'f', 1));
                slice->setLabelVisible();
                connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                    handlePieSliceHovered(slice, state);
                });
            }
        } else if (QBarSeries *barSeries = dynamic_cast<QBarSeries*>(series)) {
            for (QBarSet *set : barSeries->barSets()) {
                currentBarSet = set; // Store for use in handleBarHovered
                connect(set, &QBarSet::hovered, this, &ChartWindow::handleBarHovered);
            }
        }
    }

    currentChart->legend()->setVisible(true);
    currentChart->legend()->setAlignment(Qt::AlignBottom);
    ui->statsChartView->setChart(currentChart); // Use statsChartView
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);
}

void ChartWindow::on_refreshChartButton_clicked()
{
    updateChart();
}

void ChartWindow::on_resetChartButton_clicked()
{
    ui->statsTypeComboBox->setCurrentIndex(0);
    updateFilterComboBox();
    updateChart();
}

void ChartWindow::handlePieSliceHovered(QPieSlice *slice, bool state)
{
    if (state) {
        tooltipLabel->setText(QString("%1: %2 (%3%)")
                                  .arg(slice->label())
                                  .arg(static_cast<int>(slice->value()))
                                  .arg(slice->percentage() * 100, 0, 'f', 1));
        tooltipLabel->move(QCursor::pos());
        tooltipLabel->show();
    } else {
        tooltipLabel->hide();
    }
}

void ChartWindow::handleBarHovered(bool status, int index)
{
    if (status && currentBarSet) {
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(currentChart->axes(Qt::Horizontal).first());
        if (axisX) {
            QString category = axisX->categories().at(index);
            qreal value = currentBarSet->at(index);
            tooltipLabel->setText(QString("%1: %2").arg(category).arg(value));
            tooltipLabel->move(QCursor::pos());
            tooltipLabel->show();
        }
    } else {
        tooltipLabel->hide();
    }
}
