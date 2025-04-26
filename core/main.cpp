#include "../ui/mainwindow/mainwindow.h"
#include "connection.h"
#include "notificationmanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Check available drivers
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();

    // Create an instance of Connection and attempt to establish the database connection
    Connection conn;
    bool dbConnected = conn.createconnect();
    if (!dbConnected) {
        QMessageBox::warning(nullptr, "Database Warning",
                             "Failed to connect to the database. Some features may be unavailable.");
    }

    // Create a shared NotificationManager instance
    NotificationManager notificationManager;

    // Create MainWindow, passing the NotificationManager, and show it
    MainWindow w(dbConnected, &notificationManager);
    w.show();

    return a.exec();
}
