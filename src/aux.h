#ifndef MC_UTILS_AUX_H
#define MC_UTILS_AUX_H

#include <stdio.h>

typedef struct Coordinate {
    int x;
    int y;
    int z;
} coord;

/* basic helpers used across the program */
void ver(void);
void enchanting_table(void);
void anvil(void);
void print_usage(void);
void help(void);
int six_seven(int ret);

#endif /* MC_UTILS_AUX_H */
