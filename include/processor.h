#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "display/display.h"
#include "misc/debug.h"
#include "keyboard/keyboard.h"
#include "speaker/speaker.h"


struct Display;
struct CPU *init_CPU(struct RAM *ram, struct Display *disp, struct Keyboard *keyboard, struct Speaker *speaker);
int cpu_cycle(struct CPU *cpu);
void cpu_update_timers(struct CPU *cpu);
void free_CPU(struct CPU *cpu);

#endif