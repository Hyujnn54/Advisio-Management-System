#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "resource.h"
#include <QFileDialog>
#include <QPixmap>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSelectImage_clicked();
    void on_confirmFormButton_clicked();
    void refreshTableWidget();




private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
