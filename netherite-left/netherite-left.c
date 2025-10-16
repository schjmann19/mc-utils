#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define BEACON_REQ_SCRAPS 5904

#define VER 0.1

static void bad_arg(void){
    fprintf(stderr, "bad argument\n use -h or --help for help\n");
    exit(1);
}

static void ver(void){
    printf("netherite-left version %.1f\n", VER);
    printf("by Jimena Neumann (schjmann19@gmail.com)\n");
}

static void hint(void){
    printf("this is also applicable to gold for the beacon, since it's the same amount as scraps.\n");
}

static void help(void){
    ver();
    printf("calculates how much more netherite you need to get a beacon.\n");
    printf("input how many scraps, ingots, and blocks you have.\n");
    printf("output is in scraps.\n\n");
    hint();
    printf("Usage:\n");
    printf("  netherite-left [options]\n\n");
    printf("Options:\n");
    printf("  -s, --scraps <N>     Number of scraps (integer)\n");
    printf("  -i, --ingots <N>     Number of ingots (integer)\n");
    printf("  -b, --blocks <N>     Number of blocks (integer)\n");
    printf("  -h, --help           Show this help and exit\n");
    printf("  -v, --version        Show version and exit\n");
    printf("  -g, --gold           Show hint for gold and exit\n");
}

int main(int argc, char **argv)
{     /* calculate in scraps, the smallest unit (gold is equivalent); accept options */
    int scraps = -1;
    int ingots = -1;
    int blocks = -1;
    int noninteractive = 0;

    static struct option longopts[] = {
        {"scraps",  required_argument, NULL, 's'},
        {"ingots",  required_argument, NULL, 'i'},
        {"blocks",  required_argument, NULL, 'b'},
        {"help",    no_argument,       NULL, 'h'},
        {"version", no_argument,       NULL, 'v'},
        {"gold",    no_argument,       NULL, 'g'},
        {0,0,0,0}
    };

    /* short options: s,i,b require arguments; h,v,g are flags (no argument) */
    int opt;
    while ((opt = getopt_long(argc, argv, "s:i:b:hvg", longopts, NULL)) != -1) {
        switch (opt) {
            case 's': scraps = atoi(optarg); break;
            case 'i': ingots = atoi(optarg); break;
            case 'b': blocks = atoi(optarg); break;
            case 'h': help(); return 0;
            case 'v': ver(); return 0;
            case 'g': hint(); return 0;
            default: bad_arg(); return 1;
        }
    }

    /* If none of the counts were provided on the command line, prompt interactively */
    if (scraps < 0 && ingots < 0 && blocks < 0) {
        if (scraps < 0) {
            printf("scraps? "); if (scanf("%d", &scraps) != 1) scraps = 0;
        }
        if (ingots < 0) {
            printf("ingots? "); if (scanf("%d", &ingots) != 1) ingots = 0;
        }
        if (blocks < 0) {
            printf("blocks? "); if (scanf("%d", &blocks) != 1) blocks = 0;
        }
    } else {
        /* any missing count defaults to zero when using CLI flags */
        if (scraps < 0) scraps = 0;
        if (ingots < 0) ingots = 0;
        if (blocks < 0) blocks = 0;
    }

    int total_scraps = scraps + (ingots * 4) + (blocks * 36);
    int scraps_needed = BEACON_REQ_SCRAPS - total_scraps;

    if (scraps_needed <= 0) {
        printf("have enough, add 1 to the counter\n");
    } else {
        printf("u need %d more netherite scraps.\n", scraps_needed);
        printf("%d/%d.\n", total_scraps, BEACON_REQ_SCRAPS);
    }
    return 0;
}