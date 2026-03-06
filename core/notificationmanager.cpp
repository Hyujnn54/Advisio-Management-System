// core/notificationmanager.cpp
#include "notificationmanager.h"

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent), notificationCount(0)
{
}

void NotificationManager::addNotification(const QString &title, const QString &description, const QString &details, int row)
{
    notifications.append({title, description, details, row});
    notificationCount++;
    emit notificationCountChanged(notificationCount);
}

void NotificationManager::clearNotifications()
{
    notifications.clear();
    notificationCount = 0;
    emit notificationCountChanged(notificationCount);
}

int NotificationManager::getNotificationCount() const
{
    return notificationCount;
}

const QVector<NotificationManager::Notification>& NotificationManager::getNotifications() const
{
    return notifications;
}
