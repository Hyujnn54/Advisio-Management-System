// managers/meeting/meeting.cpp
#include "meeting.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError> // Ensure this is included
#include "lib/qrcodegen/qrcodegen.hpp"
#include <QPainter>
#include <QDebug>

meeting::meeting()
    : id(-1),
    title(""),
    organiser(""),
    participant(""),
    agenda(""),
    duration(0),
    datem(QDateTime::currentDateTime()),
    employeeId(-1),
    clientId(-1),
    resourceId(-1)
{
}

// Parameterized constructor
meeting::meeting(QString title, QString organiser, QString participant, QString agenda, int duration, QDateTime datem,
                 int employeeId, int clientId, int resourceId)
    : title(title), organiser(organiser), participant(participant), agenda(agenda), duration(duration), datem(datem),
    employeeId(employeeId), clientId(clientId), resourceId(resourceId)
{
}

// Getters
int meeting::getId() const { return id; }
QString meeting::getTitle() const { return title; }
QString meeting::getOrganiser() const { return organiser; }
QString meeting::getParticipant() const { return participant; }
QString meeting::getAgenda() const { return agenda; }
int meeting::getDuration() const { return duration; }
QDateTime meeting::getDatem() const { return datem; }
int meeting::getEmployeeId() const { return employeeId; }
int meeting::getClientId() const { return clientId; }
int meeting::getResourceId() const { return resourceId; }

// Setters
void meeting::setId(const int &id) { this->id = id; }
void meeting::setTitle(const QString &title) { this->title = title; }
void meeting::setOrganiser(const QString &organiser) { this->organiser = organiser; }
void meeting::setParticipant(const QString &participant) { this->participant = participant; }
void meeting::setAgenda(const QString &agenda) { this->agenda = agenda; }
void meeting::setDuration(int duration) { this->duration = duration; }
void meeting::setDatem(const QDateTime &datem) { this->datem = datem; }
void meeting::setEmployeeId(int employeeId) { this->employeeId = employeeId; }
void meeting::setClientId(int clientId) { this->clientId = clientId; }
void meeting::setResourceId(int resourceId) { this->resourceId = resourceId; }

// Functionalities
bool meeting::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO MEETING (TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, DATEM, EMPLOYEE_ID, CLIENT_ID, RESSOURCE_ID) "
                  "VALUES (:title, :organiser, :participant, :agenda, :duration, :datem, :employeeId, :clientId, :resourceId)");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", datem);
    query.bindValue(":employeeId", employeeId >= 0 ? QVariant(employeeId) : QVariant(QMetaType(QMetaType::Int)));
    query.bindValue(":clientId", clientId >= 0 ? QVariant(clientId) : QVariant(QMetaType(QMetaType::Int)));
    query.bindValue(":resourceId", resourceId >= 0 ? QVariant(resourceId) : QVariant(QMetaType(QMetaType::Int)));

    if (!query.exec()) {
        qDebug() << "Add meeting failed: " << query.lastError().text();
        return false;
    }
    return true;
}

QPixmap meeting::generateQRCode() const
{
    using namespace qrcodegen;

    QString qrData = QString("Meeting ID: %1\nTitle: %2\nOrganiser: %3\nParticipant: %4\nAgenda: %5\nDuration: %6 min\nDate: %7")
                         .arg(id)
                         .arg(title)
                         .arg(organiser)
                         .arg(participant)
                         .arg(agenda)
                         .arg(duration)
                         .arg(datem.toString("yyyy-MM-dd hh:mm"));

    try {
        QrCode qr = QrCode::encodeText(qrData.toUtf8().constData(), QrCode::Ecc::MEDIUM);
        const int scale = 10;
        const int size = qr.getSize() * scale;
        if (size <= 0) {
            qDebug() << "QR code size is invalid";
            return QPixmap();
        }

        QPixmap pixmap(size, size);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);

        for (int y = 0; y < qr.getSize(); y++) {
            for (int x = 0; x < qr.getSize(); x++) {
                if (qr.getModule(x, y)) {
                    painter.drawRect(x * scale, y * scale, scale, scale);
                }
            }
        }

        return pixmap;
    } catch (const std::exception &e) {
        qDebug() << "QR code generation failed:" << e.what();
        return QPixmap();
    }
}

QSqlQueryModel* meeting::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query("SELECT ID, TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, DATEM FROM MEETING");
    model->setQuery(std::move(query));
    if (model->lastError().isValid()) {
        qDebug() << "SQL Error in afficher:" << model->lastError().text();
    } else {
        qDebug() << "afficher: Query successful, returned" << model->rowCount() << "rows";
        for (int row = 0; row < model->rowCount(); ++row) {
            qDebug() << "Row" << row << ": ID =" << model->data(model->index(row, 0)).toInt()
            << ", Title =" << model->data(model->index(row, 1)).toString()
            << ", Organiser =" << model->data(model->index(row, 2)).toString()
            << ", Participant =" << model->data(model->index(row, 3)).toString()
            << ", Agenda =" << model->data(model->index(row, 4)).toString()
            << ", Duration =" << model->data(model->index(row, 5)).toInt()
            << ", DATEM =" << model->data(model->index(row, 6)).toDateTime();
        }
    }
    return model;
}

bool meeting::deleteMeeting(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM MEETING WHERE ID = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete meeting failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool meeting::update()
{
    QSqlQuery query;
    query.prepare("UPDATE MEETING SET TITLE = :title, ORGANISER = :organiser, PARTICIPANT = :participant, "
                  "AGENDA = :agenda, DURATION = :duration, DATEM = :datem, EMPLOYEE_ID = :employeeId, "
                  "CLIENT_ID = :clientId, RESSOURCE_ID = :resourceId WHERE ID = :id");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", datem);
    query.bindValue(":employeeId", employeeId >= 0 ? QVariant(employeeId) : QVariant(QMetaType(QMetaType::Int)));
    query.bindValue(":clientId", clientId >= 0 ? QVariant(clientId) : QVariant(QMetaType(QMetaType::Int)));
    query.bindValue(":resourceId", resourceId >= 0 ? QVariant(resourceId) : QVariant(QMetaType(QMetaType::Int)));
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to update meeting: " << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* meeting::searchByTitle(const QString& title)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM MEETING WHERE TITLE LIKE '%" + title + "%'");
    if (model->lastError().isValid()) {
        qDebug() << "Search meeting failed: " << model->lastError().text();
    }
    return model;
}
