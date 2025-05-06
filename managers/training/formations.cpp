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
    : idfor(0),
    formation(""),
    description(""),
    trainer(""),
    datef(QDate::currentDate()),
    time(0),
    prix(0.0)
{
}

// Parameterized constructor
formations::formations(int idfor, QString &formation, QString &description, QString &trainer, QDate &datef, int &time, double &prix)
    : idfor(idfor),
    formation(formation),
    description(description),
    trainer(trainer),
    datef(datef),
    time(time),
    prix(prix)
{
}

// Getters
int formations::getIdfor() const
{
    return idfor;
}

QString formations::getFormation() const
{
    return formation;
}

QString formations::getDescription() const
{
    return description;
}

QString formations::getTrainer() const
{
    return trainer;
}

QDate formations::getDatef() const
{
    return datef;
}

int formations::getTime() const
{
    return time;
}

double formations::getPrix() const
{
    return prix;
}

// Setters
void formations::setIdfor(int idfor)
{
    this->idfor = idfor;
}

void formations::setFormation(const QString &formation)
{
    this->formation = formation;
}

void formations::setDescription(const QString &description)
{
    this->description = description;
}

void formations::setTrainer(const QString &trainer)
{
    this->trainer = trainer;
}

void formations::setDatef(const QDate &datef)
{
    this->datef = datef;
}

void formations::setTime(int time)
{
    this->time = time;
}

void formations::setPrix(double prix)
{
    this->prix = prix;
}

bool formations::ajoutforma()
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX) "
                  "VALUES (FORMATIONS_SEQ.NEXTVAL, :formation, :description, :trainer, :datef, :time, :prix)");

    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec())
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel *formations::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qDebug() << "Database not open!";
        model->setQuery(QString(), db);
        return model;
    }
    model->setQuery("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS", db);
    if (model->lastError().isValid())
    {
        qDebug() << "SQL Error:" << model->lastError().text();
    }
    qDebug() << "Rows fetched:" << model->rowCount() << "Columns:" << model->columnCount();
    for (int row = 0; row < model->rowCount(); ++row)
    {
        qDebug() << "Row" << row << ":"
                 << model->index(row, 0).data().toString() << ","
                 << model->index(row, 1).data().toString() << ","
                 << model->index(row, 2).data().toString() << ","
                 << model->index(row, 3).data().toString() << ","
                 << model->index(row, 4).data().toString() << ","
                 << model->index(row, 5).data().toString() << ","
                 << model->index(row, 6).data().toString();
    }
    return model;
}

QSqlQueryModel *formations::searchByType(const QString &type)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS WHERE FORMATION = :formation");
    query.bindValue(":formation", type);
    
    // Exécuter la requête et définir le modèle
    if (query.exec()) {
        model->setQuery(std::move(query)); // Utiliser std::move pour éviter la copie
    } else {
        qDebug() << "SQL Error in searchByType:" << query.lastError().text();
        model->setQuery(QSqlQuery()); // Utiliser un constructeur par défaut au lieu de QSqlQuery(db)
    }
    return model;
}

QSqlQueryModel *formations::searchByTrainer(const QString &trainer)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS WHERE TRAINER = :trainer");
    query.bindValue(":trainer", trainer);
    
    // Exécuter la requête et définir le modèle
    if (query.exec()) {
        model->setQuery(std::move(query)); // Utiliser std::move pour éviter la copie
    } else {
        qDebug() << "SQL Error in searchByTrainer:" << query.lastError().text();
        model->setQuery(QSqlQuery()); // Utiliser un constructeur par défaut
    }
    return model;
}

QSqlQueryModel *formations::getTrainingsForDate(const QDate &date)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS WHERE DATEF = :datef");
    query.bindValue(":datef", date);
    
    // Exécuter la requête et définir le modèle
    if (query.exec()) {
        model->setQuery(std::move(query)); // Utiliser std::move pour éviter la copie
    } else {
        qDebug() << "SQL Error in getTrainingsForDate:" << query.lastError().text();
        model->setQuery(QSqlQuery()); // Utiliser un constructeur par défaut
    }
    return model;
}

