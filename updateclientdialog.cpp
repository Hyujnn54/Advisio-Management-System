#include "updateclientdialog.h"
#include "ui_updateclientdialog.h"

UpdateClientDialog::UpdateClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateClientDialog)
{
    ui->setupUi(this);
    setWindowTitle("Update Client");

    // Connect buttons
    connect(ui->confirmButton, &QPushButton::clicked, this, &UpdateClientDialog::on_confirmButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &UpdateClientDialog::on_cancelButton_clicked);
}

UpdateClientDialog::~UpdateClientDialog()
{
    delete ui;
}

void UpdateClientDialog::setClientData(const QString &name, const QString &sector, const QString &contactInfo, const QDateTime &consultationDateTime, int consultant) {
    ui->nameLineEdit->setText(name);
    ui->sectorLineEdit->setText(sector);
    ui->contactLineEdit->setText(contactInfo);
    ui->consultationDateTime->setDateTime(consultationDateTime);
    ui->consultantLineEdit->setText(QString::number(consultant));
}

QString UpdateClientDialog::getName() const {
    return ui->nameLineEdit->text();
}

QString UpdateClientDialog::getSector() const {
    return ui->sectorLineEdit->text();
}

QString UpdateClientDialog::getContactInfo() const {
    return ui->contactLineEdit->text();
}

QDateTime UpdateClientDialog::getConsultationDateTime() const {
    return ui->consultationDateTime->dateTime();
}

int UpdateClientDialog::getConsultant() const {
    return ui->consultantLineEdit->text().toInt();
}

void UpdateClientDialog::on_confirmButton_clicked()
{
    accept(); // Close dialog with accept status
}

void UpdateClientDialog::on_cancelButton_clicked()
{
    reject(); // Close dialog with reject status
}
