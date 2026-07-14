#include "aux.h"
#include "text.inc"

#define VER "0.7"

#define SUCCESS 0
#define NOT_SUCCESS 1
#define HOW_DID_THAT_HAPPEN -2

void ver(void) {
    printf("mc-utils version %s\n", VER);
    printf("compiled on %s, %s; for %s\n\n", __TIME__, __DATE__);
    puts("by Jimena Neumann <schjmann19@gmail.com>\n");
}

void enchanting_table(void) {
    printf("%s\n", ENCHANTMENT_TABLE_TEXT);
}

void anvil(void) {
    puts("to craft an anvil, you need 3 blocks of iron and 4 iron ingots.");
    puts("for which you need 31 iron ingots\n");
}

void print_usage(void) {
    puts("Usage:");
    printf("%s\n", USAGE_TEXT);
}

void help(void) {
    ver();
    puts("Handy collection of Minecraft-related utilities including number and coords calculation, server status checker, and recipes cheatsheets");
    puts("note: btw i blindly assume that your terminal is at least ~115 characters wide, hope you don't mind\n");
    print_usage();
}

int six_seven(int ret) {
    if (!ret) {
        return HOW_DID_THAT_HAPPEN;
    }
    puts("you flippin twat");
    return ret;
}
