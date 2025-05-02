/*#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSerialPort>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_btnSearch_clicked();

private:
    Ui::SearchDialog *ui;
    QSerialPort *serialPort;

    bool queryDatabase(const QString &resourceName);
    void sendToArduino(bool resourceExists);
};

#endif // SEARCHDIALOG_H
*/

/*#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSerialPort>
#include "resource.h" // Include Resource class for quantity update
//#include "mainwindow.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_btnSearch_clicked();

private:
    Ui::SearchDialog *ui;
    QSerialPort *serialPort;

    bool queryDatabase(const QString &resourceName, int &resourceId, Resource &resource); // Modified to return resource ID and data
    bool updateResourceQuantity(int resourceId, int currentQuantity, const QString &employeeName); // New method
    void sendToArduino(bool resourceExists);
};

#endif // SEARCHDIALOG_H*/

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSerialPort>
#include "resource.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

signals:
    void resourceUpdated(); // New signal for quantity update

private slots:
    void on_btnSearch_clicked();

private:
    Ui::SearchDialog *ui;
    QSerialPort *serialPort;

    bool queryDatabase(const QString &resourceName, int &resourceId, Resource &resource);
    bool updateResourceQuantity(int resourceId, int currentQuantity, const QString &employeeName);
    void sendToArduino(bool resourceExists);
};

#endif // SEARCHDIALOG_H
