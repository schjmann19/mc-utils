#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VER 0.1

void ver(void){
    printf("mc-calc version %.1f\n", VER);
    printf("by Jimena Neumann\n\n");
}

void enchanting_table(void){
    printf("For level 15, you need 45 books.\n");
    printf("for which you need 45 leather and 135 paper.\n");
    printf("for which you need 2 stacks and 7 sugar cane.\n");
    printf("also, 1 stack and 26 planks\n");
    printf("or 23 logs.\n");
}

void anvil(void){
    printf("to craft an anvil, you need 3 blocks of iron and 4 iron ingots.\n");
    printf("for which you need 31 iron ingots\n");
}

void print_usage(void) {
    printf("Usage:\n");
    printf("  mc-calc --goal <number> [--small]  Calculate stacks for given number\n");
    printf("  mc-calc --enchant                  Show enchanting table requirements\n");
    printf("  mc-calc --anvil                    Show anvil crafting requirements\n");
    printf("  mc-calc -v, --version              Show version information\n");
    printf("  mc-calc -h, --help                 Show this help message\n\n");
}

void help(void){
    ver();
    printf("Handy item quantity and crafting recipes calculator and cheatsheet\n\n");
    print_usage();
}

#define nl; printf("\n");
