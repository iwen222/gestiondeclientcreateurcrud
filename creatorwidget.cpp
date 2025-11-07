#include "creatorwidget.h"
#include <QApplication>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDate>
#include <QDebug>

CreatorWidget::CreatorWidget(QWidget *parent)
    : QWidget(parent), creatorDAO(new CreatorDAO(this)), creatorSelectionneId(-1)
{
    setupUI();
    connecterSignaux();
    chargerDonnees();
}

CreatorWidget::~CreatorWidget()
{
    delete creatorDAO;
}

void CreatorWidget::setupUI()
{
    setWindowTitle("Gestion des Créateurs");
    resize(1200, 800);
    
    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Créer les onglets
    tabWidget = new QTabWidget(this);
    
    // Onglet Gestion
    ongletGestion = new QWidget();
    setupFormulaire();
    setupTableau();
    setupBoutons();
    
    // Onglet Statistiques
    ongletStatistiques = new QWidget();
    mettreAJourStatistiques();
    
    tabWidget->addTab(ongletGestion, "Gestion des Créateurs");
    tabWidget->addTab(ongletStatistiques, "Statistiques");
    
    mainLayout->addWidget(tabWidget);
}

void CreatorWidget::setupFormulaire()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(ongletGestion);
    
    // Formulaire à gauche
    groupBoxFormulaire = new QGroupBox("Informations du Créateur");
    groupBoxFormulaire->setMaximumWidth(400);
    
    QFormLayout *formLayout = new QFormLayout(groupBoxFormulaire);
    
    // Champs du formulaire
    lineEditNom = new QLineEdit();
    lineEditPrenom = new QLineEdit();
    lineEditEmail = new QLineEdit();
    lineEditTelephone = new QLineEdit();
    lineEditSpecialite = new QLineEdit();
    spinBoxAbonnes = new QSpinBox();
    spinBoxAbonnes->setRange(0, 10000000);
    spinBoxAbonnes->setValue(0);
    
    dateEditInscription = new QDateEdit();
    dateEditInscription->setDate(QDate::currentDate());
    dateEditInscription->setCalendarPopup(true);
    
    // Ajouter les champs au formulaire
    formLayout->addRow("Nom *:", lineEditNom);
    formLayout->addRow("Prénom *:", lineEditPrenom);
    formLayout->addRow("Email *:", lineEditEmail);
    formLayout->addRow("Téléphone:", lineEditTelephone);
    formLayout->addRow("Spécialité:", lineEditSpecialite);
    formLayout->addRow("Abonnés:", spinBoxAbonnes);
    formLayout->addRow("Date inscription:", dateEditInscription);
    
    // Actions
    groupBoxActions = new QGroupBox("Actions");
    QVBoxLayout *actionsLayout = new QVBoxLayout(groupBoxActions);
    
    btnAjouter = new QPushButton("➕ Ajouter");
    btnModifier = new QPushButton("✏️ Modifier");
    btnSupprimer = new QPushButton("🗑️ Supprimer");
    btnVider = new QPushButton("🧹 Vider");
    btnActualiser = new QPushButton("🔄 Actualiser");
    btnExporterPDF = new QPushButton("📄 Exporter PDF");
    
    // Style des boutons
    btnAjouter->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; }");
    btnModifier->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 8px; }");
    btnSupprimer->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; padding: 8px; }");
    btnVider->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; padding: 8px; }");
    btnActualiser->setStyleSheet("QPushButton { background-color: #9C27B0; color: white; font-weight: bold; padding: 8px; }");
    btnExporterPDF->setStyleSheet("QPushButton { background-color: #607D8B; color: white; font-weight: bold; padding: 8px; }");
    
    actionsLayout->addWidget(btnAjouter);
    actionsLayout->addWidget(btnModifier);
    actionsLayout->addWidget(btnSupprimer);
    actionsLayout->addWidget(btnVider);
    actionsLayout->addWidget(btnActualiser);
    actionsLayout->addWidget(btnExporterPDF);
    actionsLayout->addStretch();
    
    // Recherche
    groupBoxRecherche = new QGroupBox("Recherche et Filtres");
    QFormLayout *rechercheLayout = new QFormLayout(groupBoxRecherche);
    
    lineEditRecherche = new QLineEdit();
    lineEditRecherche->setPlaceholderText("Rechercher par nom, prénom, email...");
    
    comboFiltreSpecialite = new QComboBox();
    comboFiltreSpecialite->addItem("Toutes les spécialités");
    comboFiltreSpecialite->addItem("Mode");
    comboFiltreSpecialite->addItem("Beauté");
    comboFiltreSpecialite->addItem("Gaming");
    comboFiltreSpecialite->addItem("Tech");
    comboFiltreSpecialite->addItem("Lifestyle");
    comboFiltreSpecialite->addItem("Food");
    comboFiltreSpecialite->addItem("Travel");
    comboFiltreSpecialite->addItem("Fitness");
    comboFiltreSpecialite->addItem("Art");
    comboFiltreSpecialite->addItem("Music");
    
    comboFiltreAbonnes = new QComboBox();
    comboFiltreAbonnes->addItem("Tous les abonnés");
    comboFiltreAbonnes->addItem("Débutant (< 1K)");
    comboFiltreAbonnes->addItem("Intermédiaire (1K-10K)");
    comboFiltreAbonnes->addItem("Avancé (10K-100K)");
    comboFiltreAbonnes->addItem("Expert (100K+)");
    
    btnRechercher = new QPushButton("🔍 Rechercher");
    btnRechercher->setStyleSheet("QPushButton { background-color: #3F51B5; color: white; font-weight: bold; padding: 8px; }");
    
    rechercheLayout->addRow("Recherche:", lineEditRecherche);
    rechercheLayout->addRow("Spécialité:", comboFiltreSpecialite);
    rechercheLayout->addRow("Abonnés:", comboFiltreAbonnes);
    rechercheLayout->addRow("", btnRechercher);
    
    // Layout gauche
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(groupBoxFormulaire);
    leftLayout->addWidget(groupBoxActions);
    leftLayout->addWidget(groupBoxRecherche);
    
    mainLayout->addLayout(leftLayout);
}

