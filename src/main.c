#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <time.h>

#include "../include/display/display.h"
#include "../include/memory.h"
#include "misc/debug.h"
#include "../include/processor.h"
#include "../include/keyboard/keyboard.h"


int main(void) {
    srand(time(NULL));
    struct RAM *maMemoire = init_RAM();

    //printf("%d\n", load_RAM(maMemoire, "/home/julien/Cours/Chip-8_Project/1-chip8-logo.ch8"));

    int rom_size = load_RAM(maMemoire, "chip8_load/7-beep.ch8");
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

    struct Keyboard *MyKeyboard = (struct Keyboard*)calloc(1, sizeof(struct Keyboard));
    if (Keyboard_init(MyKeyboard) != 0) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'affichage.\n");
        return 1;
    }

    struct Speaker *MySpeaker = (struct Speaker*)calloc(1, sizeof(struct Speaker));
    if (Speaker_init(MySpeaker) != 0) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'affichage.\n");
        return 1;
    }


    struct CPU *monCPU = init_CPU(maMemoire, monDisplay, MyKeyboard, MySpeaker);

    printf("Démarrage de l'émulation...\n");
    
    bool running = true;
    SDL_Event event;

    Uint32 last_tick = SDL_GetTicks();
    double delta_accumulator = 0;
    
    int instructions_par_frame = 10;
    SDL_Keycode keymap[16] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_a, // 4 
    SDLK_z, // 5 
    SDLK_e, // 6
    SDLK_q, // 7 
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_w, // A 
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};

    /*while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        cpu_cycle(monCPU);
        Display_update(monDisplay);
        usleep(0);
    }*/
    
    while (running) {
        Uint32 current_tick = SDL_GetTicks();
        double dt = (current_tick - last_tick);
        last_tick = current_tick;
        delta_accumulator += dt;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        if (delta_accumulator >= 1.5) {
            delta_accumulator -= 1.5;
            for (int i = 0; i < instructions_par_frame; i++) {
                cpu_cycle(monCPU);
            }
            cpu_update_timers(monCPU);
            Display_update(monDisplay);
        }

        SDL_Delay(1);
    }

    printf("Fermeture de l'émulateur.\n");
    Display_destroy(monDisplay);
    Speaker_destroy(MySpeaker);
    free(monDisplay);
    free(MySpeaker);

    
    
    
    return 0;
}