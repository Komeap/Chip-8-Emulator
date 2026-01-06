#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_ttf.h> // Pour la police d'éeciture, install linux : sudo apt-get install libsdl2-ttf-dev
#include <time.h>

#include "../include/display/display.h"
#include "../include/memory.h"
#include "misc/debug.h"
#include "../include/processor.h"
#include "../include/keyboard/keyboard.h"


const char *ROM_LIST[] = {"chip8_load/1-chip8-logo.ch8", "chip8_load/2-ibm-logo.ch8", "chip8_load/3-corax+.ch8", "chip8_load/4-flags.ch8", "chip8_load/5-quirks.ch8",
                           "chip8_load/6-keypad.ch8", "chip8_load/7-beep.ch8", "chip8_load/BLINKY", "chip8_load/TETRIS", "chip8_load/PONG2", "chip8_load/TICTAC",
                        "chip8_load/BLITZ", "chip8_load/BRIX", "chip8_load/CONNECT4", "chip8_load/GUESS", "chip8_load/HIDDEN", "chip8_load/INVADERS", "chip8_load/KALEID",
                        "chip8_load/MAZE", "chip8_load/MERLIN", "chip8_load/MISSILE", "chip8_load/PUZZLE", "chip8_load/SYZYGY", "chip8_load/TANK", "chip8_load/UFO", "chip8_load/VBRIX",
                        "chip8_load/VERS", "chip8_load/WIPEOFF"};

const int ROM_COUNT = sizeof(ROM_LIST) / sizeof(ROM_LIST[0]);

void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

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
    SDLK_4, //C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};

int run_menu() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        return -1;
    }
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow("CHIP-8 MENU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    

    TTF_Font *font = TTF_OpenFont("Limited.ttf", 46);

    int selection = 0;
    bool in_menu = true;
    int result = -1;
    SDL_Event event;

    int items_per_col = (ROM_COUNT + 1) / 2;

    while (in_menu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                result = -1;
                in_menu = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: 
                        result = -1;
                        in_menu = false;
                        break;

                    case SDLK_UP:
                        selection--;
                        if (selection < 0){
                            selection = ROM_COUNT - 1;
                        }
                        break;

                    case SDLK_DOWN:
                        selection++;
                        if (selection >= ROM_COUNT){
                            selection = 0;
                        }
                        break;

                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        result = selection;
                        in_menu = false;
                        break;
                }
            }
        }

        // Affichage du Menu
        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        SDL_Color colTitre = {255, 200, 0, 255};
        draw_text(ren, font, "CHIP-8 EMULATOR", 200, 20, colTitre);

        for (int i = 0; i < ROM_COUNT; i++) {
            SDL_Color colText = {255, 255, 255, 255};
            char buffer[128];
            
            if (i == selection) {
                colText.g = 0;
                colText.b = 0; // Rchange la couleur pour celui select
                sprintf(buffer, "> %s", ROM_LIST[i]);
            } else {
                sprintf(buffer, "  %s", ROM_LIST[i]);
            }

            int col = (i < items_per_col) ? 0 : 1;
            int row = (col == 0) ? i : (i - items_per_col);
            
            int x_pos = 20 + (col * 320);
            int y_pos = 60 + (row * 28);

            draw_text(ren, font, buffer, x_pos, y_pos, colText);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    
    return result;
}

// Fonction qui lance une Rom
bool run_game(const char *rom_path, struct Display *monDisplay, struct Keyboard *MyKeyboard, struct Speaker *MySpeaker) {
    struct RAM *maMemoire = init_RAM();
    if (load_RAM(maMemoire, rom_path) == 0) {
        fprintf(stderr, "Erreur de  chargement ROM\n");
        free_RAM(maMemoire); 
        return true;
    }


    if (Display_init(monDisplay, 10) != 0) {
        free_RAM(maMemoire);
        return false;
    }
    
    Display_set_colors(monDisplay, 200, 50, 155, 50, 50, 50); 

    struct CPU *monCPU = init_CPU(maMemoire, monDisplay, MyKeyboard, MySpeaker);
    
    bool running = true;
    SDL_Event event;
    Uint32 last_tick = SDL_GetTicks();
    double delta_accumulator = 0;
    
    int instructions_par_frame = 20;
    
    while (running) {
        Uint32 current_tick = SDL_GetTicks();
        double dt = (current_tick - last_tick);
        last_tick = current_tick;
        delta_accumulator += dt;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                free_CPU(monCPU);
                free_RAM(maMemoire);
                Display_destroy(monDisplay);
                return false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        while (delta_accumulator >= 15) {
            delta_accumulator -= 15;
            for (int i = 0; i < instructions_par_frame; i++) {
                cpu_cycle(monCPU);
            }
            cpu_update_timers(monCPU);
        }
        Display_update(monDisplay);
        SDL_Delay(1);
    }

    Display_destroy(monDisplay);
    free_CPU(monCPU);
    free_RAM(maMemoire);

    return true;
}

int main() {
    srand(time(NULL));

    struct Display *monDisplay = (struct Display*)calloc(1, sizeof(struct Display));
    struct Keyboard *MyKeyboard = (struct Keyboard*)calloc(1, sizeof(struct Keyboard));
    struct Speaker *MySpeaker = (struct Speaker*)calloc(1, sizeof(struct Speaker));
    
    Keyboard_init(MyKeyboard);
    Speaker_init(MySpeaker);

    bool app_running = true;

    while (app_running) {
        int choix = run_menu();

        if (choix == -1) {
            app_running = false;
        } else {
            // Lance un jeu, creer ca propre fenetre
            bool back = run_game(ROM_LIST[choix], monDisplay, MyKeyboard, MySpeaker);
            if (!back) app_running = false;
        }
    }

    free(monDisplay);
    free(MyKeyboard);
    free(MySpeaker);
    SDL_Quit();

    return 0;
}

/*
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_rom>\n", argv[0]);
        return 1;
    }
    srand(time(NULL));
    struct RAM *maMemoire = init_RAM();

    //printf("%d\n", load_RAM(maMemoire, "/home/julien/Cours/Chip-8_Project/1-chip8-logo.ch8"));


    if (load_RAM(maMemoire, argv[1]) == 0) {
        fprintf(stderr, "Erreur chargement ROM.\n");
        return 1;
    }
    
    //print_RAM(maMemoire);

    //printf("%s\n",instruction_as_str((uint16_t)0xFF));
    //print_instruct(maMemoire);
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

    Display_set_colors(monDisplay, 155,155,155,155,52,230);


    struct CPU *monCPU = init_CPU(maMemoire, monDisplay, MyKeyboard, MySpeaker);

    printf("Démarrage de l'émulation...\n");
    
    bool running = true;
    SDL_Event event;

    Uint32 last_tick = SDL_GetTicks();
    double delta_accumulator = 0;
    
    int instructions_par_frame = 10;
    
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
        if (delta_accumulator >= 15) {
            delta_accumulator -= 15;
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
*/