#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "formations.h"
#include <QMainWindow>

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
    void on_addButtonclicked();

private:
    Ui::MainWindow *ui;
    formations formation;
private:
    // Declare refreshTableWidget here with the exact signature
    void refreshTableWidget();


private slots:
    void on_deleteButtonClicked();
private slots:
    void on_updateButtonClicked();






};

#endif // MAINWINDOW_H
