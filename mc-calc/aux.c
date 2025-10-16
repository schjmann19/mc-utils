#include <stdio.h>

#define VER 0.2

void ver(void){
    printf("mc-calc version %.1f\n", VER);
    printf("by Jimena Neumann (schjmann19@gmail.com)\n\n");
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

void print_usage(void){
    printf("Usage:\n");
    printf("  mc-calc --goal <number> [--small]    Calculate stacks for given number\n");
    printf("  mc-calc --enchant                    Show enchanting table requirements\n");
    printf("  mc-calc --anvil                      Show anvil crafting requirements\n");
    printf("  mc-calc -v, --version                Show version information\n");
    printf("  mc-calc -h, --help                   Show this help message\n");
    printf("  mc-calc -t, --total                  Calculate total items from stacks and items\n");
    printf("    Options for --total:\n");
    printf("      -s, --stacks <number>            Number of full stacks\n");
    printf("      -i, --items <number>             Additional individual items\n");
    printf("      --stacks-small                   Use small stacks (16 items)\n\n");
}   /* i want to add --craft or --recipe but its just thousands of printf's, i'll get around to doing it though */
        // about a week later (from starting to implement it) i'm still doing it

void help(void){
    ver();
    printf("Handy item quantity and crafting recipes calculator and cheatsheet\n\n");
    printf("note: btw i blindly assume that your terminal is at least 115 characters wide, hope you don't mind");
    print_usage();
}

#define nl printf("\n");

