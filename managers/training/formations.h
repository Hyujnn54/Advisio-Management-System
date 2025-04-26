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

    // Existing methods
    QSqlQueryModel *afficher();
    QSqlQueryModel *searchByType(const QString &type);
    QSqlQueryModel *searchByTrainer(const QString &trainer);
    QSqlQueryModel *getTrainingsForDate(const QDate &date);

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

    // New methods for TrainingManager
    QPair<bool, QString> ajouter(const QString &formation, const QString &description, const QString &trainer,
                                 const QDate &datef, int time, double prix);
    bool modifier(const QString &oldFormation, const QString &newFormation, const QString &newDescription,
                  const QString &newTrainer, const QDate &newDatef, int newTime, double newPrix);
    bool supprimer(const QString &formation);

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
