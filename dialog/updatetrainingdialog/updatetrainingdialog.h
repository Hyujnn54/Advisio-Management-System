#ifndef UPDATETRAININGDIALOG_H
#define UPDATETRAININGDIALOG_H

#include <QDialog>
#include <QDate>
#include "ui_updatetrainingdialog.h"

class UpdateTrainingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTrainingDialog(QWidget *parent = nullptr);
    ~UpdateTrainingDialog();
    void setTrainingData(const QString &name, const QString &description, const QString &trainer,
                         const QDate &date, int time, double prix);
    QString getName() const;
    QString getDescription() const;
    QString getTrainer() const;
    QDate getDate() const;
    int getTime() const;
    double getPrix() const;
    void setResourceData(int trainingId);
    QList<QPair<int, int>> getSelectedResources() const;
    void accept() override;
    void setTrainingId(int id) { trainingId = id; }

private:
    Ui::UpdateTrainingDialog *ui;
    int trainingId;
};

#endif // UPDATETRAININGDIALOG_H
