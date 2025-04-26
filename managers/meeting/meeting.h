// managers/meeting/meeting.h
#ifndef MEETING_H
#define MEETING_H

#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QSqlQueryModel>
#include <QSqlError>

class meeting {
private:
    int id;
    QString title;
    QString organiser;
    QString participant;
    QString agenda;
    int duration;
    QDateTime datem;
    int employeeId;
    int clientId;
    int resourceId;

public:
    meeting();  // Added default constructor
    meeting(QString title, QString organiser, QString participant, QString agenda, int duration, QDateTime datem,
            int employeeId = -1, int clientId = -1, int resourceId = -1);

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getOrganiser() const;
    QString getParticipant() const;
    QString getAgenda() const;
    int getDuration() const;
    QDateTime getDatem() const;
    int getEmployeeId() const;
    int getClientId() const;
    int getResourceId() const;

    // Setters
    void setId(const int &id);
    void setTitle(const QString &title);
    void setOrganiser(const QString &organiser);
    void setParticipant(const QString &participant);
    void setAgenda(const QString &agenda);
    void setDuration(int duration);
    void setDatem(const QDateTime &datem);
    void setEmployeeId(int employeeId);
    void setClientId(int clientId);
    void setResourceId(int resourceId);

    // Functionalities
    bool add();
    QPixmap generateQRCode() const;
    QSqlQueryModel* afficher();
    bool deleteMeeting(int id);
    bool update();
    QSqlQueryModel* searchByTitle(const QString& title);
};

#endif // MEETING_H
