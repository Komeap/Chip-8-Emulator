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
    struct Keyboard *keyboard;
    struct Speaker *speaker;
    int key_waiting_release;
};

struct CPU *init_CPU(struct RAM *ram, struct Display *disp, struct Keyboard *keyboard, struct Speaker *speaker){
    struct CPU *cpu = (struct CPU *)calloc(1, sizeof(struct CPU));
    cpu->ram = ram;
    cpu->display = disp;
    cpu->keyboard = keyboard;
    cpu->speaker = speaker;
    cpu->PC = 0x200;
    cpu->key_waiting_release = -1;
    return cpu;
}

int cpu_cycle(struct CPU *cpu){
    uint8_t msb, lsb;

    read_RAM(cpu->ram, cpu->PC, &msb);
    read_RAM(cpu->ram, cpu->PC + 1, &lsb);

    uint16_t opcode = (msb << 8) | lsb;

    cpu->PC += 2;

    //printf("Exec: %s\n", instruction_as_str(opcode));
    
    uint8_t op = (opcode & 0xF000) >> 12; // Le 1er chiffre (Famille d'instruction)
    uint8_t x  = (opcode & 0x0F00) >> 8;  // Le 2ème chiffre (Index registre X)
    uint8_t y  = (opcode & 0x00F0) >> 4;  // Le 3ème chiffre (Index registre Y)
    uint8_t n  =  opcode & 0x000F;        // Le 4ème chiffre (Hauteur pixel ou autre)
    uint8_t nn =  opcode & 0x00FF;        // Les 2 derniers (Valeur 8 bits)
    uint16_t nnn = opcode & 0x0FFF;

    //printf("op :%hhu, x : %hhu, y: %hhu, n: %hhu, nn : %hhu, nnn :%hhu\n", op, x, y, n, nn, nnn);

 switch (op) {
        case 0x0:
            if (opcode == 0x00E0) {
                if(cpu->display) Display_CLS(cpu->display);
            } else if (opcode == 0x00EE) {
                if (cpu->SP == 0) return; // Error handled silently
                cpu->PC = cpu->stack[--cpu->SP];
            }
            break;

        case 0x1: 
            cpu->PC = nnn; 
            break;

        case 0x2:
            if (cpu->SP >= 16){
                return;
            }
            cpu->stack[cpu->SP++] = cpu->PC;
            cpu->PC = nnn;
            break;

        case 0x3: 
            if (cpu->Vx[x] == nn) {
                cpu->PC += 2;
            }
            break;

        case 0x4: 
            if (cpu->Vx[x] != nn) {
                cpu->PC += 2;
            }
            break;
        case 0x5:
            if (cpu->Vx[x] == cpu->Vx[y]){
                cpu->PC += 2;
            } 
            break;

        case 0x6: 
            cpu->Vx[x] = nn; 
            break;

        case 0x7: 
            cpu->Vx[x] += nn; 
            break;

        case 0x8:
            switch (n) {
                case 0x0: 
                    cpu->Vx[x] = cpu->Vx[y]; 
                    break;

                case 0x1: 
                    cpu->Vx[x] |= cpu->Vx[y]; 
                    break;

                case 0x2: 
                    cpu->Vx[x] &= cpu->Vx[y];
                    break;

                case 0x3:
                    cpu->Vx[x] ^= cpu->Vx[y];
                    break;

                case 0x4:
                    uint16_t sum = cpu->Vx[x] + cpu->Vx[y];
                    cpu->Vx[0xF] = (sum > 255);
                    cpu->Vx[x] = sum & 0xFF;
                    break;

                case 0x5:
                    cpu->Vx[0xF] = (cpu->Vx[x] >= cpu->Vx[y]);
                    cpu->Vx[x] -= cpu->Vx[y];
                    break;

                case 0x6:
                    cpu->Vx[0xF] = cpu->Vx[x] & 0x1;
                    cpu->Vx[x] >>= 1;
                    break;

                case 0x7:
                    cpu->Vx[0xF] = (cpu->Vx[y] >= cpu->Vx[x]);
                    cpu->Vx[x] = cpu->Vx[y] - cpu->Vx[x];
                    break;

                case 0xE:
                    cpu->Vx[0xF] = (cpu->Vx[x] >> 7) & 0x1;
                    cpu->Vx[x] <<= 1;
                    break;
            }
            break;

        case 0x9: 
            if (cpu->Vx[x] != cpu->Vx[y]) {
                cpu->PC += 2;
            }
            break;

        case 0xA: 
            cpu->I = nnn;
            break;

        case 0xB: 
            cpu->PC = nnn + cpu->Vx[0];
            break;
        case 0xC:
            cpu->Vx[x] = (rand() % 256) & nn;
            break;

        case 0xD: {
            struct Sprite spr;
            if (Sprite_init(&spr, n) != 0) break;
            for (uint8_t i = 0; i < n; i++) {
                uint8_t b = 0;
                read_RAM(cpu->ram, cpu->I + i, &b);
                Sprite_add(&spr, b);
            }
            uint8_t vf = 0;
            if (cpu->display) {
                Display_DRW(cpu->display, &spr, cpu->Vx[x], cpu->Vx[y], &vf);
            }
            cpu->Vx[0xF] = vf;
            Sprite_destroy(&spr);
        } break;

        case 0xE: {
            int state = KEY_UP;

            Keyboard_get(cpu->keyboard, cpu->Vx[x] & 0xF, &state);
            
            if (nn == 0x9E) { // SKP
                if (state == KEY_DOWN) cpu->PC += 2;
            } else if (nn == 0xA1) { // SKNP
                if (state == KEY_UP) cpu->PC += 2;
            }
        } break;

        case 0xF:
            switch (nn) {
                case 0x07: 
                    cpu->Vx[x] = cpu->DT;
                    break;

                case 0x0A:
                if (cpu->key_waiting_release == -1) {
                    int key_found = -1;
                    for (uint8_t i = 0; i < 16; i++) {
                        int state = KEY_UP;
                        Keyboard_get(cpu->keyboard, i, &state);
                        if (state == KEY_DOWN) {
                            key_found = i;
                            break;
                        }
                    }

                    if (key_found != -1) {
                        cpu->key_waiting_release = key_found;
                    }
                    cpu->PC -= 2;
                }
                else {
                    int state = KEY_UP;
                    Keyboard_get(cpu->keyboard, (uint8_t)cpu->key_waiting_release, &state);

                    if (state == KEY_UP) {
                        cpu->Vx[x] = (uint8_t)cpu->key_waiting_release;
                        cpu->key_waiting_release = -1;
                    } else {
                        cpu->PC -= 2;
                    }
                }
                break;

                case 0x15: 
                    cpu->DT = cpu->Vx[x];
                    break;

                case 0x18: 
                    cpu->ST = cpu->Vx[x];
                    break;

                case 0x1E:
                    cpu->I += cpu->Vx[x];
                    break;

                case 0x29:
                    cpu->I = (cpu->Vx[x] & 0xF) * 5;
                    break;

                case 0x33:
                    write_RAM(cpu->ram, cpu->I,     cpu->Vx[x] / 100);
                    write_RAM(cpu->ram, cpu->I + 1, (cpu->Vx[x] / 10) % 10);
                    write_RAM(cpu->ram, cpu->I + 2, cpu->Vx[x] % 10);
                    break;

                case 0x55:
                    for (int i = 0; i <= x; i++){
                        write_RAM(cpu->ram, cpu->I + i, cpu->Vx[i]);
                    } 
                    break;

                case 0x65:
                    for (int i = 0; i <= x; i++){
                        read_RAM(cpu->ram, cpu->I + i, &cpu->Vx[i]);
                    }
                    break;
            }
            break;
    }
}

void cpu_update_timers(struct CPU *cpu) {
    if (cpu->DT > 0) {
        cpu->DT--;
    }

    if (cpu->ST > 0) {
        cpu->ST--;
        Speaker_on(cpu->speaker);
    } else {
        Speaker_off(cpu->speaker);
    }
}