void CreatorWidget::setupTableau()
{
    QHBoxLayout *mainLayout = qobject_cast<QHBoxLayout*>(ongletGestion->layout());
    
    // Tableau à droite
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    QLabel *labelTableau = new QLabel("Liste des Créateurs");
    labelTableau->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    
    tableWidgetCreators = new QTableWidget();
    tableWidgetCreators->setColumnCount(8);
    tableWidgetCreators->setHorizontalHeaderLabels({
        "ID", "Nom", "Prénom", "Email", "Téléphone", 
        "Spécialité", "Abonnés", "Date Inscription"
    });
    
    // Configuration du tableau
    tableWidgetCreators->setAlternatingRowColors(true);
    tableWidgetCreators->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetCreators->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidgetCreators->setSortingEnabled(true);
    
    // Redimensionner les colonnes
    tableWidgetCreators->horizontalHeader()->setStretchLastSection(true);
    tableWidgetCreators->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    
    rightLayout->addWidget(labelTableau);
    rightLayout->addWidget(tableWidgetCreators);
    
    mainLayout->addLayout(rightLayout);
}

void CreatorWidget::setupBoutons()
{
    // Les boutons sont déjà configurés dans setupFormulaire()
}

void CreatorWidget::connecterSignaux()
{
    // Boutons
    connect(btnAjouter, &QPushButton::clicked, this, &CreatorWidget::ajouterCreator);
    connect(btnModifier, &QPushButton::clicked, this, &CreatorWidget::modifierCreator);
    connect(btnSupprimer, &QPushButton::clicked, this, &CreatorWidget::supprimerCreator);
    connect(btnVider, &QPushButton::clicked, this, &CreatorWidget::viderFormulaire);
    connect(btnActualiser, &QPushButton::clicked, this, &CreatorWidget::actualiserListe);
    connect(btnExporterPDF, &QPushButton::clicked, this, &CreatorWidget::exporterPDF);
    connect(btnRechercher, &QPushButton::clicked, this, &CreatorWidget::rechercherCreators);
    
    // Tableau
    connect(tableWidgetCreators, &QTableWidget::itemSelectionChanged, this, &CreatorWidget::selectionnerCreator);
    
    // DAO
    connect(creatorDAO, &CreatorDAO::creatorAjoute, this, &CreatorWidget::onCreatorAjoute);
    connect(creatorDAO, &CreatorDAO::creatorSupprime, this, &CreatorWidget::onCreatorSupprime);
    connect(creatorDAO, &CreatorDAO::creatorModifie, this, &CreatorWidget::onCreatorModifie);
    connect(creatorDAO, &CreatorDAO::erreurOccurred, this, &CreatorWidget::onErreurOccurred);
}

void CreatorWidget::chargerDonnees()
{
    QList<Creator> creators = creatorDAO->getAllCreators();
    afficherCreatorsDansTableau(creators);
}

