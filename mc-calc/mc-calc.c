#include "aux.c"
// not yet #include "craft.c"
#include <stdlib.h>
#include <string.h>

#define STACK 64
#define SMALL_STACK 16

void calculate_stacks(
    int total_items, int stack_size) {
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
    nl;
}

int calculate_total(
    int num_stacks, int remaining_items, int stack_size) {
    return (num_stacks * stack_size) + remaining_items;
}

int main(int argc, char *argv[]) 
{
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
            printf("Error: must specify at least --stacks or --items with values\n");
            return 1;
        }

        int total = calculate_total(num_stacks, remaining_items, stack_size);
        printf("%d.\n", total);
    }
    else {
        printf("Unknown option: %s\n", argv[1]); print_usage();
        return 1;
    }
    return 0;
}

/* remember, never trust the government. */