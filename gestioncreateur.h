#ifndef GESTIONCREATEUR_H
#define GESTIONCREATEUR_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class GestionCreateur : public QObject
{
    Q_OBJECT
public:
    explicit GestionCreateur(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~GestionCreateur();

public slots:
    void afficher();

private slots:
    void ajouter();
    void supprimer();
    void modifier();
    void rechercher();
    void trier();

private:
    Ui::MainWindow *ui;
    void remplirTableAvecQuery(QSqlQuery &query);
};

#endif // GESTIONCREATEUR_H
