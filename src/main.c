#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../include/display/display.h"
#include "../include/memory.h"
#include "misc/debug.h"
#include "../include/processor.h"



int main(void) {
    struct RAM *maMemoire = init_RAM();

    //printf("%d\n", load_RAM(maMemoire, "/home/julien/Cours/Chip-8_Project/1-chip8-logo.ch8"));

    int rom_size = load_RAM(maMemoire, "chip8_load/2-ibm-logo.ch8");
    if (rom_size == -1) {
        printf("Erreur lors du chargement de la ROM\n");
        return 1;
    }
    printf("ROM chargée, taille: %d bytes\n", rom_size);
    
    //print_RAM(maMemoire);

    //printf("%s\n",instruction_as_str((uint16_t)0xFF));
    print_instruct(maMemoire);
    struct Display *monDisplay = (struct Display*)calloc(1, sizeof(struct Display));
    if (Display_init(monDisplay, 10) != 0) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'affichage.\n");
        return 1;
    }

    struct CPU *monCPU = init_CPU(maMemoire, monDisplay);

    printf("Démarrage de l'émulation...\n");
    
    bool running = true;
    SDL_Event event;

    for (int i  = 0; i < 2000; i++) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        cpu_cycle(monCPU);
        Display_update(monDisplay);
        usleep(50000000);
    }
    
    printf("Fermeture de l'émulateur.\n");
    Display_destroy(monDisplay);
    free(monDisplay);
    // Ajoutez free(maMemoire) et free(monCPU) si vous avez les fonctions pour.
    
    return 0;
}