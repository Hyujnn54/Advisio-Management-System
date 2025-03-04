/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *txtTitle;
    QPushButton *btnAdd;
    QLabel *label_2;
    QLineEdit *txtTrainer;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QFrame *frame;
    QPushButton *sidebarButton;
    QPushButton *sidebarButton_2;
    QPushButton *sidebarButton_3;
    QPushButton *sidebarButton_4;
    QPushButton *sidebarButton_5;
    QFrame *frame_3;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *updateButton;
    QLabel *label;
    QLabel *label_8;
    QDoubleSpinBox *spinCost;
    QPushButton *exportPdfButton;
    QLabel *label_9;
    QTextEdit *txtDescription;
    QLabel *label_4;
    QSpinBox *spinDuration;
    QLabel *label_7;
    QLabel *label_6;
    QPushButton *deletef;
    QListWidget *listWidget;
    QTableWidget *tabtr;
    QLineEdit *format;
    QPushButton *add;
    QLabel *label_3;
    QLineEdit *tr;
    QLabel *label_10;
    QDoubleSpinBox *prixb;
    QTextEdit *des;
    QLabel *label_13;
    QSpinBox *timeb;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QDateEdit *date;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 798);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background: qlineargradient(\n"
"        spread: pad,\n"
"        x1: 0, y1: 0,\n"
"        x2: 0, y2: 1,\n"
"        stop: 0 #87CEEB, /* Sky Blue */\n"
"        stop: 1 #FFFFFF  /* White */\n"
"    );\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        txtTitle = new QLineEdit(centralwidget);
        txtTitle->setObjectName("txtTitle");
        txtTitle->setGeometry(QRect(220, 150, 121, 41));
        txtTitle->setStyleSheet(QString::fromUtf8("width: 100%;\n"
"color: rgb(0, 0, 0);\n"
"    padding: 10px;\n"
"    margin: 8px 0;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"    background: #f9f9f9;"));
        btnAdd = new QPushButton(centralwidget);
        btnAdd->setObjectName("btnAdd");
        btnAdd->setGeometry(QRect(450, 510, 111, 31));
        btnAdd->setStyleSheet(QString::fromUtf8("background-color: #007bff;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(220, 140, 71, 16));
        QFont font;
        font.setFamilies({QString::fromUtf8("Open Sans")});
        font.setBold(true);
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;\n"
"background-color: transparent;"));
        txtTrainer = new QLineEdit(centralwidget);
        txtTrainer->setObjectName("txtTrainer");
        txtTrainer->setGeometry(QRect(220, 380, 191, 51));
        txtTrainer->setStyleSheet(QString::fromUtf8("width: 100%;\n"
"color: rgb(0, 0, 0);\n"
"    padding: 10px;\n"
"    margin: 8px 0;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"    background: #f9f9f9;"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(410, 280, 49, 16));
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(910, 210, 181, 21));
        lineEdit->setStyleSheet(QString::fromUtf8("border: 1px solid #cccccc;\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"    font-size: 14px;\n"
"    background-color: #ffffff;\n"
"    transition: border-color 0.3s ease, background-color 0.3s ease;\n"
""));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 90, 191, 541));
        frame->setToolTipDuration(1);
        frame->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: white; /* White background */\n"
"}"));
        frame->setLocale(QLocale(QLocale::English, QLocale::World));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        sidebarButton = new QPushButton(frame);
        sidebarButton->setObjectName("sidebarButton");
        sidebarButton->setGeometry(QRect(20, 90, 151, 61));
        sidebarButton->setStyleSheet(QString::fromUtf8("QPushButton#sidebarButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#sidebarButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        sidebarButton_2 = new QPushButton(frame);
        sidebarButton_2->setObjectName("sidebarButton_2");
        sidebarButton_2->setGeometry(QRect(20, 180, 151, 61));
        sidebarButton_2->setStyleSheet(QString::fromUtf8("QPushButton#sidebarButton_2 {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#sidebarButton_2:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        sidebarButton_3 = new QPushButton(frame);
        sidebarButton_3->setObjectName("sidebarButton_3");
        sidebarButton_3->setGeometry(QRect(20, 270, 151, 61));
        sidebarButton_3->setStyleSheet(QString::fromUtf8("QPushButton#sidebarButton_3 {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#sidebarButton_3:hover {\n"
"   background-color: #0056A3;\n"
"}"));
        sidebarButton_4 = new QPushButton(frame);
        sidebarButton_4->setObjectName("sidebarButton_4");
        sidebarButton_4->setGeometry(QRect(20, 360, 151, 61));
        sidebarButton_4->setStyleSheet(QString::fromUtf8("QPushButton#sidebarButton_4 {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#sidebarButton_4:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        sidebarButton_5 = new QPushButton(frame);
        sidebarButton_5->setObjectName("sidebarButton_5");
        sidebarButton_5->setGeometry(QRect(20, 450, 151, 61));
        sidebarButton_5->setStyleSheet(QString::fromUtf8("QPushButton#sidebarButton_5 {\n"
"   background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#sidebarButton_5:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(10, 10, 191, 161));
        frame_3->setStyleSheet(QString::fromUtf8("QWidgett {\n"
"    background-color: #0078D7; /* Solid Blue */\n"
"    border-bottom: 2px solid white; /* White separator */\n"
"}"));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        label_11 = new QLabel(frame_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(30, 0, 161, 171));
        label_11->setStyleSheet(QString::fromUtf8("background-color: transparent\n"
";"));
        label_11->setPixmap(QPixmap(QString::fromUtf8("C:/Users/Desktop/LLLLL.png")));
        label_11->setScaledContents(true);
        label_12 = new QLabel(frame_3);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(10, 10, 161, 151));
        label_12->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        label_12->setPixmap(QPixmap(QString::fromUtf8("LLLLL.png")));
        label_12->setScaledContents(true);
        updateButton = new QPushButton(centralwidget);
        updateButton->setObjectName("updateButton");
        updateButton->setGeometry(QRect(1040, 440, 71, 31));
        updateButton->setStyleSheet(QString::fromUtf8("background-color: #007bff;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;"));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(400, 10, 411, 51));
        label->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 22px;\n"
"    font-weight: bold;\n"
"    color: #003f7f; /* Navy blue */\n"
"    margin-bottom: 15px;"));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(230, 90, 171, 21));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Open Sans")});
        font1.setPointSize(13);
        font1.setWeight(QFont::Black);
        label_8->setFont(font1);
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: transparent;"));
        spinCost = new QDoubleSpinBox(centralwidget);
        spinCost->setObjectName("spinCost");
        spinCost->setGeometry(QRect(420, 390, 61, 31));
        spinCost->setStyleSheet(QString::fromUtf8("display: flex;\n"
"color: rgb(0, 0, 0);\n"
"    gap: 10px;\n"
"background-color: transparent;"));
        exportPdfButton = new QPushButton(centralwidget);
        exportPdfButton->setObjectName("exportPdfButton");
        exportPdfButton->setGeometry(QRect(830, 440, 111, 31));
        exportPdfButton->setStyleSheet(QString::fromUtf8("background-color: #007bff;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;"));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(870, 210, 41, 21));
        label_9->setStyleSheet(QString::fromUtf8("    background-color: transparent;\n"
"    border: 1px solid #aaaaaa;\n"
"    border-radius: 5px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;"));
        label_9->setPixmap(QPixmap(QString::fromUtf8("search.png")));
        label_9->setScaledContents(true);
        txtDescription = new QTextEdit(centralwidget);
        txtDescription->setObjectName("txtDescription");
        txtDescription->setGeometry(QRect(220, 250, 191, 81));
        txtDescription->setStyleSheet(QString::fromUtf8("width: 100%;\n"
"color: rgb(0, 0, 0);\n"
"    padding: 10px;\n"
"    margin: 8px 0;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"    background: #f9f9f9;"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(220, 370, 61, 16));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        spinDuration = new QSpinBox(centralwidget);
        spinDuration->setObjectName("spinDuration");
        spinDuration->setGeometry(QRect(420, 300, 41, 25));
        spinDuration->setStyleSheet(QString::fromUtf8("display: flex;\n"
"color: rgb(0, 0, 0);\n"
"    gap: 10px;\n"
"background-color: transparent;"));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(610, 210, 121, 21));
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: transparent;"));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(410, 370, 49, 16));
        label_6->setFont(font);
        label_6->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        deletef = new QPushButton(centralwidget);
        deletef->setObjectName("deletef");
        deletef->setGeometry(QRect(950, 440, 71, 31));
        deletef->setStyleSheet(QString::fromUtf8("background-color: #007bff;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;"));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(210, 120, 371, 451));
        listWidget->setStyleSheet(QString::fromUtf8("background-color: #fefefe;\n"
"    border: 1px solid #e0e0e0;\n"
"    border-radius: 12px;\n"
"    padding: 20px;\n"
"    box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1);"));
        tabtr = new QTableWidget(centralwidget);
        if (tabtr->columnCount() < 6)
            tabtr->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tabtr->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tabtr->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tabtr->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tabtr->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tabtr->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tabtr->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tabtr->setObjectName("tabtr");
        tabtr->setGeometry(QRect(600, 240, 511, 191));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Open Sans")});
        font2.setBold(true);
        font2.setItalic(false);
        tabtr->setFont(font2);
        tabtr->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"color: rgb(0, 0, 0);\n"
"color: rgb(0, 0, 0);\n"
"    border: 1px solid #dee2e6;\n"
"    border-radius: 4px;\n"
"    gridline-color: #dee2e6;"));
        format = new QLineEdit(centralwidget);
        format->setObjectName("format");
        format->setGeometry(QRect(220, 150, 141, 61));
        format->setStyleSheet(QString::fromUtf8("width: 100%;\n"
"color: rgb(0, 0, 0);\n"
"    padding: 10px;\n"
"    margin: 8px 0;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"    background: #f9f9f9;"));
        add = new QPushButton(centralwidget);
        add->setObjectName("add");
        add->setGeometry(QRect(450, 530, 111, 31));
        add->setStyleSheet(QString::fromUtf8("background-color: #007bff;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(220, 140, 71, 16));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;\n"
"background-color: transparent;"));
        tr = new QLineEdit(centralwidget);
        tr->setObjectName("tr");
        tr->setGeometry(QRect(220, 380, 211, 61));
        tr->setStyleSheet(QString::fromUtf8("width: 100%;\n"
"color: rgb(0, 0, 0);\n"
"    padding: 10px;\n"
"    margin: 8px 0;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"    background: #f9f9f9;"));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(440, 300, 49, 16));
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        prixb = new QDoubleSpinBox(centralwidget);
        prixb->setObjectName("prixb");
        prixb->setGeometry(QRect(440, 390, 61, 31));
        prixb->setStyleSheet(QString::fromUtf8("display: flex;\n"
"color: rgb(0, 0, 0);\n"
"    gap: 10px;\n"
"background-color: transparent;"));
        des = new QTextEdit(centralwidget);
        des->setObjectName("des");
        des->setGeometry(QRect(220, 250, 221, 111));
        des->setStyleSheet(QString::fromUtf8("width: 100%;\n"
"color: rgb(0, 0, 0);\n"
"    padding: 10px;\n"
"    margin: 8px 0;\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"    background: #f9f9f9;"));
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(220, 370, 61, 16));
        label_13->setFont(font);
        label_13->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        timeb = new QSpinBox(centralwidget);
        timeb->setObjectName("timeb");
        timeb->setGeometry(QRect(450, 320, 41, 25));
        timeb->setStyleSheet(QString::fromUtf8("display: flex;\n"
"color: rgb(0, 0, 0);\n"
"color: rgb(0, 0, 0);\n"
"    gap: 10px;\n"
"background-color: transparent;"));
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(440, 370, 49, 16));
        label_14->setFont(font);
        label_14->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(220, 230, 101, 21));
        label_15->setFont(font);
        label_15->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;background-color: transparent;"));
        label_16 = new QLabel(centralwidget);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(220, 460, 61, 16));
        label_16->setFont(font);
        label_16->setStyleSheet(QString::fromUtf8("background-color: transparent;font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    padding-bottom: 5px;"));
        date = new QDateEdit(centralwidget);
        date->setObjectName("date");
        date->setGeometry(QRect(220, 480, 131, 25));
        date->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        txtTitle->setText(QString());
        btnAdd->setText(QCoreApplication::translate("MainWindow", "Add Training", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "formation", nullptr));
        txtTrainer->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "time", nullptr));
        sidebarButton->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        sidebarButton_2->setText(QCoreApplication::translate("MainWindow", "Client", nullptr));
        sidebarButton_3->setText(QCoreApplication::translate("MainWindow", "Resource", nullptr));
        sidebarButton_4->setText(QCoreApplication::translate("MainWindow", "Meeting", nullptr));
        sidebarButton_5->setText(QCoreApplication::translate("MainWindow", "Training", nullptr));
        label_11->setText(QString());
        label_12->setText(QString());
        updateButton->setText(QCoreApplication::translate("MainWindow", "Modify", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:20pt;\">Training Management System</span></p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Ajouter Formation</p></body></html>", nullptr));
        exportPdfButton->setText(QCoreApplication::translate("MainWindow", "download PDF", nullptr));
        label_9->setText(QString());
        txtDescription->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:16px; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><br /></p></body></html>", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Trainer", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">tableTrainings</span></p></body></html>", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Price", nullptr));
        deletef->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tabtr->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tabtr->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "formation", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tabtr->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "date", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tabtr->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "duration", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tabtr->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "cost", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tabtr->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "trainer", nullptr));
        format->setText(QString());
        add->setText(QCoreApplication::translate("MainWindow", "Add Training", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "formation", nullptr));
        tr->setText(QString());
        label_10->setText(QCoreApplication::translate("MainWindow", "time", nullptr));
        des->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:16px; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><br /></p></body></html>", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Trainer", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Price", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
