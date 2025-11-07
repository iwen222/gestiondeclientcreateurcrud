#include "creator.h"
#include <QDebug>

Creator::Creator()
    : id(-1), abonnes(0), dateInscription(QDate::currentDate())
{
}

Creator::Creator(int id, const QString& nom, const QString& prenom, 
                 const QString& email, const QString& telephone,
                 const QString& specialite, int abonnes, 
                 const QDate& dateInscription)
    : id(id), nom(nom), prenom(prenom), email(email), telephone(telephone),
      specialite(specialite), abonnes(abonnes), dateInscription(dateInscription)
{
}

bool Creator::isValid() const
{
    return !nom.isEmpty() && !prenom.isEmpty() && !email.isEmpty();
}

QString Creator::toString() const
{
    return QString("Creator{id=%1, nom=%2, prenom=%3, email=%4, telephone=%5, "
                  "specialite=%6, abonnes=%7, dateInscription=%8}")
            .arg(id)
            .arg(nom)
            .arg(prenom)
            .arg(email)
            .arg(telephone)
            .arg(specialite)
            .arg(abonnes)
            .arg(dateInscription.toString("yyyy-MM-dd"));
}