void CreatorWidget::afficherCreatorsDansTableau(const QList<Creator>& creators)
{
    tableWidgetCreators->setRowCount(creators.size());
    
    for (int i = 0; i < creators.size(); ++i) {
        const Creator& creator = creators[i];
        
        tableWidgetCreators->setItem(i, 0, new QTableWidgetItem(QString::number(creator.getId())));
        tableWidgetCreators->setItem(i, 1, new QTableWidgetItem(creator.getNom()));
        tableWidgetCreators->setItem(i, 2, new QTableWidgetItem(creator.getPrenom()));
        tableWidgetCreators->setItem(i, 3, new QTableWidgetItem(creator.getEmail()));
        tableWidgetCreators->setItem(i, 4, new QTableWidgetItem(creator.getTelephone()));
        tableWidgetCreators->setItem(i, 5, new QTableWidgetItem(creator.getSpecialite()));
        tableWidgetCreators->setItem(i, 6, new QTableWidgetItem(QString::number(creator.getAbonnes())));
        tableWidgetCreators->setItem(i, 7, new QTableWidgetItem(creator.getDateInscription().toString("dd/MM/yyyy")));
    }
}

void CreatorWidget::ajouterCreator()
{
    if (!validerFormulaire()) {
        return;
    }
    
    Creator creator = getCreatorDepuisFormulaire();
    if (creatorDAO->ajouterCreator(creator)) {
        viderFormulaire();
        actualiserListe();
        mettreAJourStatistiques();
    }
}

void CreatorWidget::modifierCreator()
{
    if (creatorSelectionneId == -1) {
        afficherMessage("Erreur", "Veuillez sélectionner un créateur à modifier.", true);
        return;
    }
    
    if (!validerFormulaire()) {
        return;
    }
    
    Creator creator = getCreatorDepuisFormulaire();
    creator.setId(creatorSelectionneId);
    
    if (creatorDAO->modifierCreator(creator)) {
        viderFormulaire();
        actualiserListe();
        mettreAJourStatistiques();
    }
}

void CreatorWidget::supprimerCreator()
{
    if (creatorSelectionneId == -1) {
        afficherMessage("Erreur", "Veuillez sélectionner un créateur à supprimer.", true);
        return;
    }
    
    int reponse = QMessageBox::question(this, "Confirmation", 
                                       "Êtes-vous sûr de vouloir supprimer ce créateur ?",
                                       QMessageBox::Yes | QMessageBox::No);
    
    if (reponse == QMessageBox::Yes) {
        if (creatorDAO->supprimerCreator(creatorSelectionneId)) {
            viderFormulaire();
            actualiserListe();
            mettreAJourStatistiques();
        }
    }
}

void CreatorWidget::afficherTousCreators()
{
    actualiserListe();
}

void CreatorWidget::rechercherCreators()
{
    QString critere = lineEditRecherche->text().trimmed();
    QList<Creator> creators;
    
    if (critere.isEmpty()) {
        creators = creatorDAO->getAllCreators();
    } else {
        creators = creatorDAO->rechercherCreators(critere);
    }
    
    // Appliquer les filtres
    if (comboFiltreSpecialite->currentIndex() > 0) {
        QString specialite = comboFiltreSpecialite->currentText();
        creators = creatorDAO->getCreatorsBySpecialite(specialite);
    }
    
    if (comboFiltreAbonnes->currentIndex() > 0) {
        QString categorie = comboFiltreAbonnes->currentText();
        int minAbonnes = 0, maxAbonnes = -1;
        
        if (categorie.contains("1K-10K")) {
            minAbonnes = 1000;
            maxAbonnes = 10000;
        } else if (categorie.contains("10K-100K")) {
            minAbonnes = 10000;
            maxAbonnes = 100000;
        } else if (categorie.contains("100K+")) {
            minAbonnes = 100000;
        } else if (categorie.contains("< 1K")) {
            maxAbonnes = 1000;
        }
        
        creators = creatorDAO->getCreatorsByAbonnes(minAbonnes, maxAbonnes);
    }
    
    afficherCreatorsDansTableau(creators);
}

void CreatorWidget::selectionnerCreator()
{
    int row = tableWidgetCreators->currentRow();
    if (row >= 0) {
        creatorSelectionneId = tableWidgetCreators->item(row, 0)->text().toInt();
        Creator creator = creatorDAO->getCreatorById(creatorSelectionneId);
        afficherCreatorDansFormulaire(creator);
    }
}

void CreatorWidget::viderFormulaire()
{
    lineEditNom->clear();
    lineEditPrenom->clear();
    lineEditEmail->clear();
    lineEditTelephone->clear();
    lineEditSpecialite->clear();
    spinBoxAbonnes->setValue(0);
    dateEditInscription->setDate(QDate::currentDate());
    creatorSelectionneId = -1;
}

void CreatorWidget::exporterPDF()
{
    // TODO: Implémenter l'export PDF
    afficherMessage("Info", "Fonctionnalité d'export PDF à implémenter.");
}

