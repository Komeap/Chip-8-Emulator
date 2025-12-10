#include "../include/display/display.h"
#include "../include/memory.h"
#include "misc/debug.h"
#include "../include/processor.h"



int main(void) {
    struct RAM *maMemoire = init_RAM();

    printf("%d\n", load_RAM(maMemoire, "/home/julien/Cours/Chip-8_Project/1-chip8-logo.ch8"));
    
    //print_RAM(maMemoire);

    //printf("%s\n",instruction_as_str((uint16_t)0xFF));
    //print_instruct(maMemoire);
    struct Display *monDisplay = (struct Display*)calloc(1, sizeof(struct Display));
    Display_init(monDisplay, 10);
    struct CPU *monCPU = init_CPU(maMemoire, monDisplay);
    cpu_cycle(monCPU);
}
