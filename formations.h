#ifndef FORMATIONS_H
#define FORMATIONS_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class formations
{
public:
    formations();
    formations(QString& formation,QString& description,QString& trainer,QDate& datef,int& time,int& prix);
    QSqlQueryModel * afficher();
    int getIdfor() const;
    QString getFormation() const;
    QString getDescription() const;
    QString getTrainer() const;
    QDate getDatef() const;
    int getTime() const;
    int getPrix() const;


    void setFormation(const QString& formation);
    void setDescriptione(const QString& description);
    void setTrainer(const QString& trainer);
    void setDatef(const QDate& datef);
    void setTime(int time);
    void setPrix(int prix);

    bool ajoutforma();
    static bool deleteFormation(int id);
    static bool updateFormation(int idfor, const QString& newFormation, const QString& newDescription,
                                const QString& newTrainer, const QDate& newDatef, int newTime, int newPrix);
    static bool exists(int idfor);
private:
    int idfor;
    QString formation;
    QString description;
    QString trainer;
    QDate datef;
    int time;
    int prix;

};



#endif // FORMATIONS_H
