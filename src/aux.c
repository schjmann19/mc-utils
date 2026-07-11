#include "aux.h"

#define VER "0.6"

#define SUCCESS 0
#define NOT_SUCCESS 1
#define HOW_DID_THAT_HAPPEN -2

void ver(void) {
    printf("mc-utils version %s\n", VER);
    puts("by Jimena Neumann (schjmann19@gmail.com)\n");
    printf("compiled on %s, %s\n\n", __TIME__, __DATE__);
}

void enchanting_table(void) {
    puts("For level 15, you need 45 books.");
    puts("for which you need 45 leather and 135 paper.");
    puts("for which you need 2 stacks and 7 sugar cane.");
    puts("also, 1 stack and 26 planks");
    puts("or 23 logs.\n");
}

void anvil(void) {
    puts("to craft an anvil, you need 3 blocks of iron and 4 iron ingots.");
    puts("for which you need 31 iron ingots\n");
}

void print_usage(void) {
    puts("Usage:");
    puts("  mc-util --goal <number> [--small]    Calculate stacks for given number");
    puts("  mc-util --enchant                    Show enchanting table requirements");
    puts("  mc-util --anvil                      Show anvil crafting requirements");
    puts("  mc-util -v, --version                Show version information");
    puts("  mc-util -h, --help                   Show this help message");
    puts("  mc-util -t, --total                  Calculate total items from stacks and items");
    puts("    Options for --total:");
    puts("      -s, --stacks <number>            Number of full stacks");
    puts("      -i, --items <number>             Additional individual items");
    puts("      --stacks-small                   Use small stacks (16 items)\n");
    puts("  mc-util --download                   Download latest Minecraft JAR to ./minecraftjar/");
    puts("  mc-util --recipe <item>              Show crafting recipe for <item> (uses extractor/recipes.json)");
    puts("    Options for --recipe:");
    puts("      --recipes=<path>                  Use custom recipes JSON file (default: extractor/recipes.json)");
    puts("      --netherite                       Use netherite in crafting recipes");
    puts("  mc-util --generate-list              List all craftable item names (one per line, for fuzzy finding)");
    puts("    Options for --generate-list:");
    puts("      --recipes=<path>                  Use custom recipes JSON file (default: extractor/recipes.json)");
}

void help(void) {
    ver();
    puts("Handy item quantity and crafting recipes calculator and cheatsheet");
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
