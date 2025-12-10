#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "display/display.h"
#include "misc/debug.h"


struct Display;
struct CPU *init_CPU(struct RAM *ram, struct Display *disp);
int cpu_cycle(struct CPU *cpu);

#endif