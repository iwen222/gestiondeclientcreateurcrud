// Fichier de test pour la fonctionnalité de prédiction
// Ce fichier peut être utilisé pour tester la logique indépendamment

#include "prediction.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Prediction pred;
    
    qDebug() << "=== TEST DE PREDICTION ===\n";
    
    // Test 1: Vidéo courte pour jeunes
    qDebug() << "Test 1: Vidéo courte pour jeunes";
    Prediction::ResultatPrediction r1 = pred.predireMeilleurMoment(
        "Video courte",
        "Jeunes (18-25 ans)",
        QDateTime::currentDateTime()
    );
    qDebug() << "Moment:" << r1.meilleurMoment;
    qDebug() << "Plateforme:" << r1.meilleurePlateforme;
    qDebug() << "Score:" << r1.score;
    qDebug() << "Justification:" << r1.justification;
    qDebug() << "\n";
    
    // Test 2: Article pour professionnels
    qDebug() << "Test 2: Article pour professionnels";
    Prediction::ResultatPrediction r2 = pred.predireMeilleurMoment(
        "Article",
        "Professionnels",
        QDateTime::currentDateTime()
    );
    qDebug() << "Moment:" << r2.meilleurMoment;
    qDebug() << "Plateforme:" << r2.meilleurePlateforme;
    qDebug() << "Score:" << r2.score;
    qDebug() << "Justification:" << r2.justification;
    qDebug() << "\n";
    
    // Test 3: Podcast pour grand public
    qDebug() << "Test 3: Podcast pour grand public";
    Prediction::ResultatPrediction r3 = pred.predireMeilleurMoment(
        "Podcast",
        "Grand public",
        QDateTime::currentDateTime()
    );
    qDebug() << "Moment:" << r3.meilleurMoment;
    qDebug() << "Plateforme:" << r3.meilleurePlateforme;
    qDebug() << "Score:" << r3.score;
    qDebug() << "Justification:" << r3.justification;
    qDebug() << "\n";
    
    qDebug() << "=== FIN DES TESTS ===";
    
    return 0;
}
