#include "gestioncreateur.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QDebug>

GestionCreateur::GestionCreateur(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent), ui(ui)
{
    // Connexions (assure-toi que ces noms existent EXACTEMENT dans mainwindow.ui)
    connect(ui->toolbuttajouter, &QToolButton::clicked, this, &GestionCreateur::ajouter);
    connect(ui->toolbutsupp,   &QToolButton::clicked, this, &GestionCreateur::supprimer);
    connect(ui->toolbutttrie,   &QToolButton::clicked, this, &GestionCreateur::modifier);
    connect(ui->toolbutttrie,    &QToolButton::clicked, this, &GestionCreateur::rechercher);
    connect(ui->toolbutttrie,    &QToolButton::clicked, this, &GestionCreateur::trier);
    // optionnel : bouton "afficher" si tu l'as
    // Initialise l'affichage
    afficher();
}

GestionCreateur::~GestionCreateur()
{
    // Rien à libérer ici (ui appartient au MainWindow)
}

void GestionCreateur::remplirTableAvecQuery(QSqlQuery &query)
{
    ui->tab_9->setRowCount(0);
    // colonnes : ID_CREATEUR, NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU
    ui->tab_9->setColumnCount(6);
    ui->tab_9->setHorizontalHeaderLabels(QStringList() << "ID" << "Nom" << "Prenom" << "Pseudo" << "Telephone" << "Reseau");

    int row = 0;
    while (query.next()) {
        ui->tab_9->insertRow(row);
        ui->tab_9->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tab_9->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tab_9->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tab_9->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tab_9->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tab_9->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ++row;
    }
}

void GestionCreateur::afficher()
{
    QSqlQuery query;
    // sélectionne explicitement les colonnes pour être sûr de l'ordre
    if (!query.exec("SELECT ID_CREATEUR, NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU FROM CREATEUR ORDER BY ID_CREATEUR ASC")) {
        qWarning() << "Erreur SELECT afficher:" << query.lastError().text();
        return;
    }
    remplirTableAvecQuery(query);
}

void GestionCreateur::ajouter()
{
    QString id = ui->lineid->text().trimmed();
    QString nom = ui->linenom->text().trimmed();
    QString prenom = ui->lineprenom->text().trimmed();
    QString pseudo = ui->linepseudo->text().trimmed();
    QString telephone = ui->linetelephone->text().trimmed();
    QString reseau = ui->lineplat->text().trimmed();

    if (nom.isEmpty()) {
        QMessageBox::warning(nullptr, "Champs manquants", "Le nom est requis.");
        return;
    }

    QSqlQuery query;
    // Si ID fourni, on l'insère explicitement, sinon on laisse la DB gérer (si sequence/trigger présent)
    if (!id.isEmpty()) {
        query.prepare("INSERT INTO CREATEUR (ID_CREATEUR, NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU) "
                      "VALUES (:id, :nom, :prenom, :pseudo, :tel, :reseau)");
        query.bindValue(":id", id);
    } else {
        query.prepare("INSERT INTO CREATEUR (NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU) "
                      "VALUES (:nom, :prenom, :pseudo, :tel, :reseau)");
    }
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":pseudo", pseudo);
    query.bindValue(":tel", telephone);
    query.bindValue(":reseau", reseau);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL (ajout)", query.lastError().text());
        return;
    }

    QMessageBox::information(nullptr, "Succès", "Créateur ajouté.");
    // rafraîchir la table et vider les champs si tu veux
    afficher();
    ui->lineid->clear();
    ui->linenom->clear();
    ui->lineprenom->clear();
    ui->linepseudo->clear();
    ui->linetelephone->clear();
    ui->lineplat->clear();
}

void GestionCreateur::supprimer()
{
    QString id = ui->lineid->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(nullptr, "Supprimer", "Renseigne l'ID du créateur à supprimer.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CREATEUR WHERE ID_CREATEUR = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL (suppression)", query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::information(nullptr, "Suppression", "Aucun enregistrement supprimé (ID inconnu).");
    } else {
        QMessageBox::information(nullptr, "Suppression", "Créateur supprimé.");
    }
    afficher();
}

void GestionCreateur::modifier()
{
    QString id = ui->lineid->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(nullptr, "Modifier", "Renseigne l'ID du créateur à modifier.");
        return;
    }

    QString nom = ui->linenom->text().trimmed();
    QString prenom = ui->lineprenom->text().trimmed();
    QString pseudo = ui->linepseudo->text().trimmed();
    QString telephone = ui->linetelephone->text().trimmed();
    QString reseau = ui->lineplat->text().trimmed();

    QSqlQuery query;
    query.prepare("UPDATE CREATEUR SET NOM=:nom, PRENOM=:prenom, PSEUDO=:pseudo, TELEPHONE=:tel, RESEAU=:reseau "
                  "WHERE ID_CREATEUR = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":pseudo", pseudo);
    query.bindValue(":tel", telephone);
    query.bindValue(":reseau", reseau);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL (modification)", query.lastError().text());
        return;
    }

    QMessageBox::information(nullptr, "Modification", "Créateur modifié.");
    afficher();
}

void GestionCreateur::rechercher()
{
    QString key = ui->linerech->text().trimmed();
    if (key.isEmpty()) {
        afficher();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_CREATEUR, NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU FROM CREATEUR "
                  "WHERE LOWER(NOM) LIKE LOWER(:kw) OR LOWER(PRENOM) LIKE LOWER(:kw) OR LOWER(PSEUDO) LIKE LOWER(:kw)");
    query.bindValue(":kw", "%" + key + "%");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL (recherche)", query.lastError().text());
        return;
    }

    remplirTableAvecQuery(query);
}

void GestionCreateur::trier()
{
    // lit la valeur du combo box linetrie (ex: "id", "nom", "prenom", "email", "telephone")
    QString crit = ui->linetrie->currentText().trimmed().toUpper();
    QString col;
    if (crit == "ID" || crit == "ID_CREATEUR") col = "ID_CREATEUR";
    else if (crit == "NOM") col = "NOM";
    else if (crit == "PRENOM") col = "PRENOM";
    else if (crit == "PSEUDO") col = "PSEUDO";
    else if (crit == "TELEPHONE") col = "TELEPHONE";
    else if (crit == "RESEAU") col = "RESEAU";
    else col = "ID_CREATEUR";

    QSqlQuery query;
    QString q = QString("SELECT ID_CREATEUR, NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU FROM CREATEUR ORDER BY %1 ASC").arg(col);
    if (!query.exec(q)) {
        QMessageBox::critical(nullptr, "Erreur SQL (trier)", query.lastError().text());
        return;
    }
    remplirTableAvecQuery(query);
}
