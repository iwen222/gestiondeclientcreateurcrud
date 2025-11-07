#include "creatordao.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

CreatorDAO::CreatorDAO(QObject *parent)
    : QObject(parent)
{
    initialiserTable();
}

bool CreatorDAO::initialiserTable()
{
    if (!verifierConnexion()) {
        return false;
    }
    
    QSqlQuery query;
    QString createTableSQL = QString(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "ID NUMBER PRIMARY KEY, "
        "NOM VARCHAR2(100) NOT NULL, "
        "PRENOM VARCHAR2(100) NOT NULL, "
        "EMAIL VARCHAR2(200) UNIQUE NOT NULL, "
        "TELEPHONE VARCHAR2(20), "
        "SPECIALITE VARCHAR2(100), "
        "ABONNES NUMBER DEFAULT 0, "
        "DATE_INSCRIPTION DATE DEFAULT SYSDATE"
        ")").arg(getTableName());
    
    if (!query.exec(createTableSQL)) {
        qDebug() << "Erreur création table:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Table CREATEUR initialisée avec succès";
    return true;
}

bool CreatorDAO::verifierConnexion()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit erreurOccurred("Base de données non connectée");
        return false;
    }
    return true;
}

bool CreatorDAO::ajouterCreator(const Creator& creator)
{
    if (!verifierConnexion()) {
        return false;
    }
    
    if (!creator.isValid()) {
        emit erreurOccurred("Données du créateur invalides");
        return false;
    }
    
    if (existeCreator(creator.getEmail())) {
        emit erreurOccurred("Un créateur avec cet email existe déjà");
        return false;
    }
    
    QSqlQuery query;
    query.prepare(QString(
        "INSERT INTO %1 (NOM, PRENOM, EMAIL, TELEPHONE, SPECIALITE, ABONNES, DATE_INSCRIPTION) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)").arg(getTableName()));
    
    query.addBindValue(creator.getNom());
    query.addBindValue(creator.getPrenom());
    query.addBindValue(creator.getEmail());
    query.addBindValue(creator.getTelephone());
    query.addBindValue(creator.getSpecialite());
    query.addBindValue(creator.getAbonnes());
    query.addBindValue(creator.getDateInscription());
    
    if (query.exec()) {
        emit creatorAjoute(creator);
        qDebug() << "Créateur ajouté avec succès";
        return true;
    } else {
        emit erreurOccurred("Erreur lors de l'ajout: " + query.lastError().text());
        return false;
    }
}

bool CreatorDAO::supprimerCreator(int id)
{
    if (!verifierConnexion()) {
        return false;
    }
    
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE ID = ?").arg(getTableName()));
    query.addBindValue(id);
    
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            emit creatorSupprime(id);
            qDebug() << "Créateur supprimé avec succès";
            return true;
        } else {
            emit erreurOccurred("Aucun créateur trouvé avec cet ID");
            return false;
        }
    } else {
        emit erreurOccurred("Erreur lors de la suppression: " + query.lastError().text());
        return false;
    }
}

bool CreatorDAO::modifierCreator(const Creator& creator)
{
    if (!verifierConnexion()) {
        return false;
    }
    
    if (!creator.isValid()) {
        emit erreurOccurred("Données du créateur invalides");
        return false;
    }
    
    QSqlQuery query;
    query.prepare(QString(
        "UPDATE %1 SET NOM = ?, PRENOM = ?, EMAIL = ?, TELEPHONE = ?, "
        "SPECIALITE = ?, ABONNES = ?, DATE_INSCRIPTION = ? WHERE ID = ?").arg(getTableName()));
    
    query.addBindValue(creator.getNom());
    query.addBindValue(creator.getPrenom());
    query.addBindValue(creator.getEmail());
    query.addBindValue(creator.getTelephone());
    query.addBindValue(creator.getSpecialite());
    query.addBindValue(creator.getAbonnes());
    query.addBindValue(creator.getDateInscription());
    query.addBindValue(creator.getId());
    
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            emit creatorModifie(creator);
            qDebug() << "Créateur modifié avec succès";
            return true;
        } else {
            emit erreurOccurred("Aucun créateur trouvé avec cet ID");
            return false;
        }
    } else {
        emit erreurOccurred("Erreur lors de la modification: " + query.lastError().text());
        return false;
    }
}

