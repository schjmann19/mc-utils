#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define BEACON_REQ_SCRAPS 5904

#define NETHERITE_LEFT_VER "0.2"

#define _ -1

static void NL_bad_arg(void) {
    fprintf(stderr, "bad argument use -h or --help for help\n");
    exit(1);
}

static void NL_ver(void) {
    printf("netherite-left version %s\n", NETHERITE_LEFT_VER);
    puts("by Jimena Neumann (schjmann19@gmail.com)");
}

static void NL_hint(void) {
    puts("this is also applicable to gold for the beacon, since it's the same amount as scraps.");
}

static void NL_help(void) {
    NL_ver();
    puts("calculates how much more netherite you need to get a beacon.");
    puts("input how many scraps, ingots, and blocks you have.");
    puts("output is in scraps.");
    NL_hint();
    puts("Usage:");
    puts("  netherite-left [options]");
    puts("Options:");
    puts("  -s, --scraps <N>     Number of scraps (integer)");
    puts("  -i, --ingots <N>     Number of ingots (integer)");
    puts("  -b, --blocks <N>     Number of blocks (integer)");
    puts("  -h, --help           Show this help and exit");
    puts("  -v, --version        Show version and exit");
    puts("  -g, --gold           Show hint for gold and exit");
}

int netherite_left(int argc, char **argv) { /* calculate in scraps, the smallest unit (gold is equivalent); accept options */
    int scraps = _;
    int ingots = _;
    int blocks = _;

    static struct option longopts[] = {
        {"scraps", required_argument, NULL, 's'},
        {"ingots", required_argument, NULL, 'i'},
        {"blocks", required_argument, NULL, 'b'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"gold", no_argument, NULL, 'g'},
        {0, 0, 0, 0}};

    /* short options: s,i,b require arguments; h,v,g are flags (no argument) */
    int opt;
    while ((opt = getopt_long(argc, argv, "s:i:b:hvg", longopts, NULL)) != _) {
        switch (opt) {
        case 's':
            scraps = atoi(optarg);
            break;
        case 'i':
            ingots = atoi(optarg);
            break;
        case 'b':
            blocks = atoi(optarg);
            break;
        case 'h':
            NL_help();
            return EXIT_SUCCESS;
        case 'v':
            NL_ver();
            return EXIT_SUCCESS;
        case 'g':
            NL_hint();
            return EXIT_SUCCESS;
        default:
            NL_bad_arg();
            return EXIT_FAILURE;
        }
    }

    /* If none of the counts were provided on the command line, prompt interactively */
    if (scraps < 0 && ingots < 0 && blocks < 0) {
        if (scraps < 0) {
            printf("scraps? ");
            if (scanf("%d", &scraps) != 1) {
                scraps = 0;
            }
        }
        if (ingots < 0) {
            printf("ingots? ");
            if (scanf("%d", &ingots) != 1) {
                ingots = 0;
            }
        }
        if (blocks < 0) {
            printf("blocks? ");
            if (scanf("%d", &blocks) != 1) {
                blocks = 0;
            }
        }
    } else {
        /* any missing count defaults to zero when using CLI flags */
        if (scraps < 0) {
            scraps = 0;
        }
        if (ingots < 0) {
            ingots = 0;
        }
        if (blocks < 0) {
            blocks = 0;
        }
    }

    int total_scraps = scraps + (ingots * 4) + (blocks * 36);
    int scraps_needed = BEACON_REQ_SCRAPS - total_scraps;

    if (scraps_needed <= 0) {
        // printf("have enough, add 1 to the counter\n");
        puts("u have enough");
    } else {
        printf("u need %d more netherite scraps.\n", scraps_needed);
        printf("%d/%d.\n", total_scraps, BEACON_REQ_SCRAPS);
    }
    return EXIT_SUCCESS;
}