#include "createur.h"
#include <QSqlQuery>
#include <QVariant>

Createur::Createur() {}

Createur::Createur(int id, QString n, QString p, QString ps, QString t, QString r)
    : id_createur(id), nom(n), prenom(p), pseudo(ps), telephone(t), reseau(r) {}

int Createur::getId() const { return id_createur; }
QString Createur::getNom() const { return nom; }
QString Createur::getPrenom() const { return prenom; }
QString Createur::getPseudo() const { return pseudo; }
QString Createur::getTelephone() const { return telephone; }
QString Createur::getReseau() const { return reseau; }

bool Createur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CREATEUR (ID_CREATEUR, NOM, PRENOM, PSEUDO, TELEPHONE, RESEAU) "
                  "VALUES (:id, :nom, :prenom, :pseudo, :telephone, :reseau)");
    query.bindValue(":id", id_createur);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":pseudo", pseudo);
    query.bindValue(":telephone", telephone);
    query.bindValue(":reseau", reseau);
    return query.exec();
}

QSqlQueryModel* Createur::afficher()
{
    auto *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CREATEUR ORDER BY ID_CREATEUR ASC");
    return model;
}

bool Createur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CREATEUR WHERE ID_CREATEUR = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Createur::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE CREATEUR SET NOM=:nom, PRENOM=:prenom, PSEUDO=:pseudo, TELEPHONE=:telephone, RESEAU=:reseau "
                  "WHERE ID_CREATEUR=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":pseudo", pseudo);
    query.bindValue(":telephone", telephone);
    query.bindValue(":reseau", reseau);
    return query.exec();
}

QSqlQueryModel* Createur::rechercher(const QString &critere)
{
    auto *model = new QSqlQueryModel();
    QString query = QString("SELECT * FROM CREATEUR WHERE "
                            "NOM LIKE '%%1%' OR PRENOM LIKE '%%1%' OR PSEUDO LIKE '%%1%'")
                        .arg(critere);
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Createur::trier(const QString &colonne, const QString &ordre)
{
    auto *model = new QSqlQueryModel();
    QString query = QString("SELECT * FROM CREATEUR ORDER BY %1 %2").arg(colonne, ordre);
    model->setQuery(query);
    return model;
}
