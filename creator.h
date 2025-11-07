#ifndef CREATOR_H
#define CREATOR_H

#include <QString>
#include <QDate>
#include <QDateTime>

class Creator
{
public:
    Creator();
    Creator(int id, const QString& nom, const QString& prenom, 
            const QString& email, const QString& telephone,
            const QString& specialite, int abonnes, 
            const QDate& dateInscription);
    
    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getEmail() const { return email; }
    QString getTelephone() const { return telephone; }
    QString getSpecialite() const { return specialite; }
    int getAbonnes() const { return abonnes; }
    QDate getDateInscription() const { return dateInscription; }
    
    // Setters
    void setId(int id) { this->id = id; }
    void setNom(const QString& nom) { this->nom = nom; }
    void setPrenom(const QString& prenom) { this->prenom = prenom; }
    void setEmail(const QString& email) { this->email = email; }
    void setTelephone(const QString& telephone) { this->telephone = telephone; }
    void setSpecialite(const QString& specialite) { this->specialite = specialite; }
    void setAbonnes(int abonnes) { this->abonnes = abonnes; }
    void setDateInscription(const QDate& date) { this->dateInscription = date; }
    
    // Méthodes utilitaires
    QString getNomComplet() const { return prenom + " " + nom; }
    bool isValid() const;
    QString toString() const;
    
private:
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    QString specialite;
    int abonnes;
    QDate dateInscription;
};

#endif // CREATOR_H