/*bool formations::deleteFormation(int idfor)
{
    QSqlQuery query;

    // Step 1: Delete dependent records from TRAINING_RESOURCES
    query.prepare("DELETE FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :idfor");
    query.bindValue(":idfor", idfor);
    if (!query.exec()) {
        qDebug() << "Delete from TRAINING_RESOURCES failed: " << query.lastError().text();
        return false;
    }

    // Step 2: Delete the formation from FORMATIONS
    query.prepare("DELETE FROM AHMED.FORMATIONS WHERE IDFORM = :idfor");
    query.bindValue(":idfor", idfor);
    if (!query.exec()) {
        qDebug() << "Delete failed: " << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No row found with ID:" << idfor;
        return false;
    }

    return true;
}*/
/*bool formations::deleteFormation(int idfor)
{
    QSqlQuery query;

    // Step 1: Check for child records in TRAINING_RESOURCES
    query.prepare("SELECT COUNT(*) FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :idfor");
    query.bindValue(":idfor", idfor);
    if (!query.exec()) {
        qDebug() << "Failed to check child records in TRAINING_RESOURCES for TRAINING_ID" << idfor << ":" << query.lastError().text();
        return false;
    }
    query.next();
    int childCount = query.value(0).toInt();
    qDebug() << "Found" << childCount << "child records in TRAINING_RESOURCES for TRAINING_ID" << idfor;

    // Step 2: Delete dependent records from TRAINING_RESOURCES
    query.prepare("DELETE FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :idfor");
    query.bindValue(":idfor", idfor);
    if (!query.exec()) {
        qDebug() << "Delete from TRAINING_RESOURCES failed for TRAINING_ID" << idfor << ":" << query.lastError().text();
        return false;
    }
    int rowsAffected = query.numRowsAffected();
    qDebug() << "Deleted" << rowsAffected << "rows from TRAINING_RESOURCES for TRAINING_ID" << idfor;

    // Step 3: Verify child records are gone
    query.prepare("SELECT COUNT(*) FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :idfor");
    query.bindValue(":idfor", idfor);
    if (!query.exec()) {
        qDebug() << "Failed to verify child records in TRAINING_RESOURCES after deletion for TRAINING_ID" << idfor << ":" << query.lastError().text();
        return false;
    }
    query.next();
    childCount = query.value(0).toInt();
    qDebug() << "After deletion, found" << childCount << "child records in TRAINING_RESOURCES for TRAINING_ID" << idfor;
    if (childCount > 0) {
        qDebug() << "Child records remain in TRAINING_RESOURCES - deletion did not work as expected";
        return false;
    }

    // Step 4: Delete the formation from FORMATIONS
    query.prepare("DELETE FROM AHMED.FORMATIONS WHERE IDFORM = :idfor");
    query.bindValue(":idfor", idfor);
    if (!query.exec()) {
        qDebug() << "Delete from FORMATIONS failed for IDFORM" << idfor << ":" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No row found in FORMATIONS with IDFORM:" << idfor;
        return false;
    }

    qDebug() << "Successfully deleted formation with IDFORM" << idfor;
    return true;
}*/

bool formations::updateFormation(int idfor, const QString &newFormation, const QString &newDescription,
                                 const QString &newTrainer, const QDate &newDatef, int newTime, double newPrix)
{
    QSqlQuery query;
    query.prepare("UPDATE AHMED.FORMATIONS SET FORMATION = :formation, DESCRIPTION = :description, "
                  "TRAINER = :trainer, DATEF = :datef, TIME = :time, PRIX = :prix "
                  "WHERE IDFORM = :id");

    query.bindValue(":formation", newFormation);
    query.bindValue(":description", newDescription);
    query.bindValue(":trainer", newTrainer);
    query.bindValue(":datef", newDatef);
    query.bindValue(":time", newTime);
    query.bindValue(":prix", newPrix);
    query.bindValue(":id", idfor);

    if (!query.exec())
    {
        qDebug() << "Failed to update formation:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        qDebug() << "No row found to update with ID:" << idfor;
        return false;
    }

    return true;
}

bool formations::exists(int idfor)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM AHMED.FORMATIONS WHERE IDFORM = :idform");
    query.bindValue(":idform", idfor);

    if (query.exec())
    {
        if (query.next())
        {
            int count = query.value(0).toInt();
            qDebug() << "ID check for" << idfor << ": count =" << count;
            return count > 0;
        }
    }
    else
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }
    return false;
}

// Methods for TrainingManager
QPair<bool, QString> formations::ajouter(const QString &formation, const QString &description, const QString &trainer,
                                         const QDate &datef, int time, double prix)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX) "
                  "VALUES (FORMATIONS_SEQ.NEXTVAL, :formation, :description, :trainer, :datef, :time, :prix)");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec())
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return qMakePair(false, query.lastError().text());
    }
    return qMakePair(true, QString("Training added successfully"));
}

QPair<bool, QString> formations::ajouter(const QString &formation, const QString &description, int trainerId,
                                         const QDate &datef, int time, double prix)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, EPLOYEE_ID, DATEF, TIME, PRIX) "
                  "VALUES (FORMATIONS_SEQ.NEXTVAL, :formation, :description, :trainerId, :datef, :time, :prix)");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainerId", trainerId);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec())
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return qMakePair(false, query.lastError().text());
    }
    return qMakePair(true, QString("Training added successfully"));
}

