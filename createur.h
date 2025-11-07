#ifndef CREATEUR_H
#define CREATEUR_H

#include <QString>
#include <QSqlQueryModel>

class Createur
{
public:
    Createur();
    Createur(int, QString, QString, QString, QString, QString);

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getPseudo() const;
    QString getTelephone() const;
    QString getReseau() const;

    // CRUD operations
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int);
    QSqlQueryModel* rechercher(const QString &critere);
    QSqlQueryModel* trier(const QString &colonne, const QString &ordre);

private:
    int id_createur;
    QString nom, prenom, pseudo, telephone, reseau;
};

#endif // CREATEUR_H
