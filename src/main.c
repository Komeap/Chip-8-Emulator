#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_ttf.h> /// Pour la police d'éeciture, install linux : sudo apt-get install libsdl2-ttf-dev
#include <time.h>

#include "../include/display/display.h"
#include "../include/memory.h"
#include "misc/debug.h"
#include "../include/processor.h"
#include "../include/keyboard/keyboard.h"

/// @brief Liste des liens des ROMS/jeux chip8
const char *ROM_LIST[] = {"chip8_load/1-chip8-logo.ch8", "chip8_load/2-ibm-logo.ch8", "chip8_load/3-corax+.ch8", "chip8_load/4-flags.ch8", "chip8_load/5-quirks.ch8",
                           "chip8_load/6-keypad.ch8", "chip8_load/7-beep.ch8", "chip8_load/BLINKY", "chip8_load/TETRIS", "chip8_load/PONG2", "chip8_load/TICTAC",
                        "chip8_load/BLITZ", "chip8_load/BRIX", "chip8_load/CONNECT4", "chip8_load/GUESS", "chip8_load/HIDDEN", "chip8_load/INVADERS", "chip8_load/KALEID",
                        "chip8_load/MAZE", "chip8_load/MERLIN", "chip8_load/MISSILE", "chip8_load/PUZZLE", "chip8_load/SYZYGY", "chip8_load/TANK", "chip8_load/UFO", "chip8_load/VBRIX",
                        "chip8_load/VERS", "chip8_load/WIPEOFF"};

/// @brief  nombre de Rom dans la ROM_LIST[]
const int ROM_COUNT = sizeof(ROM_LIST) / sizeof(ROM_LIST[0]);

void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    /**
     * @brief Fonction qui permet d'afficher du texte a l'écran avec SDL
     * @param renderer L'endroit ou ca affiche
     * @param font police décriture
     * @param text text a écrire
     * @param x position x
     * @param y position y
     * @param color Couleur du texte
     */
    
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

/// @brief Gere le menu d'acceuil pour lancer les rom
int run_menu() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){ /// check init
        return -1;
    }
    TTF_Init();

    /// Créer la page d'acceuil (MENU), et le renderer(calque)
    SDL_Window *win = SDL_CreateWindow("CHIP-8 MENU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font *font = TTF_OpenFont("Limited.ttf", 46); /// init de la police d ecriture, (Fichier ttf a la racine du projet)

    /// Variables pour la gestion du menu
    int selection = 0;
    bool in_menu = true;
    int result = -1;
    SDL_Event event = {0};

    /// gestion du nombre d'item par colonne pour que ca reste dans la page
    int items_per_col = (ROM_COUNT + 1) / 2;

    /// boucle de gestion du Menu
    while (in_menu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { // SI on fait echap ou la croix
                result = -1;
                in_menu = false;
            }
            if (event.type == SDL_KEYDOWN) { // Si on clique sur une touche
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: 
                        result = -1;
                        in_menu = false;
                        break;

                    case SDLK_UP: // Si c'est la fléche up -> On change la selection
                        selection--;
                        if (selection < 0){ 
                            selection = ROM_COUNT - 1;
                        }
                        break;

                    case SDLK_DOWN:  // Si c'est la fléche down -> On change la selection
                        selection++;
                        if (selection >= ROM_COUNT){
                            selection = 0;
                        }
                        break;

                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:  // Si c'est la enter -> On stop la boucle -> charge la rom
                        result = selection;
                        in_menu = false;
                        break;
                }
            }
        }

        /// Affichage du Menu
        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        SDL_Color colTitre = {255, 200, 0, 255};
        draw_text(ren, font, "CHIP-8 EMULATOR", 200, 20, colTitre); /// ecriture

        /// Boucle qui affiche les séléctions des ROMS
        for (int i = 0; i < ROM_COUNT; i++) {
            SDL_Color colText = {255, 255, 255, 255};
            char buffer[128];
            
            if (i == selection) {
                colText.g = 0;
                colText.b = 0; // change la couleur pour celui select
                sprintf(buffer, "> %s", ROM_LIST[i]); // Affiche une petit fléche ">" devant celui qui est select
            } else {
                sprintf(buffer, "  %s", ROM_LIST[i]);
            }

            int col, row;

            // Si on n'a pas encore commencer la deuxieme col
            if (i < items_per_col) {
                col = 0;
                row = i;
            } 
            else {
                col = 1;
                row = i - items_per_col;
            }

            /// Calcul des postion
            int x_pos = 20 + (col * 320);
            int y_pos = 60 + (row * 28);

            draw_text(ren, font, buffer, x_pos, y_pos, colText); /// Affichage
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    /// Fermeture propre
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    
    return result;
}

