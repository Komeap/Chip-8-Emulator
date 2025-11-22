#Projet CHIP-8 Emulator

Bienvenue dans la documentation de mon projet émulateur chip-8

## Compilation

Pour compiler ce projet on utilise un MakeFile, pour l'uitliser ouvrez un terminal à la racine et lancez :

 - make clean (Nettoie le projet)
 - make (créer l'éxécutable)
 - ./chip8 (nom de l'éxécutable)

 Si on rajoute des fichier librairies, il faut modifier la ligne du Makefile LIBS = ... -lMalibrairie (sans le lib au debut libmain -> -lmain)

 Si on veut changer le fichier à éxécuter de maniére permanante, il faut modifier SRC = ...

 Si on veut compiler plusiuer fichier .c ensemble SRC = fichier1.c fichier2.c ..., ou SRC = $(wildcard *.c) éxécute tous les .c disponible


##  Doxygen (Cheat Sheet)

Pour voir la doc, lancer dans un navigateur docs/html/index.html (Si, vous lisez ça normalement c'est OK)

Déja configurer, lorsque vous modifiez entrer dans le terminal '''doxygen Doxyfile'''

Voici la liste des balises (`@`) à utiliser pour documenter le projet efficacement.

### 1. Les indispensables (Fonctions & Méthodes)
* **`@brief`** : Description courte (une phrase) qui apparaît dans les listes sommaires.
* **`@details`** : (Optionnel) Description longue. Tout texte mis après une ligne vide sous le `@brief` est considéré comme du détail.
* **`@file`** : À mettre tout en haut d'un fichier pour le décrire.
* **`@param [nom]`** : Décrit un paramètre. Précisez les unités si nécessaire (ex: ms, pixels).
* **`@return`** : Décrit ce que renvoie la fonction.
* **`@retval [valeur]`** : Pour détailler une valeur de retour spécifique (ex: `@retval -1 Erreur mémoire`).

### 2. Alertes & Remarques (Très utile pour le Chip-8)
* **`@note`** : Ajoute un encadré "Note" pour une info complémentaire.
* **`@warning`** : Ajoute un encadré rouge "Avertissement" (ex: "Ne pas appeler avant init()").
* **`@attention`** : Similaire à warning, pour attirer l'attention sur un point critique.

### 3. Suivi de projet & Méta-données
* **`@todo`** : Signale une fonctionnalité manquante ou à améliorer. Doxygen crée une page récapitulative de tous les TODOs.
* **`@bug`** : Signale un bug connu qui n'est pas encore corrigé.
* **`@deprecated`** : Indique qu'une fonction est obsolète et ne devrait plus être utilisée (utile si tu refais ton architecture).

### 4. Exemples de code
Pour montrer comment utiliser une fonction directement dans la doc :
* **`@code` ... `@endcode`** : Permet d'insérer un bloc de code coloré.

## Utilisation de Git(hub) 

### Commande de base (dans l'odre)
* git pull :  !! A faire toujours avant de commencer à travailler !! récupére les modifications de l'autre
* Tu codes ...
* Verifier que ca marche toujours make, ./chip8
* git add .
* git commit -m "Information sur ce que tu a fait " (Important ++ pour savoir ce que l'autre a fait et pour come back)
* git push envoie les modifs

### Commande complementaire

* git status : affiche le status, 

        Rouge :Fichier modifié mais pas pris en compte (faire git add)

        Fichier prêt à être validé (faire git commit)

* git log  : affiche le graph des commit 

        --oneline : les recents sur une ligne

        --all : tous

        --graph : fait un graph

* git restore . : efface tous depuis le dernier commit

### Branche

Pour travailler sur deux truc diff en même temps sans se gener
* git checkout -b nom-de-ma-branche : Crée la branche et te met dessus direct
* git branch : te dit sur qu'elle branche tu est (branche1 branch2 *BrancheActuelle)

### Merge des branches après un travaille en paralléle

* git .add + git commit !! Pas oublier d'enregistrer (localement) les modif de l'autre branche avant de changer de branche !!
* git checkout branche1 : revenir sur la branche branche1

         !! toujours commit + add . avant, pour garder les modification sur l'autre branche !!

* git pull : Au cas où il y a eut des modif sur la main
* git merge branche2 : fusion branche2 avec la branche où l'on est
* git push : envoie le tous
* git branch -d branch2 : supprimer branch2 une fois que tous est ok (Pas obligatoire)


