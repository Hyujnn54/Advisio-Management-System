/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_4;
    QDateEdit *dateEdit;
    QDateEdit *dateEdit_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QTableWidget *tableWidget;
    QWidget *widget;
    QLabel *label;
    QLabel *label_2;
    QFrame *frame;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        QPalette palette;
        QLinearGradient gradient(0, 0, 1, 1);
        gradient.setSpread(QGradient::PadSpread);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0, QColor(173, 216, 230, 255));
        gradient.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush(gradient);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QLinearGradient gradient1(0, 0, 1, 1);
        gradient1.setSpread(QGradient::PadSpread);
        gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient1.setColorAt(0, QColor(173, 216, 230, 255));
        gradient1.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush1(gradient1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QLinearGradient gradient2(0, 0, 1, 1);
        gradient2.setSpread(QGradient::PadSpread);
        gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient2.setColorAt(0, QColor(173, 216, 230, 255));
        gradient2.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush2(gradient2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        QLinearGradient gradient3(0, 0, 1, 1);
        gradient3.setSpread(QGradient::PadSpread);
        gradient3.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient3.setColorAt(0, QColor(173, 216, 230, 255));
        gradient3.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush3(gradient3);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        QLinearGradient gradient4(0, 0, 1, 1);
        gradient4.setSpread(QGradient::PadSpread);
        gradient4.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient4.setColorAt(0, QColor(173, 216, 230, 255));
        gradient4.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush4(gradient4);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        QLinearGradient gradient5(0, 0, 1, 1);
        gradient5.setSpread(QGradient::PadSpread);
        gradient5.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient5.setColorAt(0, QColor(173, 216, 230, 255));
        gradient5.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush5(gradient5);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        QLinearGradient gradient6(0, 0, 1, 1);
        gradient6.setSpread(QGradient::PadSpread);
        gradient6.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient6.setColorAt(0, QColor(173, 216, 230, 255));
        gradient6.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush6(gradient6);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        QLinearGradient gradient7(0, 0, 1, 1);
        gradient7.setSpread(QGradient::PadSpread);
        gradient7.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient7.setColorAt(0, QColor(173, 216, 230, 255));
        gradient7.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush7(gradient7);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        QLinearGradient gradient8(0, 0, 1, 1);
        gradient8.setSpread(QGradient::PadSpread);
        gradient8.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient8.setColorAt(0, QColor(173, 216, 230, 255));
        gradient8.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush8(gradient8);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        MainWindow->setPalette(palette);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, \n"
"        stop:0 rgba(173, 216, 230, 255), /* Light Blue */\n"
"        stop:1 rgba(255, 255, 255, 255)); /* White */\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalWidget = new QWidget(centralwidget);
        verticalWidget->setObjectName("verticalWidget");
        verticalWidget->setGeometry(QRect(330, 130, 261, 401));
        verticalWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        verticalLayout = new QVBoxLayout(verticalWidget);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(15, 10, 15, -1);
        lineEdit_2 = new QLineEdit(verticalWidget);
        lineEdit_2->setObjectName("lineEdit_2");
        QPalette palette1;
        QBrush brush9(QColor(0, 0, 0, 0));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush9);
        QBrush brush10(QColor(255, 255, 255, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush9);
        lineEdit_2->setPalette(palette1);
        lineEdit_2->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(lineEdit_2);

        lineEdit_4 = new QLineEdit(verticalWidget);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(lineEdit_4);

        dateEdit = new QDateEdit(verticalWidget);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
""));

        verticalLayout->addWidget(dateEdit);

        dateEdit_2 = new QDateEdit(verticalWidget);
        dateEdit_2->setObjectName("dateEdit_2");
        dateEdit_2->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(dateEdit_2);

        pushButton_2 = new QPushButton(verticalWidget);
        pushButton_2->setObjectName("pushButton_2");
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        QBrush brush11(QColor(0, 120, 215, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush11);
        QBrush brush12(QColor(255, 255, 255, 128));
        brush12.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_2->setPalette(palette2);
        pushButton_2->setAutoFillBackground(false);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));

        verticalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(verticalWidget);
        pushButton->setObjectName("pushButton");
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton->setPalette(palette3);
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));

        verticalLayout->addWidget(pushButton);

        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(650, 120, 511, 261));
        tableWidget->setStyleSheet(QString::fromUtf8("background-color: white;"));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(320, 120, 281, 421));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
""));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(320, 60, 261, 61));
        QPalette palette4;
        QBrush brush13(QColor(0, 0, 0, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush13);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush13);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush9);
        QBrush brush14(QColor(0, 0, 0, 128));
        brush14.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Active, QPalette::PlaceholderText, brush14);
#endif
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush13);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush13);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush9);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush14);
#endif
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush9);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush14);
#endif
        label->setPalette(palette4);
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(20);
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: transparent;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(650, 60, 261, 61));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush13);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush13);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush9);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Active, QPalette::PlaceholderText, brush14);
#endif
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush13);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush13);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush9);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush14);
#endif
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush9);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush14);
#endif
        label_2->setPalette(palette5);
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: transparent;"));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 231, 781));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(0, 110, 221, 36));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_3->setPalette(palette6);
        pushButton_3->setAutoFillBackground(false);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_4 = new QPushButton(frame);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(0, 160, 221, 36));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette7.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette7.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette7.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_4->setPalette(palette7);
        pushButton_4->setAutoFillBackground(false);
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_5 = new QPushButton(frame);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(0, 210, 221, 36));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette8.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette8.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette8.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_5->setPalette(palette8);
        pushButton_5->setAutoFillBackground(false);
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_6 = new QPushButton(frame);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(0, 260, 221, 36));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette9.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette9.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette9.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette9.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette9.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette9.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette9.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette9.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette9.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette9.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_6->setPalette(palette9);
        pushButton_6->setAutoFillBackground(false);
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_7 = new QPushButton(frame);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(0, 320, 221, 36));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette10.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette10.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette10.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_7->setPalette(palette10);
        pushButton_7->setAutoFillBackground(false);
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(1020, 390, 141, 36));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette11.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette11.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette11.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette11.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette11.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette11.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_8->setPalette(palette11);
        pushButton_8->setAutoFillBackground(false);
        pushButton_8->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_9 = new QPushButton(centralwidget);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(870, 390, 141, 36));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush10);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette12.setBrush(QPalette::Active, QPalette::PlaceholderText, brush12);
#endif
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush10);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette12.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush12);
#endif
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette12.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush12);
#endif
        pushButton_9->setPalette(palette12);
        pushButton_9->setAutoFillBackground(false);
        pushButton_9->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        MainWindow->setCentralWidget(centralwidget);
        widget->raise();
        verticalWidget->raise();
        tableWidget->raise();
        label->raise();
        label_2->raise();
        frame->raise();
        pushButton_8->raise();
        pushButton_9->raise();
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lineEdit_2->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter Resource Name", nullptr));
        lineEdit_4->setText(QString());
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter Resource Quantity", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Purchase Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Expiration Dare", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Add Resources :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Resources :", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Resources", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "departement", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "employee", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "formation", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "materiel", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "Modify", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
