#include "prediction.h"
#include <QDebug>

Prediction::Prediction()
{
    initialiserDonnees();
}

void Prediction::initialiserDonnees()
{
    // Heures optimales par jour de la semaine (basé sur des études d'engagement)
    // Lundi
    heuresOptimales["Lundi"] = {8, 9, 12, 13, 17, 18, 19};
    // Mardi
    heuresOptimales["Mardi"] = {8, 9, 11, 12, 13, 17, 18};
    // Mercredi
    heuresOptimales["Mercredi"] = {7, 8, 9, 11, 13, 17, 18, 19};
    // Jeudi
    heuresOptimales["Jeudi"] = {8, 9, 12, 13, 17, 18, 19, 20};
    // Vendredi
    heuresOptimales["Vendredi"] = {7, 8, 9, 13, 17, 18, 19};
    // Samedi
    heuresOptimales["Samedi"] = {9, 10, 11, 12, 14, 15, 16, 17};
    // Dimanche
    heuresOptimales["Dimanche"] = {9, 10, 11, 14, 15, 16, 17, 18, 19};
    
    // Scores de plateformes par type de contenu
    scoresPlateforme["Video courte"]["TikTok"] = 95;
    scoresPlateforme["Video courte"]["Instagram Reels"] = 90;
    scoresPlateforme["Video courte"]["YouTube Shorts"] = 85;
    scoresPlateforme["Video courte"]["Facebook"] = 70;
    
    scoresPlateforme["Video longue"]["YouTube"] = 95;
    scoresPlateforme["Video longue"]["Facebook"] = 75;
    scoresPlateforme["Video longue"]["LinkedIn"] = 60;
    
    scoresPlateforme["Image"]["Instagram"] = 95;
    scoresPlateforme["Image"]["Pinterest"] = 90;
    scoresPlateforme["Image"]["Facebook"] = 80;
    scoresPlateforme["Image"]["Twitter"] = 70;
    
    scoresPlateforme["Texte"]["Twitter"] = 95;
    scoresPlateforme["Texte"]["LinkedIn"] = 90;
    scoresPlateforme["Texte"]["Facebook"] = 75;
    
    scoresPlateforme["Article"]["LinkedIn"] = 95;
    scoresPlateforme["Article"]["Medium"] = 90;
    scoresPlateforme["Article"]["Facebook"] = 70;
    
    scoresPlateforme["Podcast"]["Spotify"] = 95;
    scoresPlateforme["Podcast"]["Apple Podcasts"] = 90;
    scoresPlateforme["Podcast"]["YouTube"] = 85;
}

QString Prediction::obtenirJourSemaine(const QDateTime& date)
{
    int jour = date.date().dayOfWeek();
    switch(jour) {
        case 1: return "Lundi";
        case 2: return "Mardi";
        case 3: return "Mercredi";
        case 4: return "Jeudi";
        case 5: return "Vendredi";
        case 6: return "Samedi";
        case 7: return "Dimanche";
        default: return "Lundi";
    }
}


QString Prediction::predireMeilleurePlateforme(
    const QString& typeContenu,
    const QString& publicCible,
    int dureeContenu)
{
    QString meilleurePlateforme = "Instagram";
    int meilleurScore = 0;
    
    // Vérifier si nous avons des données pour ce type de contenu
    if (scoresPlateforme.contains(typeContenu)) {
        QMap<QString, int> scores = scoresPlateforme[typeContenu];
        
        for (auto it = scores.begin(); it != scores.end(); ++it) {
            int score = it.value();
            
            // Ajuster le score selon le public cible
            if (publicCible.contains("Professionnel", Qt::CaseInsensitive) &&
                it.key().contains("LinkedIn")) {
                score += 10;
            } else if (publicCible.contains("Jeune", Qt::CaseInsensitive) &&
                       (it.key().contains("TikTok") || it.key().contains("Instagram"))) {
                score += 10;
            }
            
            // Ajuster selon la durée pour les vidéos
            if (typeContenu.contains("Video", Qt::CaseInsensitive)) {
                if (dureeContenu > 0 && dureeContenu < 60 && it.key().contains("TikTok")) {
                    score += 5;
                } else if (dureeContenu > 300 && it.key().contains("YouTube")) {
                    score += 5;
                }
            }
            
            if (score > meilleurScore) {
                meilleurScore = score;
                meilleurePlateforme = it.key();
            }
        }
    }
    
    return meilleurePlateforme;
}

Prediction::ResultatPrediction Prediction::predireMeilleurMoment(
    const QString& typeContenu,
    const QString& publicCible,
    const QDateTime& dateDebut)
{
    ResultatPrediction resultat;
    
    // Prédire le meilleur moment
    QString jour = obtenirJourSemaine(dateDebut);
    QVector<int> heures = heuresOptimales.value(jour, {9, 12, 18});
    
    int heureOptimale = 12;
    if (publicCible.contains("Jeune", Qt::CaseInsensitive)) {
        heureOptimale = heures.size() > 0 ? heures.last() : 19;
    } else if (publicCible.contains("Professionnel", Qt::CaseInsensitive)) {
        heureOptimale = heures.size() > 1 ? heures[1] : 9;
    } else {
        heureOptimale = heures.size() > heures.size()/2 ? heures[heures.size()/2] : 13;
    }
    
    QDateTime momentOptimal = dateDebut;
    momentOptimal.setTime(QTime(heureOptimale, 0));
    
    // Prédire la meilleure plateforme
    resultat.meilleurePlateforme = predireMeilleurePlateforme(typeContenu, publicCible, 0);
    resultat.meilleurMoment = momentOptimal.toString("dddd dd MMMM yyyy à HH:mm");
    resultat.score = 85 + (heures.contains(heureOptimale) ? 10 : 0);
    resultat.justification = QString("Pour un contenu '%1' destiné au public '%2', "
                                    "nous recommandons de publier sur %3 le %4 à %5h00. "
                                    "Ce créneau maximise la visibilité et l'engagement.")
                                    .arg(typeContenu)
                                    .arg(publicCible)
                                    .arg(resultat.meilleurePlateforme)
                                    .arg(jour)
                                    .arg(heureOptimale);
    
    return resultat;
}

QMap<QString, int> Prediction::analyserTendances(const QString& typeContenu)
{
    QMap<QString, int> tendances;
    
    if (scoresPlateforme.contains(typeContenu)) {
        tendances = scoresPlateforme[typeContenu];
    }
    
    return tendances;
}

int Prediction::calculerScore(const QString& plateforme, const QString& typeContenu)
{
    if (scoresPlateforme.contains(typeContenu)) {
        return scoresPlateforme[typeContenu].value(plateforme, 50);
    }
    return 50;
}
