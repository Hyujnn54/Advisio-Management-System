#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QList>

class QWidget;

class NotificationManager : public QObject {
    Q_OBJECT

public:
    struct Notification {
        QString action;
        QDateTime timestamp;
        QString location;
        QString details;
        int lineNumber;
    };

    explicit NotificationManager(QObject *parent = nullptr);

    void addNotification(const QString &action, const QString &location, const QString &details, int lineNumber);
    int getNotificationCount() const;
    void showNotifications(QWidget *parent);

signals:
    void notificationCountChanged(int count);

private:
    QList<Notification> notifications;
};

#endif // NOTIFICATIONMANAGER_H
