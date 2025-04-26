#include "notificationmanager.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent)
{
}

void NotificationManager::addNotification(const QString &action, const QString &location, const QString &details, int lineNumber)
{
    Notification notification;
    notification.action = action;
    notification.timestamp = QDateTime::currentDateTime();
    notification.location = location;
    notification.details = details;
    notification.lineNumber = lineNumber;
    notifications.append(notification);

    emit notificationCountChanged(notifications.size());
}

int NotificationManager::getNotificationCount() const
{
    return notifications.size();
}

void NotificationManager::showNotifications(QWidget *parent)
{
    if (notifications.isEmpty()) {
        QMessageBox::information(parent, "Notifications", "No modifications recorded.");
        return;
    }

    QDialog *notificationDialog = new QDialog(parent);
    notificationDialog->setWindowTitle("Modification History");
    notificationDialog->setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(notificationDialog);
    QTextEdit *textEdit = new QTextEdit(notificationDialog);
    textEdit->setReadOnly(true);
    QPushButton *closeButton = new QPushButton("Close", notificationDialog);

    layout->addWidget(textEdit);
    layout->addWidget(closeButton);

    QString notificationText;
    for (const Notification &notification : notifications) {
        notificationText += QString("[%1] %2\nLocation: %3\nLine: %4\nDetails: %5\n\n")
        .arg(notification.timestamp.toString("yyyy-MM-dd HH:mm:ss"),
             notification.action,
             notification.location,
             QString::number(notification.lineNumber),
             notification.details);
    }
    textEdit->setText(notificationText);

    connect(closeButton, &QPushButton::clicked, notificationDialog, &QDialog::accept);

    notificationDialog->exec();
    delete notificationDialog;
}
