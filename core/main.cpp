#include "mainwindow.h"
#include "connection.h"
#include "../sign.h" // Inclure l'en-tête de la fenêtre de connexion
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    // Désactiver les messages de débogage liés aux polices
    QLoggingCategory::setFilterRules("qt.text.font.warning=false");
    QLoggingCategory::setFilterRules("qt.gui.fonts.warning=false");
    QLoggingCategory::setFilterRules("qt.directwrite.warning=false");
    QFont::insertSubstitution("Fixedsys", "Segoe UI");
    QFont::insertSubstitution("8514oem", "Segoe UI");
    QFont::insertSubstitution("MS Serif", "Segoe UI");
    QFont::insertSubstitution("Modern", "Segoe UI");

    QApplication a(argc, argv);

    // Check available drivers
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();

    // Create an instance of Connection and attempt to establish the database connection
    Connection conn;
    bool dbConnected = conn.createconnect();
    if (!dbConnected) {
        QMessageBox::warning(nullptr, "Database Warning",
                             "Failed to connect to the database. Some features may be unavailable.");
        // Continue running the application instead of exiting
    }

    // Créer et afficher l'écran de connexion au lieu de la fenêtre principale
    sign loginWindow;
    loginWindow.show();
    
    // Exécuter l'application - la fenêtre principale sera créée après une connexion réussie
    return a.exec();
}
