#ifndef MEMORY_H
#define MEMORY_H
#include <stdlib.h>
#include <stdio.h>


/// @brief Création de la ram
struct RAM{
    uint8_t *memory;
};

/**
 * @brief initialise le tableau memory de la RAM avec des 0
 */

struct RAM * init_RAM(void){
    struct RAM *M = (struct RAM *)malloc(sizeof(struct RAM));
    M->memory = (uint8_t*)calloc(4096, sizeof(uint8_t));
    return M;
}
/**
 * @brief Lire la ram, dans out
 * @param M la RAM
 * @param adresse l'en,droit on on veut read
 * @param out le pointeur ou on stock la val lue
 */
int read_RAM(struct RAM *M, int adresse, uint8_t *out){
    *out = M->memory[adresse];
    return 0;
}
/**
 * @brief Ecrire dans la ram la val
 * @param M la RAM
 * @param adresse l'en,droit on on veut ecrire
 * @param val la val a ecrire
 */
int write_RAM(struct RAM *M, int adresse, uint8_t val){
    M->memory[adresse] = val;
    return 0;
}

/**
 * @breif chargé un code chip-8
 * @param M pointeur vers la RAM
 * @param filename fichier contenant le code .ch8
 */
void load_RAM(struct RAM *M, const char* filename){
    FILE *f = fopen(filename, "rb");
    
    
}

#endif