#ifndef CREATORWIDGET_H
#define CREATORWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QFormLayout>
#include <QGridLayout>
#include <QTabWidget>
#include "creator.h"
#include "creatordao.h"

class CreatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreatorWidget(QWidget *parent = nullptr);
    ~CreatorWidget();

private slots:
    void ajouterCreator();
    void modifierCreator();
    void supprimerCreator();
    void afficherTousCreators();
    void rechercherCreators();
    void selectionnerCreator();
    void viderFormulaire();
    void exporterPDF();
    void actualiserListe();
    
    // Slots pour les signaux du DAO
    void onCreatorAjoute(const Creator& creator);
    void onCreatorSupprime(int id);
    void onCreatorModifie(const Creator& creator);
    void onErreurOccurred(const QString& message);

private:
    void setupUI();
    void setupFormulaire();
    void setupTableau();
    void setupBoutons();
    void connecterSignaux();
    void chargerDonnees();
    void afficherCreatorDansFormulaire(const Creator& creator);
    Creator getCreatorDepuisFormulaire();
    bool validerFormulaire();
    void afficherMessage(const QString& titre, const QString& message, bool erreur = false);
    void mettreAJourStatistiques();
    
    // Composants UI
    QTabWidget *tabWidget;
    
    // Onglet Gestion
    QWidget *ongletGestion;
    QGroupBox *groupBoxFormulaire;
    QGroupBox *groupBoxActions;
    QGroupBox *groupBoxRecherche;
    
    // Formulaire
    QLineEdit *lineEditNom;
    QLineEdit *lineEditPrenom;
    QLineEdit *lineEditEmail;
    QLineEdit *lineEditTelephone;
    QLineEdit *lineEditSpecialite;
    QSpinBox *spinBoxAbonnes;
    QDateEdit *dateEditInscription;
    
    // Boutons
    QPushButton *btnAjouter;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QPushButton *btnVider;
    QPushButton *btnActualiser;
    QPushButton *btnExporterPDF;
    
    // Recherche
    QLineEdit *lineEditRecherche;
    QPushButton *btnRechercher;
    QComboBox *comboFiltreSpecialite;
    QComboBox *comboFiltreAbonnes;
    
    // Tableau
    QTableWidget *tableWidgetCreators;
    
    // Onglet Statistiques
    QWidget *ongletStatistiques;
    QLabel *labelTotalCreators;
    QLabel *labelStatistiquesSpecialites;
    QLabel *labelStatistiquesAbonnes;
    
    // DAO
    CreatorDAO *creatorDAO;
    
    // Variables
    int creatorSelectionneId;
};

#endif // CREATORWIDGET_H