/// @brief Fonction qui lance une ROM
/// @param rom_path chemir d'accés de la rom (return de run_menu)
/// @param monDisplay DIsplay
/// @param MyKeyboard Keyboard
/// @param MySpeaker Speaker
bool run_game(const char *rom_path, struct Display *monDisplay, struct Keyboard *MyKeyboard, struct Speaker *MySpeaker) {
    struct RAM *maMemoire = init_RAM(); /// init ram
    if (load_RAM(maMemoire, rom_path) == 0) { // Charge la memoire + verification
        fprintf(stderr, "Erreur de  chargement ROM\n");
        free_RAM(maMemoire); 
        return true;
    }


    if (Display_init(monDisplay, 10) != 0) { /// Init display + check
        free_RAM(maMemoire);
        return false;
    }
    
    Display_set_colors(monDisplay, 200, 50, 155, 50, 50, 50); /// Setting couleur

    struct CPU *monCPU = init_CPU(maMemoire, monDisplay, MyKeyboard, MySpeaker); /// Init du CPU/processeur
    
    bool running = true;
    /// Variable de gestion du temps, et des frames
    SDL_Event event = {0};
    Uint32 last_tick = SDL_GetTicks();
    double delta_accumulator = 0;
    int instructions_par_frame = 20; /// vitesse du jeu
    
    while (running) {
        /// Gestion des ticks, pour le timer, frames et halting
        Uint32 current_tick = SDL_GetTicks();
        double dt = (current_tick - last_tick);
        last_tick = current_tick;
        delta_accumulator += dt;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { /// fermeture propre au cas ou
                free_CPU(monCPU);
                free_RAM(maMemoire);
                Display_destroy(monDisplay);
                return false;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) { /// Pour quitter le jeu avec echap
                running = false;
            }
        }

        /// Gestion des frame et de la vitesse de jeu 15ms == 66Hz
        while (delta_accumulator >= 15) {
            delta_accumulator -= 15;
            for (int i = 0; i < instructions_par_frame; i++) {
                int result = cpu_cycle(monCPU); /// Décodage de opcode
                if (result == 1) break;
            }
            cpu_update_timers(monCPU); /// Fonction qui gére le temps
        }
        Display_update(monDisplay);/// Affichage
        SDL_Delay(1);
    }

    /// Fermeture propre
    Display_destroy(monDisplay);
    free_CPU(monCPU);
    free_RAM(maMemoire);

    return true;
}

int main() {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    /// init
    struct Display *monDisplay = (struct Display*)calloc(1, sizeof(struct Display));
    struct Keyboard *MyKeyboard = (struct Keyboard*)calloc(1, sizeof(struct Keyboard));
    struct Speaker *MySpeaker = (struct Speaker*)calloc(1, sizeof(struct Speaker));
    
    Keyboard_init(MyKeyboard);
    Speaker_init(MySpeaker);

    bool app_running = true;

    while (app_running) {
        int choix = run_menu(); // LAnce le menu en premier

        if (choix == -1) { // SI on a un choix de rom dans le menu
            app_running = false;
        } else {
            // Lance une ROM
            Speaker_init(MySpeaker);
            bool back = run_game(ROM_LIST[choix], monDisplay, MyKeyboard, MySpeaker);
            Speaker_destroy(MySpeaker);
            if (!back) app_running = false;
        }
    }

    // Fermeture Propre
    free(monDisplay);

    Keyboard_destroy(MyKeyboard);
    // Speaker déjà détruit dans la boucle
    free(MyKeyboard);
    free(MySpeaker);
    SDL_Quit();

    return 0;
}
