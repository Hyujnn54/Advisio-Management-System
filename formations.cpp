#include "formations.h"
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
// Default constructor
formations::formations()
    : formation(""),           // Empty string for formation
    description(""),          // Empty string for description
    trainer(""),              // Empty string for trainer
    datef(QDate::currentDate()), // Current date as default
    time(0),                 // Empty string for time
    prix(0)                  // Empty string for price
{
}

// Parameterized constructor
formations::formations(
    // Added id parameter
     QString& formation,
     QString& description,
     QString& trainer,
     QDate& datef,
     int& time,
     int& prix)
    : formation(formation),                  // Initialize id with provided value
    description(description),
    trainer(trainer),
    datef(datef),
    time(time),
    prix(prix)
{
}

// Getters
int formations::getIdfor() const {
    return idfor;
}

QString formations::getFormation() const {
    return formation;
}

QString formations::getDescription() const {
    return description;
}

QString formations::getTrainer() const {
    return trainer;
}

QDate formations::getDatef() const {
    return datef;
}

int formations::getTime() const {
    return time;
}

int formations::getPrix() const {
    return prix;
}
bool formations::ajoutforma(){

    QSqlQuery query;
    query.prepare("INSERT INTO formations (formation, description, trainer, datef, time, prix) "
                  "VALUES (:formation, :description, :trainer, :datef, :time, :prix)");

    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);


    return query.exec();
}
QSqlQueryModel* formations::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Make sure database is open
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database connection is not open when trying to display formations!";
        model->setQuery(QString()); // Empty query
        return model;
    }

    model->setQuery("SELECT * FROM formations");

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error when displaying formations:" << model->lastError().text();
    }

    return model;
}
bool formations::deleteFormation(int idfor) {
    QSqlQuery query;
    query.prepare("DELETE FROM FORMATIONS WHERE IDFORM = :id");
    query.bindValue(":id", idfor);

    if (!query.exec()) {
        qDebug() << "Delete failed:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No row found with ID:" << idfor;
        return false;
    }

    return true;
}

bool formations::updateFormation(int idfor, const QString& newFormation, const QString& newDescription,
                                 const QString& newTrainer, const QDate& newDatef, int newTime, int newPrix) {
    QSqlQuery query;
    query.prepare("UPDATE formations SET formation = :formation, description = :description, "
                  "trainer = :trainer, datef = :datef, time = :time, prix = :prix "
                  "WHERE IDFORM = :id");

    query.bindValue(":formation", newFormation);
    query.bindValue(":description", newDescription);
    query.bindValue(":trainer", newTrainer);
    query.bindValue(":datef", newDatef);
    query.bindValue(":time", newTime);
    query.bindValue(":prix", newPrix);
    query.bindValue(":id", idfor);

    if (!query.exec()) {
        qDebug() << "Failed to update formation:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No row found to update with ID:" << idfor;
        return false;
    }

    return true;
}
bool formations::exists(int idfor) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM formations WHERE idform = :idform");  // Use 'idform'
    query.bindValue(":idform", idfor);

    if (query.exec()) {
        if (query.next()) {
            int count = query.value(0).toInt();
            qDebug() << "ID check for" << idfor << ": count =" << count; // Debug info
            return count > 0;  // Return true if ID exists
        }
    } else {
        qDebug() << "Query execution failed:" << query.lastError().text(); // Debug error
    }
    return false;
}

