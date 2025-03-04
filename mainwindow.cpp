#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formations.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->add, SIGNAL(clicked()), this, SLOT(on_addButtonclicked()));
    QObject::connect(ui->deletef, SIGNAL(clicked()), this, SLOT(on_deleteButtonClicked()));
    QObject::connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(on_updateButtonClicked()));


}

MainWindow::~MainWindow()
{
    delete ui;

}
void MainWindow::on_addButtonclicked(){

    QString formation = ui->format->text();
    QString description = ui->des->toPlainText();
    QString trainer = ui->tr->text();
    QDate datef = ui->date->date();

    int time = ui->timeb->text().toInt();
    int prix = ui->prixb->text().toInt();


    formations f(formation, description, trainer, datef, time, prix);

    if (f.ajoutforma()) {
        QMessageBox::information(this, "Succès", "Formation ajoutée avec succès !");
        refreshTableWidget();  // Refresh after adding
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de formation.");
    }

}
void MainWindow::refreshTableWidget() {
    formations f;
    QSqlQueryModel* model = f.afficher();

    // Set up the tableWidget with data from the model
    ui->tabtr->setRowCount(0); // Clear existing rows

    // Copy data from model to tableWidget
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tabtr->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tabtr->setItem(row, col, item);
        }
    }
}
#include <QInputDialog>

void MainWindow::on_deleteButtonClicked() {
    bool ok;
    int idfor = QInputDialog::getInt(this, "Supprimer Formation", "Entrez l'ID de la formation à supprimer:", 0, 0, 1000000, 1, &ok);

    if (!ok) {
        return; // User cancelled the dialog
    }

    formations f;
    if (!f.exists(idfor)) {  // Check if the formation ID exists
        QMessageBox::warning(this, "Erreur", "L'ID de la formation n'existe pas !");
        return;
    }

    if (formations::deleteFormation(idfor)) {
        QMessageBox::information(this, "Succès", "Formation supprimée avec succès !");
        refreshTableWidget();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de la formation.");
    }


}

void MainWindow::on_updateButtonClicked()
{
    bool ok;
    int idfor = QInputDialog::getInt(this, "Mettre à jour la formation", "Entrez l'ID de la formation:", 0, 0, 1000000, 1, &ok);
    if (!ok) return;

    formations f;
    if (!f.exists(idfor)) {  // Check if the formation ID exists
        QMessageBox::warning(this, "Erreur", "L'ID de la formation n'existe pas !");
        return;
    }

    // Dialog container
    QDialog dialog(this);
    dialog.setWindowTitle("Modifier la formation");

    QFormLayout form(&dialog);

    // Fields
    QLineEdit formationEdit;
    QLineEdit descriptionEdit;
    QLineEdit trainerEdit;
    QDateEdit dateEdit;
    dateEdit.setDate(QDate::currentDate());

    QSpinBox timeSpin;
    timeSpin.setRange(0, 1000);
    QSpinBox priceSpin;
    priceSpin.setRange(0, 100000);

    // Add fields to layout
    form.addRow("Nom Formation:", &formationEdit);
    form.addRow("Description:", &descriptionEdit);
    form.addRow("Formateur:", &trainerEdit);
    form.addRow("Date:", &dateEdit);
    form.addRow("Durée (heures):", &timeSpin);
    form.addRow("Prix:", &priceSpin);

    // Buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Show dialog and handle result
    if (dialog.exec() == QDialog::Accepted) {
        QString newFormation = formationEdit.text();
        QString newDescription = descriptionEdit.text();
        QString newTrainer = trainerEdit.text();
        QDate newDate = dateEdit.date();
        int newTime = timeSpin.value();
        int newPrix = priceSpin.value();
        if (!formations::exists(idfor)) {  // Check if the formation ID exists
            QMessageBox::warning(this, "Erreur", "L'ID de la formation n'existe pas !");
            return;
        }

        if (f.updateFormation(idfor, newFormation, newDescription, newTrainer, newDate, newTime, newPrix)) {
            QMessageBox::information(this, "Succès", "Formation mise à jour avec succès !");
            refreshTableWidget();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la formation.");
        }
    }
}





