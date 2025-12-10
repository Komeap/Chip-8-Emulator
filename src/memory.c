#include "../include/display/display.h"
#include "../include/memory.h"
#include "misc/debug.h"

#define RAM_SIZE 4096

/// @brief Création de la ram
struct RAM{
    uint8_t *memory;
};

/**
 * @brief initialise le tableau memory de la RAM avec des 0
 */

struct RAM * init_RAM(void){
    struct RAM *M = (struct RAM *)malloc(sizeof(struct RAM));
    M->memory = (uint8_t*)calloc(RAM_SIZE, sizeof(uint8_t));
    return M;
}
/**
 * @brief Lire la ram, dans out
 * @param M la RAM
 * @param adresse l'en,droit on on veut read
 * @param out le pointeur ou on stock la val lue
 */

int read_RAM(struct RAM *M, uint16_t adresse, uint8_t *out){
    if(adresse < RAM_SIZE){
        *out = M->memory[adresse];
        return 0;
    }    
    else{
        return 1;
    }
}

int print_RAM(struct RAM *M){
    printf("Je vais print la RAM");
    for(int i = 512; i < RAM_SIZE; i++){
        printf("%04X ", M->memory[i]);
        if (i%16 == 0) {
            printf("  %d %02x \n", i, i);
        }
    }
    return 1;
}

/**
 * @brief Ecrire dans la ram la val
 * @param M la RAM
 * @param adresse l'en,droit on on veut ecrire
 * @param val la val a ecrire
 */
int write_RAM(struct RAM *M, uint16_t adresse, uint8_t val){
    if(adresse < RAM_SIZE){
        M->memory[adresse] = val;
        return 0;
    }
    else{
        return 1;
    }
    
}

/**
 * @breif chargé un code chip-8
 * @param M pointeur vers la RAM
 * @param filename fichier contenant le code .ch8
 */

int load_RAM(struct RAM *M, const char* filename){
    FILE *f = fopen(filename, "rb");
    if (f == NULL){
        printf("Mon paf est pas bon\n");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f); 
    rewind(f);

    //printf("Taille du fichier: %ld octets\n", file_size);

    if (file_size > (RAM_SIZE - 0x200)) {
        fprintf(stderr, "Erreur: La ROM est trop grande pour la mémoire du CHIP-8 !\n");
        fclose(f);
        return 0;
    }

    size_t result = fread(&M->memory[0x200], 1, file_size, f);

    if ((long)result != file_size ){
        return 0;
    }

    fclose(f);
    return 1;
}


void print_instruct(struct RAM *M) {
    for (int i = 0x200; i < 0x200 + 50; i += 2) { 
        
        uint8_t msb = M->memory[i];
        uint8_t lsb = M->memory[i+1];   
        uint16_t opcode = (msb << 8) | lsb;

        const char *description = instruction_as_str(opcode);
        printf("Addr 0x%03X : %04X  ->  %s\n", i, opcode, description);
    }
}


