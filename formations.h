#ifndef FORMATIONS_H
#define FORMATIONS_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class formations
{
public:
    formations();
    formations(int idfor, QString &formation, QString &description, QString &trainer, QDate &datef, int &time, double &prix);
    QSqlQueryModel *afficher();
    QSqlQueryModel *searchByType(const QString &type); // Added for "By Formation" filter
    QSqlQueryModel *searchByTrainer(const QString &trainer); // Added for "By Trainer" filter
    QSqlQueryModel *getTrainingsForDate(const QDate &date); // Added for "By Date" filter

    int getIdfor() const;
    QString getFormation() const;
    QString getDescription() const;
    QString getTrainer() const;
    QDate getDatef() const;
    int getTime() const;
    double getPrix() const;

    void setIdfor(int idfor);
    void setFormation(const QString &formation);
    void setDescription(const QString &description);
    void setTrainer(const QString &trainer);
    void setDatef(const QDate &datef);
    void setTime(int time);
    void setPrix(double prix);

    bool ajoutforma();
    static bool deleteFormation(int id);
    static bool updateFormation(int idfor, const QString &newFormation, const QString &newDescription,
                                const QString &newTrainer, const QDate &newDatef, int newTime, double newPrix);
    static bool exists(int idfor);

private:
    int idfor;
    QString formation;
    QString description;
    QString trainer;
    QDate datef;
    int time;
    double prix;
};

#endif // FORMATIONS_H
