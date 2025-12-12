#include "memory.h"
#include "processor.h"


#define NB_REGISTER 16

struct CPU{
    uint8_t Vx[NB_REGISTER];
    uint16_t I;
    uint16_t PC;
    uint8_t DT;
    uint8_t ST;
    uint8_t SP;
    struct Display * display;
    struct RAM * ram;
};

struct CPU *init_CPU(struct RAM *ram, struct Display *disp){
    struct CPU *cpu = (struct CPU *)calloc(1, sizeof(struct CPU));
    cpu->ram = ram;
    cpu->display = disp;
    cpu->PC = 0x200;
    return cpu;
}

int cpu_cycle(struct CPU *cpu){
    uint8_t msb, lsb;

    read_RAM(cpu->ram, cpu->PC, &msb);
    read_RAM(cpu->ram, cpu->PC + 1, &lsb);

    uint16_t opcode = (msb << 8) | lsb;

    cpu->PC += 2;

    printf("Exec: %s\n", instruction_as_str(opcode));

    switch (opcode & 0xF000) {
        case 0x00E0:
            Display_CLS(cpu->display);
        case 0x0000:
            break;
            
        case 0x1000:
            cpu->PC = opcode & 0x0FFF;
            break;

        case 0x6000:
            {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t kk = opcode & 0x00FF;
                cpu->Vx[x] = kk;
            }
            break;
            
        case 0xA000:
             cpu->I = opcode & 0x0FFF;
             break;
             
        case 0xD000:
            uint8_t vx_idx = (opcode & 0x0F00) >> 8;
            uint8_t vy_idx = (opcode & 0x00F0) >> 4;
            uint8_t height = opcode & 0x000F;

            uint8_t x_pos = cpu->Vx[vx_idx];
            uint8_t y_pos = cpu->Vx[vy_idx];

            cpu->Vx[0xF] = 0;

            // Indique qu'on va toucher à l'écran
            int dessin_effectue = 0; 

            for (int row = 0; row < height; row++) {
                uint8_t sprite_byte = 0;
                read_RAM(cpu->ram, cpu->I + row, &sprite_byte);

                for (int col = 0; col < 8; col++) {
                    if ((sprite_byte & (0x80 >> col)) != 0) {
                        int screen_x = (x_pos + col) % 64; 
                        int screen_y = (y_pos + row) % 32;

                        if (cpu->display->contents[screen_x][screen_y] == 1) {
                            cpu->display->contents[screen_x][screen_y] = 0;
                            cpu->Vx[0xF] = 1;
                        } else {
                            cpu->display->contents[screen_x][screen_y] = 1;
                        }
                        // On note qu'on a modifié au moins un pixel
                        dessin_effectue = 1;
                    }
                }
            }
            
            // --- AJOUT CRUCIAL ICI ---
            if (dessin_effectue) {
                cpu->display->modified = 1;
            }
            break; 
        }
        return 0;
    }
