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
             break;

        default:
            printf("Instruction inconnue : %04X\n", opcode);
            break;
    }
    return 0;
}

