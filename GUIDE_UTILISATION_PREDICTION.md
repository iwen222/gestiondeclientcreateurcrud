# Guide d'utilisation - Prédiction du meilleur moment et plateforme

## 🎯 Objectif
Cette fonctionnalité vous permet de **maximiser l'impact de votre contenu** en déterminant :
- ⏰ Le **meilleur moment** pour publier
- 🌐 La **meilleure plateforme** à utiliser

## 📋 Prérequis
- Application compilée avec Qt 5.x ou 6.x
- Tous les fichiers du projet correctement intégrés

## 🚀 Comment utiliser la fonctionnalité

### Étape 1: Accéder à la fonctionnalité
1. Lancez l'application
2. Dans le menu latéral gauche, cliquez sur le bouton **"Prédiction Contenu"**
3. Vous accédez à l'interface de prédiction avec deux sections :
   - **Formulaire de saisie** (gauche)
   - **Résultats** (droite)

### Étape 2: Remplir le formulaire

#### Type de contenu
Sélectionnez parmi :
- 📹 **Video courte** - Contenu de moins de 60 secondes (TikTok, Reels)
- 🎬 **Video longue** - Contenu de plus de 1 minute (YouTube, IGTV)
- 🖼️ **Image** - Photo, infographie
- 📝 **Texte** - Post textuel, tweet
- 📄 **Article** - Article de blog, publication longue
- 🎙️ **Podcast** - Épisode audio

#### Public cible
Choisissez votre audience :
- 👶 **Jeunes (18-25 ans)** - Génération Z, étudiants
- 👨‍💼 **Jeunes adultes (26-35 ans)** - Millennials actifs
- 💼 **Professionnels** - Travailleurs, entrepreneurs
- 🏢 **Entreprises** - B2B, décideurs
- 🌍 **Grand public** - Audience générale
- 📚 **Étudiants** - Public scolaire/universitaire

#### Date de début (optionnel)
- Par défaut : Date et heure actuelles
- Vous pouvez sélectionner une date future
- L'algorithme adaptera les recommandations selon le jour de la semaine

#### Durée (optionnel, pour vidéos)
- Indiquez la durée en secondes
- Exemple : 30s = 30, 5min = 300, 1h = 3600
- Affecte la recommandation de plateforme

### Étape 3: Obtenir la prédiction
1. Cliquez sur le bouton **"🔮 Prédire le meilleur moment et plateforme"**
2. Une popup affiche un résumé avec le score de confiance
3. Les résultats détaillés apparaissent dans la section droite

### Étape 4: Interpréter les résultats

#### 📅 Meilleur moment
Format : "Jour JJ Mois AAAA à HH:mm"
- Exemple : *Mercredi 30 octobre 2024 à 18:00*
- Basé sur les heures de forte activité de votre public cible

#### 🌐 Meilleure plateforme
- Nom de la plateforme recommandée
- Exemple : *Instagram Reels*, *YouTube*, *LinkedIn*

#### 💡 Justification
- Explication détaillée de la recommandation
- Prend en compte tous les paramètres saisis
- Score de confiance (sur 100)

## 📊 Exemples d'utilisation

### Exemple 1: Créateur de contenu TikTok
**Situation :** Vous créez des vidéos courtes pour un public jeune

**Saisie :**
- Type: Video courte
- Public: Jeunes (18-25 ans)
- Date: Mercredi prochain
- Durée: 30 secondes

**Résultat attendu :**
- Moment: Mercredi vers 18h-19h
- Plateforme: TikTok ou Instagram Reels
- Score: 90-95/100

### Exemple 2: Professionnel LinkedIn
**Situation :** Vous publiez un article professionnel

**Saisie :**
- Type: Article
- Public: Professionnels
- Date: Mardi prochain

**Résultat attendu :**
- Moment: Mardi vers 9h-12h
- Plateforme: LinkedIn
- Score: 90-95/100

### Exemple 3: Podcasteur
**Situation :** Vous sortez un nouvel épisode de podcast

**Saisie :**
- Type: Podcast
- Public: Grand public
- Date: Jeudi prochain

**Résultat attendu :**
- Moment: Jeudi vers 13h ou 18h
- Plateforme: Spotify ou Apple Podcasts
- Score: 85-90/100

## 💡 Conseils pour optimiser vos publications

### ⏰ Meilleurs créneaux par public

**Jeunes / Étudiants**
- ✅ Soirée (17h-21h)
- ✅ Week-end matin (10h-12h)
- ❌ Heures de cours (9h-17h en semaine)

**Professionnels**
- ✅ Matin (7h-9h) - Avant le travail
- ✅ Pause déjeuner (12h-14h)
- ✅ Début de soirée (17h-19h) - Après le travail
- ❌ Milieu de journée (10h-12h, 14h-17h)

**Grand Public**
- ✅ Pause déjeuner (12h-14h)
- ✅ Soirée (18h-21h)
- ✅ Week-end après-midi (14h-17h)

### 🌐 Plateformes par type de contenu

| Type de contenu | Plateforme primaire | Plateformes secondaires |
|----------------|---------------------|------------------------|
| Video courte | TikTok | Instagram Reels, YouTube Shorts |
| Video longue | YouTube | Facebook, LinkedIn (pro) |
| Image | Instagram | Pinterest, Facebook |
| Texte | Twitter | LinkedIn (pro), Facebook |
| Article | LinkedIn (pro) | Medium, Facebook |
| Podcast | Spotify | Apple Podcasts, YouTube |

## ⚙️ Personnalisation avancée

### Modifier les heures optimales
Éditez le fichier `prediction.cpp`, fonction `initialiserDonnees()` :
```cpp
heuresOptimales["Lundi"] = {8, 9, 12, 13, 17, 18, 19};
```

### Modifier les scores de plateformes
Éditez le fichier `prediction.cpp`, section scores :
```cpp
scoresPlateforme["Video courte"]["TikTok"] = 95;
```

### Ajouter de nouvelles plateformes
1. Ajoutez les scores dans `initialiserDonnees()`
2. Recompilez l'application

## 🔧 Dépannage

### Le bouton "Prédiction Contenu" n'apparaît pas
- Vérifiez que `mainwindow.ui` contient le bouton `btn_prediction`
- Recompilez le projet

### Erreur de compilation
- Assurez-vous que tous les fichiers sont dans le répertoire du projet
- Vérifiez que `gestiondeclient.pro` contient tous les fichiers
- Nettoyez et recompilez : `qmake && make clean && make`

### Les résultats ne s'affichent pas
- Vérifiez la console pour les messages d'erreur
- Assurez-vous que tous les champs du formulaire sont remplis

## 📈 Améliorations futures

Vous pouvez étendre cette fonctionnalité en ajoutant :
- 🔗 Connexion aux API des réseaux sociaux
- 📊 Analyse de vos publications passées
- 🤖 Intelligence artificielle pour prédictions personnalisées
- 📅 Calendrier de publication intégré
- 📧 Notifications par email/SMS

## 📞 Support
Pour toute question ou suggestion, contactez l'équipe de développement.

---

**Version:** 1.0  
**Date:** Octobre 2024  
**Auteur:** Équipe NeoCreate
