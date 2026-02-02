# Microrécif - Simulation d'Écosystème Marin

## Description
Ce projet est une simulation d'un micro-récif implémentée en C++ avec une interface graphique utilisant GTKmm. Le programme simule les interactions entre trois types de formes de vie :
- **Algues** : Producteurs primaires qui apparaissent aléatoirement.
- **Coraux** : Organismes immobiles qui grandissent et se reproduisent.
- **Charognards** : Organismes mobiles qui se nourrissent de coraux morts.

La simulation permet de visualiser ces interactions en temps réel, de contrôler l'exécution (start/stop/step) et de gérer des fichiers de configuration.

## Auteurs
- Tahri Malik
- Laachfoubi Kamil

## Prérequis
- Compilateur compatible C++17 (ex: g++)
- Bibliothèque GUI : `gtkmm-4.0`

## Compilation
Utilisez le `Makefile` inclus pour compiler le projet :

```bash
make
```

Pour nettoyer les fichiers objets et l'exécutable :

```bash
make clean
```

## Utilisation

Lancer la simulation en fournissant un fichier de configuration :

```bash
./projet samples/sim_full.txt
```

### Contrôles de l'Interface
- **Start/Stop** : Lance ou met en pause la simulation.
- **Step** : Avance la simulation d'un seul pas de temps.
- **Open** : Charger un nouveau fichier de configuration.
- **Save** : Sauvegarder l'état actuel de la simulation.
- **Exit** : Quitter l'application.
- **Naissance d'algue** : Case à cocher pour activer/désactiver l'apparition aléatoire de nouvelles algues.

## Structure du Projet
- `src/` : Fichiers sources (.cc)
- `include/` : Fichiers en-tête (.h)
- `samples/` : Fichiers d'exemple de simulation
- `build/` : Fichiers objets générés lors de la compilation

## Format des Fichiers de Simulation
Les fichiers de simulation sont des fichiers texte décrivant l'état initial du système (taille, nombre d'entités, propriétés de chaque entité). Voir les fichiers dans le dossier `samples/` pour des exemples.
