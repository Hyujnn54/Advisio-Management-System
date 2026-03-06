// core/notificationmanager.h
#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QVector>

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    struct Notification {
        QString title;
        QString description;
        QString details;
        int row;
    };

    explicit NotificationManager(QObject *parent = nullptr);

    void addNotification(const QString &title, const QString &description, const QString &details, int row);
    void clearNotifications();
    int getNotificationCount() const;
    const QVector<Notification>& getNotifications() const;

signals:
    void notificationCountChanged(int count);

private:
    QVector<Notification> notifications;
    int notificationCount;
};

#endif // NOTIFICATIONMANAGER_H
