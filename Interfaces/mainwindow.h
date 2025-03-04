#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "meeting.h"
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleAddButtonClick();
    void refreshTableWidget(); // Add method to refresh the table display
    void handleDeleteButtonClick();
    void handleUpdateButtonClick();
    void updateInputFields() ;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
