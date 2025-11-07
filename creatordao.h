#ifndef CREATORDAO_H
#define CREATORDAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include "creator.h"

class CreatorDAO : public QObject
{
    Q_OBJECT

public:
    explicit CreatorDAO(QObject *parent = nullptr);
    
    // Opérations CRUD
    bool ajouterCreator(const Creator& creator);
    bool supprimerCreator(int id);
    bool modifierCreator(const Creator& creator);
    Creator getCreatorById(int id);
    QList<Creator> getAllCreators();
    QList<Creator> rechercherCreators(const QString& critere);
    
    // Opérations spécialisées
    QList<Creator> getCreatorsBySpecialite(const QString& specialite);
    QList<Creator> getCreatorsByAbonnes(int minAbonnes, int maxAbonnes = -1);
    int getNombreTotalCreators();
    bool existeCreator(const QString& email);
    
    // Statistiques
    QMap<QString, int> getStatistiquesSpecialites();
    QMap<QString, int> getStatistiquesAbonnes();
    
signals:
    void creatorAjoute(const Creator& creator);
    void creatorSupprime(int id);
    void creatorModifie(const Creator& creator);
    void erreurOccurred(const QString& message);

private:
    bool initialiserTable();
    bool verifierConnexion();
    QString getTableName() const { return "CREATEUR"; }
};

#endif // CREATORDAO_H
