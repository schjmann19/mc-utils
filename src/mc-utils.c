#include "aux.c"
#include "netherite-left.c"
#include "recipes.c"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define STACK 64
#define SMALL_STACK 16


void calculate_stacks(
    int total_items,
    int stack_size)
{
    int stacks = total_items / stack_size;
    int remainder = total_items % stack_size;

    if (stacks > 0) {
        printf("%d stack%s", stacks, stacks > 1 ? "s" : "");
        if (remainder > 0) {
            printf(" and %d item%s", remainder, remainder > 1 ? "s" : "");
        }
    } else {
        printf("%d item%s", remainder, remainder > 1 ? "s" : "");
    }
    printf("\n");
}

int calculate_total(
    int num_stacks, int remaining_items, int stack_size) {
    return (num_stacks * stack_size) + remaining_items;
}

int main(int argc, char *argv[])
{
    /* Allow overriding recipes file: remove any --recipes=... or --recipes <path> args early */
    const char *recipes_path = "extractor/recipes.json";
    char **nargv = malloc(sizeof(char*) * (argc + 1));
    if (!nargv) { fprintf(stderr, "Out of memory\n"); return 1; }
    nargv[0] = argv[0];
    int j = 1;
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--recipes=", 10) == 0) {
            recipes_path = argv[i] + 10;
        } else if (strcmp(argv[i], "--recipes") == 0) {
            if (i + 1 < argc) { recipes_path = argv[i+1]; ++i; }
        } else {
            nargv[j++] = argv[i];
        }
    }
    int narrc = j;
    nargv[narrc] = NULL; // ensure terminated
    argv = nargv; argc = narrc;

    /* Support calling the netherite tool via CLI flags rather than argv[0]. */
    if (argc > 1 && (strcmp(argv[1], "-nl") == 0 || strcmp(argv[1], "--netherite") == 0 || strcmp(argv[1], "--netherite-left") == 0)) {
        /* Build a new argv for the netherite_left() call that strips the wrapper flag */
        int newargc = argc - 1;
        char **newargv = malloc(sizeof(char*) * (newargc + 1));
        if (!newargv) { fprintf(stderr, "Out of memory\n"); return 1; }
        newargv[0] = "netherite-left";
        for (int i = 2; i < argc; ++i) newargv[i - 1] = argv[i];
        newargv[newargc] = NULL;
        int ret = netherite_left(newargc, newargv);
        free(newargv);
        return ret;
    }

    if (argc < 2) { help(); return 1; }

    // check for version and help flags first
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) { ver(); return 0; }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) { help(); return 0; }

    if (strcmp(argv[1], "--goal") == 0) {
        if (argc < 3) { fprintf(stderr, "Error: --goal requires a number\n"); return 1; }

        int goal = atoi(argv[2]);
        if (goal <= 0) { fprintf(stderr, "Please provide a positive number\n"); return 1; }

        int stack_size = STACK;  // default to normal stack size
        if (argc > 3 && strcmp(argv[3], "--small") == 0) { stack_size = SMALL_STACK; } // or use small stacks

        calculate_stacks(goal, stack_size);
    }
    else if (strcmp(argv[1], "--enchant") == 0) {
        enchanting_table();
    }
    else if (strcmp(argv[1], "--anvil") == 0) {
        anvil();
    }
    else if (strcmp(argv[1], "--recipe") == 0) {
        if (argc < 3) { fprintf(stderr, "Error: --recipe requires an item name\n"); return 1; }
        print_recipe(argv[2], recipes_path);
    }
    else if (strcmp(argv[1], "--total") == 0 || strcmp(argv[1], "-t") == 0) {
        int stack_size = STACK;
        int num_stacks = 0;
        int remaining_items = 0;
        int i = 2;

        while (i < argc) {
            if (strcmp(argv[i], "--stacks") == 0 || strcmp(argv[i], "-s") == 0) {
                if (i + 1 >= argc) { fprintf(stderr, "Error: --stacks requires a number\n"); return 1; }
                num_stacks = atoi(argv[i + 1]);
                i += 2;
            }
            else if (strcmp(argv[i], "--items") == 0 || strcmp(argv[i], "-i") == 0) {
                if (i + 1 >= argc) { fprintf(stderr, "Error: --items requires a number\n"); return 1; }
                remaining_items = atoi(argv[i + 1]);
                i += 2;
            }
            else if (strcmp(argv[i], "--stacks-small") == 0) {
                stack_size = SMALL_STACK;
                i++;
            }
            else {
                fprintf(stderr, "Unknown option: %s\n", argv[i]);
                print_usage();
                return 1;
            }
        }

        if (num_stacks == 0 && remaining_items == 0) {
            fprintf(stderr, "Error: must specify at least --stacks or --items with values\n");
            return 1;
        }

        int total = calculate_total(num_stacks, remaining_items, stack_size);
        printf("%d.\n", total);
    }
    else {
        fprintf(stderr, "Unknown option: %s\n", argv[1]); print_usage();
        return 1;
    }
    return 0;
}

/* remember, never trust the government. */
