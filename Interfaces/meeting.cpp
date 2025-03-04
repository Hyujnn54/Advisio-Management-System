#include "meeting.h"
#include <QSqlError>
#include <QDebug>

// Parameterized constructor
meeting::meeting(QString title, QString organiser, QString participant, QString agenda, int duration)
    : title(title), organiser(organiser), participant(participant), agenda(agenda), duration(duration)
{
}

// Getters
int meeting::getId() const { return id; }
QString meeting::getTitle() const { return title; }
QString meeting::getOrganiser() const { return organiser; }
QString meeting::getParticipant() const { return participant; }
QString meeting::getAgenda() const { return agenda; }
int meeting::getDuration() const { return duration; }

// Setters
void meeting::setId(const int &id) { this->id = id; }
void meeting::setTitle(const QString &title) { this->title = title; }
void meeting::setOrganiser(const QString &organiser) { this->organiser = organiser; }
void meeting::setParticipant(const QString &participant) { this->participant = participant; }
void meeting::setAgenda(const QString &agenda) { this->agenda = agenda; }
void meeting::setDuration(int duration) { this->duration = duration; }

// Functionalities
bool meeting::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO meeting (title, organiser, participant, agenda, duration) "
                  "VALUES (:title, :organiser, :participant, :agenda, :duration)");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);

    if (!query.exec()) {
        qDebug() << "Add meeting failed: " << query.lastError();
        return false;
    }

    return true;
}

QSqlQueryModel* meeting::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM meeting");

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error when displaying meetings:" << model->lastError().text();
    }

    return model;
}

bool meeting::delet(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM meeting WHERE id = :id");
    query.bindValue(":id", id);

    bool success = query.exec();

    if (!success) {
        qDebug() << "SQL Error when deleting meeting:" << query.lastError().text();
    }

    return success;
}

bool meeting::update()
{
    QSqlQuery query;
    query.prepare("UPDATE meeting SET title = :title, organiser = :organiser, participant = :participant, agenda = :agenda, duration = :duration WHERE id = :id");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update meeting failed: " << query.lastError();
        return false;
    }

    return true;
}
