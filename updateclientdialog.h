#ifndef UPDATECLIENTDIALOG_H
#define UPDATECLIENTDIALOG_H

#include <QDialog>
#include <QDate>
#include "client.h"

namespace Ui {
class UpdateClientDialog;
}

class UpdateClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateClientDialog(QWidget *parent = nullptr);
    ~UpdateClientDialog();

    void setClientData(const QString &name, const QString &sector, const QString &contactInfo, const QDateTime &consultationDateTime, int consultant);
    QString getName() const;
    QString getSector() const;
    QString getContactInfo() const;
    QDateTime getConsultationDateTime() const;
    int getConsultant() const;

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::UpdateClientDialog *ui;
};

#endif // UPDATECLIENTDIALOG_H