Creator CreatorDAO::getCreatorById(int id)
{
    Creator creator;
    
    if (!verifierConnexion()) {
        return creator;
    }
    
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE ID = ?").arg(getTableName()));
    query.addBindValue(id);
    
    if (query.exec() && query.next()) {
        creator.setId(query.value("ID").toInt());
        creator.setNom(query.value("NOM").toString());
        creator.setPrenom(query.value("PRENOM").toString());
        creator.setEmail(query.value("EMAIL").toString());
        creator.setTelephone(query.value("TELEPHONE").toString());
        creator.setSpecialite(query.value("SPECIALITE").toString());
        creator.setAbonnes(query.value("ABONNES").toInt());
        creator.setDateInscription(query.value("DATE_INSCRIPTION").toDate());
    }
    
    return creator;
}

QList<Creator> CreatorDAO::getAllCreators()
{
    QList<Creator> creators;
    
    if (!verifierConnexion()) {
        return creators;
    }
    
    QSqlQuery query(QString("SELECT * FROM %1 ORDER BY NOM, PRENOM").arg(getTableName()));
    
    while (query.next()) {
        Creator creator;
        creator.setId(query.value("ID").toInt());
        creator.setNom(query.value("NOM").toString());
        creator.setPrenom(query.value("PRENOM").toString());
        creator.setEmail(query.value("EMAIL").toString());
        creator.setTelephone(query.value("TELEPHONE").toString());
        creator.setSpecialite(query.value("SPECIALITE").toString());
        creator.setAbonnes(query.value("ABONNES").toInt());
        creator.setDateInscription(query.value("DATE_INSCRIPTION").toDate());
        
        creators.append(creator);
    }
    
    return creators;
}

QList<Creator> CreatorDAO::rechercherCreators(const QString& critere)
{
    QList<Creator> creators;
    
    if (!verifierConnexion()) {
        return creators;
    }
    
    QSqlQuery query;
    query.prepare(QString(
        "SELECT * FROM %1 WHERE UPPER(NOM) LIKE UPPER(?) OR UPPER(PRENOM) LIKE UPPER(?) "
        "OR UPPER(EMAIL) LIKE UPPER(?) OR UPPER(SPECIALITE) LIKE UPPER(?) "
        "ORDER BY NOM, PRENOM").arg(getTableName()));
    
    QString pattern = "%" + critere + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    
    while (query.next()) {
        Creator creator;
        creator.setId(query.value("ID").toInt());
        creator.setNom(query.value("NOM").toString());
        creator.setPrenom(query.value("PRENOM").toString());
        creator.setEmail(query.value("EMAIL").toString());
        creator.setTelephone(query.value("TELEPHONE").toString());
        creator.setSpecialite(query.value("SPECIALITE").toString());
        creator.setAbonnes(query.value("ABONNES").toInt());
        creator.setDateInscription(query.value("DATE_INSCRIPTION").toDate());
        
        creators.append(creator);
    }
    
    return creators;
}

QList<Creator> CreatorDAO::getCreatorsBySpecialite(const QString& specialite)
{
    QList<Creator> creators;
    
    if (!verifierConnexion()) {
        return creators;
    }
    
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE UPPER(SPECIALITE) = UPPER(?) ORDER BY NOM, PRENOM").arg(getTableName()));
    query.addBindValue(specialite);
    
    while (query.next()) {
        Creator creator;
        creator.setId(query.value("ID").toInt());
        creator.setNom(query.value("NOM").toString());
        creator.setPrenom(query.value("PRENOM").toString());
        creator.setEmail(query.value("EMAIL").toString());
        creator.setTelephone(query.value("TELEPHONE").toString());
        creator.setSpecialite(query.value("SPECIALITE").toString());
        creator.setAbonnes(query.value("ABONNES").toInt());
        creator.setDateInscription(query.value("DATE_INSCRIPTION").toDate());
        
        creators.append(creator);
    }
    
    return creators;
}

