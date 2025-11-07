#ifndef PREDICTION_H
#define PREDICTION_H

#include <QString>
#include <QDateTime>
#include <QMap>
#include <QVector>

class Prediction
{
public:
    Prediction();
    
    // Structure pour stocker les résultats de prédiction
    struct ResultatPrediction {
        QString meilleurMoment;
        QString meilleurePlateforme;
        int score;
        QString justification;
    };
    
    // Prédire le meilleur moment pour publier
    ResultatPrediction predireMeilleurMoment(
        const QString& typeContenu,
        const QString& publicCible,
        const QDateTime& dateDebut = QDateTime::currentDateTime()
    );
    
    // Prédire la meilleure plateforme
    QString predireMeilleurePlateforme(
        const QString& typeContenu,
        const QString& publicCible,
        int dureeContenu = 0
    );
    
    // Analyser les tendances
    QMap<QString, int> analyserTendances(const QString& typeContenu);
    
private:
    // Données d'analyse pour les heures optimales
    QMap<QString, QVector<int>> heuresOptimales;
    
    // Données d'analyse pour les plateformes
    QMap<QString, QMap<QString, int>> scoresPlateforme;
    
    void initialiserDonnees();
    QString obtenirJourSemaine(const QDateTime& date);
    int calculerScore(const QString& plateforme, const QString& typeContenu);
};

#endif // PREDICTION_H
