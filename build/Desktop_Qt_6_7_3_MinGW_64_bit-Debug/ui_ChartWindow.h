/********************************************************************************
** Form generated from reading UI file 'ChartWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTWINDOW_H
#define UI_CHARTWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *chartGroupBox;
    QVBoxLayout *chartLayout;
    QGridLayout *controlGridLayout;
    QLabel *statsTypeLabel;
    QComboBox *statsTypeComboBox;
    QLabel *chartTypeLabel;
    QComboBox *chartTypeComboBox;
    QLabel *chartDetailLabel;
    QComboBox *chartDetailComboBox;
    QLabel *filterLabel;
    QComboBox *filterComboBox;
    QHBoxLayout *buttonLayout;
    QPushButton *resetButton;
    QCheckBox *toggleLegendCheckBox;
    QLabel *percentageLabel;
    QSpacerItem *buttonSpacer;
    QChartView *statsChartView;
    QLabel *hoverDescriptionLabel;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChartWindow)
    {
        if (ChartWindow->objectName().isEmpty())
            ChartWindow->setObjectName("ChartWindow");
        ChartWindow->resize(900, 700);
        ChartWindow->setStyleSheet(QString::fromUtf8("\n"
"    /* Base styles that apply to both light and dark themes */\n"
"    QGroupBox { \n"
"        font-weight: bold; \n"
"        border-radius: 5px; \n"
"        margin-top: 10px; \n"
"    }\n"
"    QGroupBox::title { \n"
"        subcontrol-origin: margin; \n"
"        subcontrol-position: top left; \n"
"        padding: 0 5px; \n"
"    }\n"
"    QComboBox { \n"
"        padding: 5px; \n"
"        border-radius: 3px; \n"
"    }\n"
"    QComboBox:hover { \n"
"        border-width: 2px;\n"
"    }\n"
"    QPushButton { \n"
"        padding: 5px 10px; \n"
"        border-radius: 3px; \n"
"    }\n"
"    QCheckBox { padding: 5px; }\n"
"    QLabel#hoverDescriptionLabel { \n"
"        border-radius: 3px; \n"
"        padding: 5px; \n"
"        font-style: italic; \n"
"    }\n"
"    QLabel#percentageLabel {\n"
"        font-weight: bold;\n"
"        font-size: 14px;\n"
"        border-radius: 5px;\n"
"        padding: 5px;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(ChartWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        chartGroupBox = new QGroupBox(centralwidget);
        chartGroupBox->setObjectName("chartGroupBox");
        chartLayout = new QVBoxLayout(chartGroupBox);
        chartLayout->setSpacing(10);
        chartLayout->setObjectName("chartLayout");
        controlGridLayout = new QGridLayout();
        controlGridLayout->setSpacing(10);
        controlGridLayout->setObjectName("controlGridLayout");
        statsTypeLabel = new QLabel(chartGroupBox);
        statsTypeLabel->setObjectName("statsTypeLabel");

        controlGridLayout->addWidget(statsTypeLabel, 0, 0, 1, 1);

        statsTypeComboBox = new QComboBox(chartGroupBox);
        statsTypeComboBox->addItem(QString());
        statsTypeComboBox->addItem(QString());
        statsTypeComboBox->addItem(QString());
        statsTypeComboBox->setObjectName("statsTypeComboBox");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(statsTypeComboBox->sizePolicy().hasHeightForWidth());
        statsTypeComboBox->setSizePolicy(sizePolicy);

        controlGridLayout->addWidget(statsTypeComboBox, 0, 1, 1, 1);

        chartTypeLabel = new QLabel(chartGroupBox);
        chartTypeLabel->setObjectName("chartTypeLabel");

        controlGridLayout->addWidget(chartTypeLabel, 0, 2, 1, 1);

        chartTypeComboBox = new QComboBox(chartGroupBox);
        chartTypeComboBox->addItem(QString());
        chartTypeComboBox->addItem(QString());
        chartTypeComboBox->setObjectName("chartTypeComboBox");
        sizePolicy.setHeightForWidth(chartTypeComboBox->sizePolicy().hasHeightForWidth());
        chartTypeComboBox->setSizePolicy(sizePolicy);

        controlGridLayout->addWidget(chartTypeComboBox, 0, 3, 1, 1);

        chartDetailLabel = new QLabel(chartGroupBox);
        chartDetailLabel->setObjectName("chartDetailLabel");

        controlGridLayout->addWidget(chartDetailLabel, 1, 0, 1, 1);

        chartDetailComboBox = new QComboBox(chartGroupBox);
        chartDetailComboBox->setObjectName("chartDetailComboBox");
        sizePolicy.setHeightForWidth(chartDetailComboBox->sizePolicy().hasHeightForWidth());
        chartDetailComboBox->setSizePolicy(sizePolicy);

        controlGridLayout->addWidget(chartDetailComboBox, 1, 1, 1, 1);

        filterLabel = new QLabel(chartGroupBox);
        filterLabel->setObjectName("filterLabel");

        controlGridLayout->addWidget(filterLabel, 1, 2, 1, 1);

        filterComboBox = new QComboBox(chartGroupBox);
        filterComboBox->addItem(QString());
        filterComboBox->setObjectName("filterComboBox");
        sizePolicy.setHeightForWidth(filterComboBox->sizePolicy().hasHeightForWidth());
        filterComboBox->setSizePolicy(sizePolicy);

        controlGridLayout->addWidget(filterComboBox, 1, 3, 1, 1);


        chartLayout->addLayout(controlGridLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        resetButton = new QPushButton(chartGroupBox);
        resetButton->setObjectName("resetButton");
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("edit-clear")));
        resetButton->setIcon(icon);

        buttonLayout->addWidget(resetButton);

        toggleLegendCheckBox = new QCheckBox(chartGroupBox);
        toggleLegendCheckBox->setObjectName("toggleLegendCheckBox");
        toggleLegendCheckBox->setChecked(true);

        buttonLayout->addWidget(toggleLegendCheckBox);

        percentageLabel = new QLabel(chartGroupBox);
        percentageLabel->setObjectName("percentageLabel");
        percentageLabel->setAlignment(Qt::AlignCenter);

        buttonLayout->addWidget(percentageLabel);

        buttonSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(buttonSpacer);


        chartLayout->addLayout(buttonLayout);

        statsChartView = new QChartView(chartGroupBox);
        statsChartView->setObjectName("statsChartView");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(statsChartView->sizePolicy().hasHeightForWidth());
        statsChartView->setSizePolicy(sizePolicy1);
        statsChartView->setMinimumSize(QSize(0, 400));

        chartLayout->addWidget(statsChartView);

        hoverDescriptionLabel = new QLabel(chartGroupBox);
        hoverDescriptionLabel->setObjectName("hoverDescriptionLabel");
        hoverDescriptionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        hoverDescriptionLabel->setMinimumHeight(30);

        chartLayout->addWidget(hoverDescriptionLabel);


        verticalLayout->addWidget(chartGroupBox);

        ChartWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(ChartWindow);
        statusBar->setObjectName("statusBar");
        ChartWindow->setStatusBar(statusBar);

        retranslateUi(ChartWindow);

        QMetaObject::connectSlotsByName(ChartWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChartWindow)
    {
        ChartWindow->setWindowTitle(QCoreApplication::translate("ChartWindow", "Advanced Statistics", nullptr));
        chartGroupBox->setTitle(QCoreApplication::translate("ChartWindow", "Advanced Statistics Dashboard", nullptr));
        statsTypeLabel->setText(QCoreApplication::translate("ChartWindow", "Data Source:", nullptr));
        statsTypeComboBox->setItemText(0, QCoreApplication::translate("ChartWindow", "Client Statistics", nullptr));
        statsTypeComboBox->setItemText(1, QCoreApplication::translate("ChartWindow", "Training Statistics", nullptr));
        statsTypeComboBox->setItemText(2, QCoreApplication::translate("ChartWindow", "Meeting Statistics", nullptr));

        chartTypeLabel->setText(QCoreApplication::translate("ChartWindow", "Chart Type:", nullptr));
        chartTypeComboBox->setItemText(0, QCoreApplication::translate("ChartWindow", "Bar Chart", nullptr));
        chartTypeComboBox->setItemText(1, QCoreApplication::translate("ChartWindow", "Pie Chart", nullptr));

        chartDetailLabel->setText(QCoreApplication::translate("ChartWindow", "Analyze by:", nullptr));
        filterLabel->setText(QCoreApplication::translate("ChartWindow", "Filter:", nullptr));
        filterComboBox->setItemText(0, QCoreApplication::translate("ChartWindow", "All Data", nullptr));

        resetButton->setText(QCoreApplication::translate("ChartWindow", "Reset View", nullptr));
        toggleLegendCheckBox->setText(QCoreApplication::translate("ChartWindow", "Show Legend", nullptr));
        percentageLabel->setText(QString());
        hoverDescriptionLabel->setText(QCoreApplication::translate("ChartWindow", "Hover over a chart element to see details", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartWindow: public Ui_ChartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTWINDOW_H