QList<Creator> CreatorDAO::getCreatorsByAbonnes(int minAbonnes, int maxAbonnes)
{
    QList<Creator> creators;
    
    if (!verifierConnexion()) {
        return creators;
    }
    
    QSqlQuery query;
    if (maxAbonnes == -1) {
        query.prepare(QString("SELECT * FROM %1 WHERE ABONNES >= ? ORDER BY ABONNES DESC").arg(getTableName()));
        query.addBindValue(minAbonnes);
    } else {
        query.prepare(QString("SELECT * FROM %1 WHERE ABONNES BETWEEN ? AND ? ORDER BY ABONNES DESC").arg(getTableName()));
        query.addBindValue(minAbonnes);
        query.addBindValue(maxAbonnes);
    }
    
    while (query.next()) {
        Creator creator;
        creator.setId(query.value("ID").toInt());
        creator.setNom(query.value("NOM").toString());
        creator.setPrenom(query.value("PRENOM").toString());
        creator.setEmail(query.value("EMAIL").toString());
        creator.setTelephone(query.value("TELEPHONE").toString());
        creator.setSpecialite(query.value("SPECIALITE").toString());
        creator.setAbonnes(query.value("ABONNES").toInt());
        creator.setDateInscription(query.value("DATE_INSCRIPTION").toDate());
        
        creators.append(creator);
    }
    
    return creators;
}

int CreatorDAO::getNombreTotalCreators()
{
    if (!verifierConnexion()) {
        return 0;
    }
    
    QSqlQuery query(QString("SELECT COUNT(*) FROM %1").arg(getTableName()));
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

bool CreatorDAO::existeCreator(const QString& email)
{
    if (!verifierConnexion()) {
        return false;
    }
    
    QSqlQuery query;
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE EMAIL = ?").arg(getTableName()));
    query.addBindValue(email);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

QMap<QString, int> CreatorDAO::getStatistiquesSpecialites()
{
    QMap<QString, int> stats;
    
    if (!verifierConnexion()) {
        return stats;
    }
    
    QSqlQuery query(QString("SELECT SPECIALITE, COUNT(*) FROM %1 GROUP BY SPECIALITE ORDER BY COUNT(*) DESC").arg(getTableName()));
    
    while (query.next()) {
        stats[query.value(0).toString()] = query.value(1).toInt();
    }
    
    return stats;
}

QMap<QString, int> CreatorDAO::getStatistiquesAbonnes()
{
    QMap<QString, int> stats;
    
    if (!verifierConnexion()) {
        return stats;
    }
    
    QSqlQuery query(QString(
        "SELECT "
        "CASE "
        "WHEN ABONNES < 1000 THEN 'Débutant (< 1K)' "
        "WHEN ABONNES < 10000 THEN 'Intermédiaire (1K-10K)' "
        "WHEN ABONNES < 100000 THEN 'Avancé (10K-100K)' "
        "ELSE 'Expert (100K+)' "
        "END as CATEGORIE, "
        "COUNT(*) "
        "FROM %1 GROUP BY "
        "CASE "
        "WHEN ABONNES < 1000 THEN 'Débutant (< 1K)' "
        "WHEN ABONNES < 10000 THEN 'Intermédiaire (1K-10K)' "
        "WHEN ABONNES < 100000 THEN 'Avancé (10K-100K)' "
        "ELSE 'Expert (100K+)' "
        "END "
        "ORDER BY COUNT(*) DESC").arg(getTableName()));
    
    while (query.next()) {
        stats[query.value(0).toString()] = query.value(1).toInt();
    }
    
    return stats;
}
