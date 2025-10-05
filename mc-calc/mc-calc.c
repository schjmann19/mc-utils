#include "aux.c"

#define STACK 64
#define SMALL_STACK 16

void calculate_stacks(int total_items, int stack_size) {
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

int main(int argc, char *argv[]) 
{
    if (argc < 2) { help(); return 1; }

    // Check for version and help flags first
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) { ver(); return 0; }
    
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) { help(); return 0; }

    if (strcmp(argv[1], "--goal") == 0) {
        if (argc < 3) { printf("Error: --goal requires a number\n"); return 1; }

        int goal = atoi(argv[2]);
        if (goal <= 0) { printf("Please provide a positive number\n"); return 1; }
        
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
    else {
        printf("Unknown option: %s\n", argv[1]);
        print_usage();
        return 1;
    }

    return 0;
}