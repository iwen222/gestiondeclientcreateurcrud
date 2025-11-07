#include "predictionwidget.h"
#include "ui_prediction.h"
#include <QMessageBox>
#include <QDateTime>

PredictionWidget::PredictionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PredictionWidget),
    prediction(new Prediction())
{
    ui->setupUi(this);
    
    // Initialiser la date/heure actuelle
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    
    // Connecter le bouton prédire
    connect(ui->btnPredire, &QPushButton::clicked, this, &PredictionWidget::predire);
    
    // Initialiser les champs de résultats
    ui->textMoment->setPlainText("Cliquez sur le bouton pour obtenir une prédiction...");
    ui->textPlateforme->setPlainText("");
    ui->textJustification->setPlainText("");
}

PredictionWidget::~PredictionWidget()
{
    delete ui;
    delete prediction;
}

void PredictionWidget::predire()
{
    // Récupérer les données du formulaire
    QString typeContenu = ui->comboTypeContenu->currentText();
    QString publicCible = ui->comboPublicCible->currentText();
    QDateTime dateDebut = ui->dateTimeEdit->dateTime();
    int duree = ui->spinDuree->value();
    
    // Effectuer la prédiction
    Prediction::ResultatPrediction resultat = prediction->predireMeilleurMoment(
        typeContenu,
        publicCible,
        dateDebut
    );
    
    // Ajuster la plateforme selon la durée si c'est une vidéo
    if (typeContenu.contains("Video", Qt::CaseInsensitive)) {
        QString meilleurePlateforme = prediction->predireMeilleurePlateforme(
            typeContenu,
            publicCible,
            duree
        );
        resultat.meilleurePlateforme = meilleurePlateforme;
    }
    
    // Afficher les résultats
    ui->textMoment->setPlainText(resultat.meilleurMoment);
    ui->textPlateforme->setPlainText(resultat.meilleurePlateforme);
    ui->textJustification->setPlainText(resultat.justification);
    
    // Afficher un message de succès
    QMessageBox::information(this, "Prédiction réussie",
                            QString("Score de confiance: %1/100\n\n"
                                   "Nous vous recommandons de publier votre %2 "
                                   "sur %3 %4")
                            .arg(resultat.score)
                            .arg(typeContenu)
                            .arg(resultat.meilleurePlateforme)
                            .arg(resultat.meilleurMoment));
}