void CreatorWidget::actualiserListe()
{
    chargerDonnees();
}

void CreatorWidget::onCreatorAjoute(const Creator& creator)
{
    afficherMessage("Succès", "Créateur ajouté avec succès !");
}

void CreatorWidget::onCreatorSupprime(int id)
{
    afficherMessage("Succès", "Créateur supprimé avec succès !");
}

void CreatorWidget::onCreatorModifie(const Creator& creator)
{
    afficherMessage("Succès", "Créateur modifié avec succès !");
}

void CreatorWidget::onErreurOccurred(const QString& message)
{
    afficherMessage("Erreur", message, true);
}

void CreatorWidget::afficherCreatorDansFormulaire(const Creator& creator)
{
    lineEditNom->setText(creator.getNom());
    lineEditPrenom->setText(creator.getPrenom());
    lineEditEmail->setText(creator.getEmail());
    lineEditTelephone->setText(creator.getTelephone());
    lineEditSpecialite->setText(creator.getSpecialite());
    spinBoxAbonnes->setValue(creator.getAbonnes());
    dateEditInscription->setDate(creator.getDateInscription());
}

Creator CreatorWidget::getCreatorDepuisFormulaire()
{
    Creator creator;
    creator.setNom(lineEditNom->text().trimmed());
    creator.setPrenom(lineEditPrenom->text().trimmed());
    creator.setEmail(lineEditEmail->text().trimmed());
    creator.setTelephone(lineEditTelephone->text().trimmed());
    creator.setSpecialite(lineEditSpecialite->text().trimmed());
    creator.setAbonnes(spinBoxAbonnes->value());
    creator.setDateInscription(dateEditInscription->date());
    return creator;
}

bool CreatorWidget::validerFormulaire()
{
    if (lineEditNom->text().trimmed().isEmpty()) {
        afficherMessage("Erreur", "Le nom est obligatoire.", true);
        lineEditNom->setFocus();
        return false;
    }
    
    if (lineEditPrenom->text().trimmed().isEmpty()) {
        afficherMessage("Erreur", "Le prénom est obligatoire.", true);
        lineEditPrenom->setFocus();
        return false;
    }
    
    if (lineEditEmail->text().trimmed().isEmpty()) {
        afficherMessage("Erreur", "L'email est obligatoire.", true);
        lineEditEmail->setFocus();
        return false;
    }
    
    if (!lineEditEmail->text().contains("@")) {
        afficherMessage("Erreur", "L'email doit être valide.", true);
        lineEditEmail->setFocus();
        return false;
    }
    
    return true;
}

void CreatorWidget::afficherMessage(const QString& titre, const QString& message, bool erreur)
{
    if (erreur) {
        QMessageBox::warning(this, titre, message);
    } else {
        QMessageBox::information(this, titre, message);
    }
}

void CreatorWidget::mettreAJourStatistiques()
{
    QVBoxLayout *layout = new QVBoxLayout(ongletStatistiques);
    
    // Total créateurs
    int total = creatorDAO->getNombreTotalCreators();
    labelTotalCreators = new QLabel(QString("Total des créateurs : %1").arg(total));
    labelTotalCreators->setStyleSheet("font-size: 18px; font-weight: bold; color: #2196F3;");
    layout->addWidget(labelTotalCreators);
    
    // Statistiques par spécialité
    QMap<QString, int> statsSpecialites = creatorDAO->getStatistiquesSpecialites();
    labelStatistiquesSpecialites = new QLabel("Statistiques par spécialité :");
    labelStatistiquesSpecialites->setStyleSheet("font-size: 14px; font-weight: bold;");
    layout->addWidget(labelStatistiquesSpecialites);
    
    for (auto it = statsSpecialites.begin(); it != statsSpecialites.end(); ++it) {
        QLabel *label = new QLabel(QString("• %1 : %2 créateurs").arg(it.key()).arg(it.value()));
        layout->addWidget(label);
    }
    
    layout->addSpacing(20);
    
    // Statistiques par abonnés
    QMap<QString, int> statsAbonnes = creatorDAO->getStatistiquesAbonnes();
    labelStatistiquesAbonnes = new QLabel("Statistiques par niveau d'abonnés :");
    labelStatistiquesAbonnes->setStyleSheet("font-size: 14px; font-weight: bold;");
    layout->addWidget(labelStatistiquesAbonnes);
    
    for (auto it = statsAbonnes.begin(); it != statsAbonnes.end(); ++it) {
        QLabel *label = new QLabel(QString("• %1 : %2 créateurs").arg(it.key()).arg(it.value()));
        layout->addWidget(label);
    }
    
    layout->addStretch();
}
