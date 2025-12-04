#include "../include/display/display.h"
#include "../include/memory.h"

int main(void) {
    struct RAM *maMemoire = init_RAM();
    uint8_t *test;
    test = malloc(sizeof(uint8_t));
    write_RAM(maMemoire, 255, 12);
    read_RAM(maMemoire, 255, test);
    printf("%u\n", *test);
    
}