QPair<bool, QString> formations::ajouter(const QString &formation, const QString &description, int trainerId, int clientId, int resourceId,
                                         const QDate &datef, int time, double prix)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, EPLOYEE_ID, CLIENT_ID, DATEF, TIME, PRIX) "
                  "VALUES (FORMATIONS_SEQ.NEXTVAL, :formation, :description, :trainerId, :clientId, :datef, :time, :prix)");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainerId", trainerId);
    query.bindValue(":clientId", clientId);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec())
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return qMakePair(false, query.lastError().text());
    }
    return qMakePair(true, QString("Training added successfully"));
}

QPair<bool, QString> formations::ajouter(const QString &formation, const QString &description, const QString &trainer, int trainerId, int clientId, int /*resourceId*/,
                                         const QDate &datef, int time, double prix)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, TRAINER, EPLOYEE_ID, CLIENT_ID, DATEF, TIME, PRIX) "
                  "VALUES (FORMATIONS_SEQ.NEXTVAL, :formation, :description, :trainer, :trainerId, :clientId, :datef, :time, :prix)");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":trainerId", trainerId);
    query.bindValue(":clientId", clientId);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec())
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return qMakePair(false, query.lastError().text());
    }
    return qMakePair(true, QString("Training added successfully"));
}

QPair<bool, QString> formations::ajouterWithId(int idform, const QString &formation, const QString &description, const QString &trainer, int trainerId, int clientId,
                                               const QDate &datef, int time, double prix)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, TRAINER, EMPLOYEE_ID, CLIENT_ID, DATEF, TIME, PRIX) "
                  "VALUES (:idform, :formation, :description, :trainer, :trainerId, :clientId, :datef, :time, :prix)");
    query.bindValue(":idform", idform);
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":trainerId", trainerId);
    query.bindValue(":clientId", clientId);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec()) {
        qDebug() << "SQL Error (ajouterWithId):" << query.lastError().text();
        return qMakePair(false, query.lastError().text());
    }
    return qMakePair(true, QString("Training added successfully"));
}

bool formations::modifier(const QString &oldFormation, const QString &newFormation, const QString &newDescription,
                          const QString &newTrainer, const QDate &newDatef, int newTime, double newPrix)
{
    QSqlQuery query;
    query.prepare("UPDATE AHMED.FORMATIONS SET FORMATION = :newFormation, DESCRIPTION = :description, "
                  "TRAINER = :trainer, DATEF = :datef, TIME = :time, PRIX = :prix "
                  "WHERE FORMATION = :oldFormation");
    query.bindValue(":newFormation", newFormation);
    query.bindValue(":description", newDescription);
    query.bindValue(":trainer", newTrainer);
    query.bindValue(":datef", newDatef);
    query.bindValue(":time", newTime);
    query.bindValue(":prix", newPrix);
    query.bindValue(":oldFormation", oldFormation);

    if (!query.exec())
    {
        qDebug() << "Update failed:" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

/*bool formations::supprimer(const QString &formation)
{
    QSqlQuery query;
    query.prepare("DELETE FROM AHMED.FORMATIONS WHERE FORMATION = :formation");
    query.bindValue(":formation", formation);

    if (!query.exec())
    {
        qDebug() << "Delete failed:" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}*/
bool formations::supprimer(const QString &formation)
{
    QSqlQuery query;

    // Step 1: Retrieve the IDFORM(s) of the formation(s) matching the given FORMATION value
    query.prepare("SELECT IDFORM FROM AHMED.FORMATIONS WHERE FORMATION = :formation");
    query.bindValue(":formation", formation);
    if (!query.exec()) {
        qDebug() << "Failed to retrieve IDFORM for FORMATION" << formation << ":" << query.lastError().text();
        return false;
    }

    // Collect all IDFORM values (in case multiple formations have the same name)
    QList<int> idforms;
    while (query.next()) {
        idforms.append(query.value(0).toInt());
    }

    if (idforms.isEmpty()) {
        qDebug() << "No formation found with FORMATION:" << formation;
        return false;
    }

    // Step 2: Delete dependent records from TRAINING_RESOURCES for each IDFORM
    for (int idfor : idforms) {
        query.prepare("DELETE FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :idfor");
        query.bindValue(":idfor", idfor);
        if (!query.exec()) {
            qDebug() << "Delete from TRAINING_RESOURCES failed for TRAINING_ID" << idfor << ":" << query.lastError().text();
            return false;
        }
        int rowsAffected = query.numRowsAffected();
        qDebug() << "Deleted" << rowsAffected << "rows from TRAINING_RESOURCES for TRAINING_ID" << idfor;
    }

    // Step 3: Delete the formation(s) from FORMATIONS
    query.prepare("DELETE FROM AHMED.FORMATIONS WHERE FORMATION = :formation");
    query.bindValue(":formation", formation);
    if (!query.exec()) {
        qDebug() << "Delete from FORMATIONS failed for FORMATION" << formation << ":" << query.lastError().text();
        return false;
    }

    int rowsDeleted = query.numRowsAffected();
    qDebug() << "Deleted" << rowsDeleted << "formations with FORMATION" << formation;

    return rowsDeleted > 0;
}
