#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "resource.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <Qstring>
#include <QDate>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QTableView>
#include <QInputDialog>
#include <QBuffer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //loadResourcesIntoTable();
        //refreshTableWidget();
   //connect(ui->confirmFormButton, &QPushButton::clicked, this, &MainWindow::on_confirmFormButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnSelectImage_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap image(filePath);
        ui->lblImagePreview->setPixmap(image.scaled(ui->lblImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}



/*void MainWindow::on_confirmFormButton_clicked() {
    // Retrieve values from the form
    QString name = ui->nameLineEdit->text();
    QString type = ui->typeComboBox->currentText();
    QString brand = ui->brandLineEdit->text();
    int quantity = ui->quantityLineEdit->text().toInt(); // Ensure conversion is correct
    QString purchase_date = ui->purchaseDateEdit->date().toString("yyyy-MM-dd");


    // Debugging output
    qDebug() << "Converted quantity:" << quantity;
    qDebug() << "Formatted date:" << purchase_date;

    // Prepare and execute your SQL query here
    QSqlQuery query;
    query.prepare("INSERT INTO resources (name, type, brand, quantity, purchase_date) "
                  "VALUES (:name, :type, :brand, :quantity, TO_DATE(:purchase_date, 'YYYY-MM-DD'))");

    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":brand", brand);
    query.bindValue(":quantity", quantity);
    query.bindValue(":purchase_date", purchase_date);
   refreshTableWidget();

    if (query.exec()) {
        qDebug() << "Resource added successfully!";
    } else {
        qDebug() << "Failed to add resource:" << query.lastError().text();
    }
}*/


void MainWindow::on_confirmFormButton_clicked() {
    // Retrieve values from the form
    QString name = ui->nameLineEdit->text();
    QString type = ui->typeComboBox->currentText();
    QString brand = ui->brandLineEdit->text();
    int quantity = ui->quantityLineEdit->text().toInt(); // Ensure conversion is correct
    QString purchase_date = ui->purchaseDateEdit->date().toString("yyyy-MM-dd");

    // Debugging output
    qDebug() << "Converted quantity:" << quantity;
    qDebug() << "Formatted date:" << purchase_date;

    // Prepare and execute your SQL query here
    QSqlQuery query;
    query.prepare("INSERT INTO resources (name, type, brand, quantity, purchase_date) "
                  "VALUES (:name, :type, :brand, :quantity, TO_DATE(:purchase_date, 'YYYY-MM-DD'))");

    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":brand", brand);
    query.bindValue(":quantity", quantity);
    query.bindValue(":purchase_date", purchase_date);

    if (query.exec()) {
        qDebug() << "Resource added successfully!";

        // Clear form fields after successful insertion
        ui->nameLineEdit->clear();
        ui->typeComboBox->setCurrentIndex(0); // Reset to default selection
        ui->brandLineEdit->clear();
        ui->quantityLineEdit->clear();
        ui->purchaseDateEdit->setDate(QDate::currentDate()); // Reset to current date

        refreshTableWidget();
    } else {
        qDebug() << "Failed to add resource:" << query.lastError().text();
    }
}



void MainWindow::refreshTableWidget() {
    Resource R;
    QSqlQueryModel* model = R.afficher();

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows

    // Copy data from model to tableWidget
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }
    delete model;
}
