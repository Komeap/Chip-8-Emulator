#Projet CHIP-8 Emulator

Bienvenue sur la documentation de mon émulateur CHIP-8, développé en C avec la bibliothèque SDL2.

### Installation des dépendances

Avant de compiler, assurez-vous d'avoir les bibliothèques de développement SDL2 installées (notamment SDL_ttf pour la gestion du texte).
Sous Linux : 
        sudo apt-get install libsdl2-ttf-dev

## Lancement du Projet

Pour compiler ce projet on utilise un MakeFile, pour l'uitliser ouvrez un terminal à la racine et lancez :

 - make clean (Nettoie le projet)
 - make (créer l'éxécutable)
 - ./chip8 (nom de l'éxécutable)

 - make clean && make && ./chip8

## Page D'acceuil Menu / Utilisation

Le Menu permet de choisir la ROM a charger en mémoire. 
Utiliser les fléches HAUTS/BAS pour selectionerla ROM, 
Et utiliser la touche entrée Pour valider et lancer la ROM
La touche echap permet de quitter le ROM et le Menu d'acceuil (Ce qui ferme propremement le projet)

## Objectif de réalisation atteint

        - logo : OK
        - IBM : OK
        - Corax : OK
        - Flags : OK
        - Quirks : OK
        - Keypad : OK
        - Beep : OK

        - Différents jeux trouver sur internet : OK

        - Bonus : Page de Menu C, lancement et séléction de la ROM



