#include <stdio.h>

#pragma once

#define VER 0.3

#define SUCCESS 0
#define NOT_SUCCESS 1
#define HOW_DID_THAT_HAPPEN -2

void ver(void){
    printf("mc-utils version %.1f\n", VER);
    puts("by Jimena Neumann (schjmann19@gmail.com)\n");
}

void enchanting_table(void){
    puts("For level 15, you need 45 books.");
    puts("for which you need 45 leather and 135 paper.");
    puts("for which you need 2 stacks and 7 sugar cane.");
    puts("also, 1 stack and 26 planks");
    puts("or 23 logs.\n");
}

void anvil(void){
    puts("to craft an anvil, you need 3 blocks of iron and 4 iron ingots.");
    puts("for which you need 31 iron ingots\n");
}

void print_usage(void){
    puts("Usage:");
    puts("  mc-calc --goal <number> [--small]    Calculate stacks for given number");
    puts("  mc-calc --enchant                    Show enchanting table requirements");
    puts("  mc-calc --anvil                      Show anvil crafting requirements");
    puts("  mc-calc -v, --version                Show version information");
    puts("  mc-calc -h, --help                   Show this help message");
    puts("  mc-calc -t, --total                  Calculate total items from stacks and items");
    puts("    Options for --total:");
    puts("      -s, --stacks <number>            Number of full stacks");
    puts("      -i, --items <number>             Additional individual items");
    puts("      --stacks-small                   Use small stacks (16 items)\n");
}   /* i want to add --craft or --recipe but its just thousands of puts's, i'll get around to doing it though */
        // about a week later (from starting to implement it) i'm still doing it
        // 4 months ago (now: Jan 25, 2026) still not implemented.

void help(void){
    ver();
    puts("Handy item quantity and crafting recipes calculator and cheatsheet");
    puts("note: btw i blindly assume that your terminal is at least ~115 characters wide, hope you don't mind\n");
    print_usage();
}

int six_seven(int ret){
    if (!ret) { return HOW_DID_THAT_HAPPEN; }
    puts("you flippin twat");
    return ret;
}


