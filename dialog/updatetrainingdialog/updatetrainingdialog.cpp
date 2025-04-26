#include "UpdateTrainingDialog.h"

UpdateTrainingDialog::UpdateTrainingDialog(QWidget *parent)
    : QDialog(parent),
    nameEdit(new QLineEdit(this)),
    descriptionEdit(new QLineEdit(this)),
    trainerEdit(new QLineEdit(this)),
    dateEdit(new QDateEdit(this)),
    timeSpinBox(new QSpinBox(this)),
    prixSpinBox(new QDoubleSpinBox(this))
{
    setWindowTitle("Update Training");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Description:", descriptionEdit);
    formLayout->addRow("Trainer:", trainerEdit);
    dateEdit->setCalendarPopup(true);
    formLayout->addRow("Date:", dateEdit);
    timeSpinBox->setRange(1, 1000);
    formLayout->addRow("Duration (hours):", timeSpinBox);
    prixSpinBox->setRange(0.0, 10000.0);
    prixSpinBox->setDecimals(2);
    formLayout->addRow("Price:", prixSpinBox);

    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void UpdateTrainingDialog::setTrainingData(const QString &name, const QString &description, const QString &trainer,
                                           const QDate &date, int time, double prix)
{
    nameEdit->setText(name);
    descriptionEdit->setText(description);
    trainerEdit->setText(trainer);
    dateEdit->setDate(date);
    timeSpinBox->setValue(time);
    prixSpinBox->setValue(prix);
}

QString UpdateTrainingDialog::getName() const
{
    return nameEdit->text().trimmed();
}

QString UpdateTrainingDialog::getDescription() const
{
    return descriptionEdit->text().trimmed();
}

QString UpdateTrainingDialog::getTrainer() const
{
    return trainerEdit->text().trimmed();
}

QDate UpdateTrainingDialog::getDate() const
{
    return dateEdit->date();
}

int UpdateTrainingDialog::getTime() const
{
    return timeSpinBox->value();
}

double UpdateTrainingDialog::getPrix() const
{
    return prixSpinBox->value();
}
