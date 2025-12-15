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
    uint16_t stack[16];
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
    
    uint8_t op = (opcode & 0xF000) >> 12; // Le 1er chiffre (Famille d'instruction)
    uint8_t x  = (opcode & 0x0F00) >> 8;  // Le 2ème chiffre (Index registre X)
    uint8_t y  = (opcode & 0x00F0) >> 4;  // Le 3ème chiffre (Index registre Y)
    uint8_t n  =  opcode & 0x000F;        // Le 4ème chiffre (Hauteur pixel ou autre)
    uint8_t nn =  opcode & 0x00FF;        // Les 2 derniers (Valeur 8 bits)
    uint16_t nnn = opcode & 0x0FFF;

    printf("op :%hhu, x : %hhu, y: %hhu, n: %hhu, nn : %hhu, nnn :%hhu\n", op, x, y, n, nn, nnn);

    switch (op){
        case 0x0:
            if (opcode == 0x00E0 && cpu->display){
                (void)Display_CLS(cpu->display);
            }else if(opcode == 0x00EE){
                if (cpu->SP == 0){
                    fprintf(stderr, "Stack underflow at RET!\n");
                    exit(1);
                }
                cpu->PC = cpu->stack[--cpu->SP];
            }
            break;

        case 0x1:
            cpu->PC = nnn;
            break;
            
        case 0x2:
            if (cpu->SP >= 16){
                fprintf(stderr, "Stack overflow at CALL!\n");
                exit(1);
            }
            cpu->stack[cpu->SP++] = cpu->PC;
            cpu->PC = nnn;
            break;

        case 0x3:
            if (cpu->Vx[x] == nn) {
                cpu->PC += 2;
            }
            break;

        case 0x6:
            cpu->Vx[x] = nn;
            break;
        
        case 0x7:
            cpu->Vx[x] += nn;
            break;
        
        case 0xA:
            cpu->I = nnn;
            break;

        case 0xD: {
            struct Sprite spr;
            if (Sprite_init(&spr, n) != 0) break;
            for (uint8_t i =0; i<n; i++){
                uint8_t b = 0;
                read_RAM(cpu->ram, (uint16_t)(cpu->I +i), &b);
                Sprite_add(&spr, b);
            }
            uint8_t VF = 0;
            if (cpu->display){
                (void)Display_DRW(cpu->display, &spr, cpu->Vx[x], cpu->Vx[y], &VF);
            }
            cpu->Vx[0xF] = VF ? 1 :0;
            break;
        }
        default:
            break;
    }
    return 0;
}
