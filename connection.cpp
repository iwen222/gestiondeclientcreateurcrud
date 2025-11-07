#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

bool createConnection() {
    qDebug() << "🔍 Available drivers:" << QSqlDatabase::drivers();

    if (!QSqlDatabase::drivers().contains("QODBC")) {
        QMessageBox::critical(nullptr, "Erreur - Pilote manquant",
                              "Le pilote ODBC (qsqlodbc.dll) n'est pas trouvé.\n"
                              "➡️ Vérifie que le dossier 'sqldrivers' se trouve à côté de ton exécutable\n"
                              "et contient le fichier qsqlodbc.dll");
        return false;
    }

    // 1️⃣ Try DSN method first (e.g. “Source_Projet2a”)
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2a");
    db.setUserName("iwen");
    db.setPassword("esprit18");

    if (db.open()) {
        qDebug() << "✅ Connexion Oracle réussie via DSN (Source_Projet2a)";
        return true;
    }

    qDebug() << "❌ Connexion via DSN échouée :" << db.lastError().text();

    // 2️⃣ Fallback: DSN-less direct connection string
    db = QSqlDatabase::addDatabase("QODBC", "fallback");
    db.setDatabaseName("Driver={Oracle in OraClient11g_home1};Dbq=XE;Uid=abir;Pwd=esprit18;");

    if (db.open()) {
        qDebug() << "✅ Connexion Oracle réussie via chaîne directe (Driver={Oracle in OraClient11g_home1};Dbq=XE)";
        return true;
    }

    // 3️⃣ If both failed, show full error
    QString pluginPath = QDir(QCoreApplication::applicationDirPath()).filePath("sqldrivers/qsqlodbc.dll");
    QString msg = QString("Échec de la connexion à Oracle.\n\n"
                          "• Erreur DSN : %1\n"
                          "• Erreur directe : %2\n\n"
                          "Vérifie que :\n"
                          " - Le client Oracle est installé et son dossier est dans le PATH.\n"
                          " - Le DSN 'Source_Projet2a' existe (ODBC 64-bit).\n"
                          " - Le plugin '%3' est présent.")
                      .arg(db.lastError().text(), db.lastError().text(), pluginPath);

    QMessageBox::critical(nullptr, "Erreur Oracle ODBC", msg);
    qDebug() << "⚠️" << msg;
    return false;
}
