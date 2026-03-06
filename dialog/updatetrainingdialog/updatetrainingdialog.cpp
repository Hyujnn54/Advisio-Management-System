#include "UpdateTrainingDialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

UpdateTrainingDialog::UpdateTrainingDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::UpdateTrainingDialog),
    trainingId(-1)
{
    ui->setupUi(this);
    // Set a minimum size and allow resizing
    setMinimumSize(400, 400);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Connect Confirm/Cancel buttons
    connect(ui->confirmButton, &QPushButton::clicked, this, &UpdateTrainingDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    qDebug() << "UpdateTrainingDialog constructed with size:" << size();
}

void UpdateTrainingDialog::setTrainingData(const QString &name, const QString &description, const QString &trainer,
                                           const QDate &date, int time, double prix)
{
    ui->formationLineEdit->setText(name);
    ui->descriptionLineEdit->setText(description);
    ui->trainerLineEdit->setText(trainer);
    ui->dateEdit->setDate(date);
    ui->timeSpinBox->setValue(time);
    ui->prixSpinBox->setValue(prix);
}

QString UpdateTrainingDialog::getName() const
{
    return ui->formationLineEdit->text().trimmed();
}

QString UpdateTrainingDialog::getDescription() const
{
    return ui->descriptionLineEdit->text().trimmed();
}

QString UpdateTrainingDialog::getTrainer() const
{
    return ui->trainerLineEdit->text().trimmed();
}

QDate UpdateTrainingDialog::getDate() const
{
    return ui->dateEdit->date();
}

int UpdateTrainingDialog::getTime() const
{
    return ui->timeSpinBox->value();
}

double UpdateTrainingDialog::getPrix() const
{
    return ui->prixSpinBox->value();
}

void UpdateTrainingDialog::setResourceData(int trainingId) {
    this->trainingId = trainingId; // Ensure trainingId is set
    // Nettoyer le layout
    QLayoutItem *child;
    while ((child = ui->resourcesVBoxLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QMap<int, int> usedResources;
    QSqlQuery usedQuery;
    usedQuery.prepare("SELECT RESOURCE_ID, QUANTITY FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :tid");
    usedQuery.bindValue(":tid", trainingId);
    if (usedQuery.exec()) {
        while (usedQuery.next()) {
            usedResources[usedQuery.value(0).toInt()] = usedQuery.value(1).toInt();
        }
    } else {
        qDebug() << "Failed to fetch used resources:" << usedQuery.lastError().text();
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery resQuery(db);
    resQuery.prepare("SELECT RESOURCE_ID, NAME FROM AHMED.RESOURCES"); // Added schema prefix
    if (!resQuery.exec()) {
        qDebug() << "Failed to fetch resources:" << resQuery.lastError().text();
        return;
    }

    while (resQuery.next()) {
        int id = resQuery.value(0).toInt();
        QString name = resQuery.value(1).toString();
        QWidget* rowWidget = new QWidget();
        QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
        QCheckBox* checkBox = new QCheckBox(name);
        checkBox->setObjectName(QString("resourceCheckBox_%1").arg(id));
        QSpinBox* spinBox = new QSpinBox();
        spinBox->setMinimum(1);
        spinBox->setMaximum(1000);
        spinBox->setValue(1);
        spinBox->setObjectName(QString("resourceSpinBox_%1").arg(id));
        spinBox->setEnabled(false);
        rowLayout->addWidget(checkBox);
        rowLayout->addWidget(spinBox);
        rowWidget->setLayout(rowLayout);
        ui->resourcesVBoxLayout->addWidget(rowWidget);
        QObject::connect(checkBox, &QCheckBox::toggled, spinBox, &QSpinBox::setEnabled);
        if (usedResources.contains(id)) {
            checkBox->setChecked(true);
            spinBox->setValue(usedResources[id]);
        }
    }

    // Update layout after adding widgets
    ui->resourcesVBoxLayout->update();
    adjustSize();
    qDebug() << "Updated dialog size after adding resources:" << size();
}

QList<QPair<int, int>> UpdateTrainingDialog::getSelectedResources() const {
    QList<QPair<int, int>> selectedResources;
    for (int i = 0; i < ui->resourcesVBoxLayout->count(); ++i) {
        QWidget* rowWidget = ui->resourcesVBoxLayout->itemAt(i)->widget();
        if (!rowWidget) continue;
        QCheckBox* checkBox = rowWidget->findChild<QCheckBox*>();
        QSpinBox* spinBox = rowWidget->findChild<QSpinBox*>();
        if (checkBox && spinBox && checkBox->isChecked()) {
            QString objName = checkBox->objectName();
            int resourceId = objName.section('_', 1, 1).toInt();
            int quantity = spinBox->value();
            selectedResources.append(qMakePair(resourceId, quantity));
        }
    }
    return selectedResources;
}

void UpdateTrainingDialog::accept() {
    // Valider les champs si besoin
    if (trainingId == -1) {
        QMessageBox::warning(this, "Error", "Training ID not set.");
        return;
    }
    QList<QPair<int, int>> selectedResources = getSelectedResources();
    QSqlQuery delQuery;
    delQuery.prepare("DELETE FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :tid");
    delQuery.bindValue(":tid", trainingId);
    if (!delQuery.exec()) {
        QMessageBox::warning(this, "Database Error", "Error while deleting old resources: " + delQuery.lastError().text());
        return;
    }
    for (const auto& pair : selectedResources) {
        int resourceId = pair.first;
        int quantity = pair.second;
        QSqlQuery linkQuery;
        linkQuery.prepare("INSERT INTO AHMED.TRAINING_RESOURCES (TRAINING_ID, RESOURCE_ID, QUANTITY) VALUES (:tid, :rid, :qty)");
        linkQuery.bindValue(":tid", trainingId);
        linkQuery.bindValue(":rid", resourceId);
        linkQuery.bindValue(":qty", quantity);
        if (!linkQuery.exec()) {
            QMessageBox::warning(this, "Database Error", "Error while adding a resource: " + linkQuery.lastError().text());
            return;
        }
    }
    QDialog::accept();
}

UpdateTrainingDialog::~UpdateTrainingDialog() {
    delete ui;
}
