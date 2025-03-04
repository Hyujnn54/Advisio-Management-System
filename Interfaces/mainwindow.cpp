#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "meeting.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the button's clicked signal to the handleAddButtonClick slot
    connect(ui->AddButton, &QPushButton::clicked, this, &MainWindow::handleAddButtonClick);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClick);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::handleUpdateButtonClick);

    // Connect the tableWidget's itemSelectionChanged signal to the updateInputFields slot
    connect(ui->tableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateInputFields);

    // Initialize the tableWidget by setting the model
    refreshTableWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTableWidget() {
    meeting m;
    QSqlQueryModel* model = m.afficher();

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

    delete model; // Clean up the model
}

void MainWindow::handleAddButtonClick() {
    // Get values from the UI
    QString title = ui->title->text();
    QString organiser = ui->organiser->text();
    QString participant = ui->participant->text();
    QString agenda = ui->agenda->currentText();

    bool conversionOk = false;
    int duration = ui->duration->text().toInt(&conversionOk);

    // Validate inputs with detailed feedback
    QStringList validationErrors;

    if (title.isEmpty()) {
        validationErrors << "Title cannot be empty";
    }

    if (organiser.isEmpty()) {
        validationErrors << "Organiser cannot be empty";
    }

    if (!conversionOk || duration <= 0) {
        validationErrors << "Duration must be a valid positive number";
    }

    if (!validationErrors.isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please correct the following errors:\n• " + validationErrors.join("\n• "));
        return;
    }

    // Add debugging output
    qDebug() << "Adding meeting with values:"
             << "\nTitle:" << title << (title.isEmpty() ? " (EMPTY)" : "")
             << "\nOrganiser:" << organiser << (organiser.isEmpty() ? " (EMPTY)" : "")
             << "\nParticipant:" << participant << (participant.isEmpty() ? " (EMPTY)" : "")
             << "\nAgenda:" << agenda << (agenda.isEmpty() ? " (EMPTY)" : "")
             << "\nDuration:" << duration;

    // Create and add the meeting
    meeting m(title, organiser, participant, agenda, duration);

    if (m.add()) {
        QMessageBox::information(this, "Success", "Meeting added successfully.");

        // Clear the form fields after successful addition
        ui->title->clear();
        ui->organiser->clear();
        ui->participant->clear();
        ui->duration->clear();
        ui->agenda->setCurrentIndex(0);

        // Refresh the table to show the new meeting
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add meeting. Check the database connection.");
    }
}

void MainWindow::handleDeleteButtonClick() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to delete.");
        return;
    }

    // Get the ID of the selected meeting
    int id = ui->tableWidget->item(row, 0)->text().toInt();

    // Delete the meeting from the database
    meeting m;
    if (m.delet(id)) {
        QMessageBox::information(this, "Success", "Meeting deleted successfully.");

        // Refresh the table to remove the deleted meeting
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete meeting. Check the database connection.");
    }
}

void MainWindow::handleUpdateButtonClick() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to update.");
        return;
    }

    // Get the ID of the selected meeting
    int id = ui->tableWidget->item(row, 0)->text().toInt();

    // Get values from the UI
    QString title = ui->title->text();
    QString organiser = ui->organiser->text();
    QString participant = ui->participant->text();
    QString agenda = ui->agenda->currentText();

    bool conversionOk = false;
    int duration = ui->duration->text().toInt(&conversionOk);

    // Validate inputs with detailed feedback
    QStringList validationErrors;

    if (title.isEmpty()) {
        validationErrors << "Title cannot be empty";
    }

    if (organiser.isEmpty()) {
        validationErrors << "Organiser cannot be empty";
    }

    if (!conversionOk || duration <= 0) {
        validationErrors << "Duration must be a valid positive number";
    }

    if (!validationErrors.isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please correct the following errors:\n• " + validationErrors.join("\n• "));
        return;
    }

    // Add debugging output
    qDebug() << "Updating meeting with values:"
             << "\nID:" << id
             << "\nTitle:" << title << (title.isEmpty() ? " (EMPTY)" : "")
             << "\nOrganiser:" << organiser << (organiser.isEmpty() ? " (EMPTY)" : "")
             << "\nParticipant:" << participant << (participant.isEmpty() ? " (EMPTY)" : "")
             << "\nAgenda:" << agenda << (agenda.isEmpty() ? " (EMPTY)" : "")
             << "\nDuration:" << duration;

    // Create and update the meeting
    meeting m;
    m.setId(id);
    m.setTitle(title);
    m.setOrganiser(organiser);
    m.setParticipant(participant);
    m.setAgenda(agenda);
    m.setDuration(duration);

    if (m.update()) {
        QMessageBox::information(this, "Success", "Meeting updated successfully.");

        // Refresh the table to show the updated meeting
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update meeting. Check the database connection.");
    }
}

void MainWindow::updateInputFields() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        return;
    }

    // Update the input fields with the values from the selected row
    ui->title->setText(ui->tableWidget->item(row, 1)->text());
    ui->organiser->setText(ui->tableWidget->item(row, 2)->text());
    ui->participant->setText(ui->tableWidget->item(row, 3)->text());
    ui->agenda->setCurrentText(ui->tableWidget->item(row, 4)->text());
    ui->duration->setText(ui->tableWidget->item(row, 5)->text());
}
