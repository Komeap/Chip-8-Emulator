#ifndef MEMORY_H
#define MEMORY_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct RAM;
struct RAM * init_RAM(void);
int read_RAM(struct RAM *M, uint16_t adresse, uint8_t *out);
int print_RAM(struct RAM *M);
int write_RAM(struct RAM *M, uint16_t adresse, uint8_t val);
int load_RAM(struct RAM *M, const char* filename);
void free_RAM(struct RAM *M);

#endif