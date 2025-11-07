#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include "gestioncreateur.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GestionCreateur *gestionCreateur = new GestionCreateur(ui, this);


    // Boutons menu (doivent exister dans mainwindow.ui)
    connect(ui->btn_staff,    &QPushButton::clicked, this, &MainWindow::showStaff);
    connect(ui->btn_client,   &QPushButton::clicked, this, &MainWindow::showClient);
    connect(ui->btn_materiel, &QPushButton::clicked, this, &MainWindow::showMateriel);
    connect(ui->btn_projet,   &QPushButton::clicked, this, &MainWindow::showProjet);
    connect(ui->btn_sponsor,  &QPushButton::clicked, this, &MainWindow::showSponsor);
    connect(ui->btn_createur, &QPushButton::clicked, this, &MainWindow::showCreateur);

    // Page d’accueil au démarrage (adapte l’index si besoin)
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Adapte les indices selon l’ordre réel des pages dans stackedWidget
void MainWindow::showStaff()    { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::showClient()   { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::showMateriel() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::showProjet()   { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::showSponsor()  { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::showCreateur() { ui->stackedWidget->setCurrentIndex(5); }
