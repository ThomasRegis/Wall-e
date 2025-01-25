# Projet Wall-E : Répository Principal

Bienvenue dans le répertoire principal de notre projet Wall-E ! Ce dépôt est organisé en trois packages distincts, chacun ayant un rôle spécifique dans le fonctionnement global du robot. Vous trouverez ici une description des packages et leurs fonctionnalités respectives.

---

## 📦 Packages

### 1. **Calibration**
- **Description :**  
Ce package contient les outils nécessaires pour calibrer les unités de mesure inertielle (**IMU**).  
- **Objectif :**  
Garantir des mesures précises en réduisant les erreurs de biais et de dérive des capteurs IMU.  
- **Contenu :**  
  - Scripts Python pour la calibration.
  - Données d'exemple pour les tests de calibration.

---

### 2. **Kalman**
- **Description :**  
Ce package implémente un filtre de Kalman pour fusionner les données provenant des IMU.  
- **Objectif :**  
Obtenir une estimation fiable et stable en combinant les données des capteurs et en réduisant le bruit.  
- **Contenu :**  
  - Implémentation du filtre de Kalman.
  - Fichiers de configuration pour ajuster les paramètres selon les besoins.

---

### 3. **Arduino-Tools**
- **Description :**  
Ce package contient le code nécessaire pour utiliser tous les capteurs du robot et contrôler ses déplacements.  
- **Objectif :**  
Gérer les capteurs et le système moteur pour permettre à Wall-E d'interagir avec son environnement.  
- **Contenu :**  
  - Sketchs Arduino pour les capteurs (ex. : ultrasons, gyroscopes, etc.).
  - Codes pour contrôler les moteurs et les déplacements.

---

## 🚀 Objectif Global
L'objectif principal de ce projet est de permettre à notre robot Wall-E de se déplacer de manière autonome tout en interprétant son environnement grâce aux capteurs et aux outils de fusion des données.

---
