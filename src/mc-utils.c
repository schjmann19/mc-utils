#define _POSIX_C_SOURCE 200809L
#include "aux.h"
#include "crconvert.h"
#include "netherite-left.h"
#include "recipes.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int32_t extract_recipes(const char *jar_path, const char *output_path);
extern int32_t download_minecraft_jar(const char *dest);

#ifdef HAVE_EMBEDDED_EXTRACTOR
extern unsigned char embedded_extractor[];
extern unsigned int embedded_extractor_len;
#endif

#define STACK 64
#define SMALL_STACK 16

static int file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        return 0;
    }
    fclose(f);
    return 1;
}

/* Download latest minecraft client jar into minecraftjar/<version>.jar */
static int download_latest_jar(char *dest, size_t destlen) {
    snprintf(dest, destlen, "minecraftjar/latest.jar");
    return download_minecraft_jar(dest);
}

/* Run the extractor binary (if present) or fall back to `cargo run`.
   jar_path is the path to the downloaded/provided jar. Returns 0 on success. */
static int run_extractor_with_jar(const char *jar_path) {
    return extract_recipes(jar_path, "extractor/recipes.json");
}

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
    printf("\n");
}

int calculate_total(int num_stacks, int remaining_items, int stack_size) {
    return (num_stacks * stack_size) + remaining_items;
}

int main(int argc, char *argv[]) {
    /* allow overriding recipes file: remove any --recipes=... or --recipes <path> args early */
    const char *recipes_path = "extractor/recipes.json";
    char **nargv = malloc(sizeof(char *) * (argc + 1));
    if (!nargv) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    nargv[0] = argv[0];
    int j = 1;
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--recipes=", 10) == 0) {
            recipes_path = argv[i] + 10;
        } else if (strcmp(argv[i], "--recipes") == 0) {
            if (i + 1 < argc) {
                recipes_path = argv[i + 1];
                ++i;
            }
        } else {
            nargv[j++] = argv[i];
        }
    }
    int narrc = j;
    nargv[narrc] = NULL; // ensure terminated
    argv = nargv;
    argc = narrc;

    /* support calling the netherite tool with cli flags rather than argv[0]. */
    if (argc > 1 && (strcmp(argv[1], "-nl") == 0 || strcmp(argv[1], "--netherite") == 0 || strcmp(argv[1], "--netherite-left") == 0)) {
        /* Build a new argv for the netherite_left() call that strips the wrapper flag */
        int newargc = argc - 1;
        char **newargv = malloc(sizeof(char *) * (newargc + 1));
        if (!newargv) {
            fprintf(stderr, "Out of memory\n");
            return 1;
        }
        newargv[0] = "netherite-left";
        for (int i = 2; i < argc; ++i) {
            newargv[i - 1] = argv[i];
        }
        newargv[newargc] = NULL;
        int ret = netherite_left(newargc, newargv);
        free(newargv);
        return ret;
    }

    if (argc < 2) {
        help();
        return 1;
    }

    // check for version and help flags first
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        ver();
        return 0;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        help();
        return 0;
    }

    if (strcmp(argv[1], "--goal") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: --goal requires a number\n");
            return 1;
        }

        int goal = atoi(argv[2]);
        if (goal <= 0) {
            fprintf(stderr, "Please provide a positive number\n");
            return 1;
        }

        int stack_size = STACK; // default to normal stack size
        if (argc > 3 && strcmp(argv[3], "--small") == 0) {
            stack_size = SMALL_STACK;
        } // or use small stacks

        calculate_stacks(goal, stack_size);
    } else if (strcmp(argv[1], "--enchant") == 0) {
        enchanting_table();
    } else if (strcmp(argv[1], "--anvil") == 0) {
        anvil();
    } else if (strcmp(argv[1], "--download") == 0) {
        printf("Downloading latest Minecraft jar...\n");
        char dest[] = "minecraftjar/latest.jar";
        if (download_minecraft_jar(dest) != 0) {
            fprintf(stderr, "Failed to download jar\n");
            return 1;
        }
        printf("Downloaded to %s\n", dest);
    } else if (strcmp(argv[1], "--crconvert") == 0) {
        /* Usage: mc-util --crconvert X Y Z "command..." */
        if (argc < 6) {
            fprintf(stderr, "Usage: %s --crconvert <x> <y> <z> \"command\"\n", argv[0]);
            return 1;
        }
        coord cp = {atoi(argv[2]), atoi(argv[3]), atoi(argv[4])};
        size_t rem = (size_t)(argc - 5);
        const char **arr = malloc(sizeof(char *) * rem);
        if (!arr) {
            fprintf(stderr, "Out of memory\n");
            return 1;
        }
        for (size_t i = 0; i < rem; ++i) {
            arr[i] = argv[5 + i];
        }
        str cmd = str_join(" ", arr, rem);
        free(arr);
        str out = convert_hardcoded_coords_to_relative(cmd, cp);
        printf("%s\n", cstr(&out));
        str_destroy(&cmd);
        str_destroy(&out);
    } else if (strcmp(argv[1], "--recipe") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: --recipe requires an item name\n");
            return 1;
        }
        print_recipe(argv[2], recipes_path);
    } else if (strcmp(argv[1], "--generate-list") == 0) {
        list_items(recipes_path);
    } else if (strcmp(argv[1], "--generate-recipes") == 0 || strcmp(argv[1], "generate_recipes") == 0) {
        /* options: --jar <path> or --download */
        const char *jar_path = NULL;
        int download = 0;
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--jar") == 0 && i + 1 < argc) {
                jar_path = argv[i + 1];
                i++;
            } else if (strncmp(argv[i], "--jar=", 6) == 0) {
                jar_path = argv[i] + 6;
            } else if (strcmp(argv[i], "--download") == 0) {
                download = 1;
            } else if (strcmp(argv[i], "--help") == 0) {
                printf("Usage: --generate-recipes [--jar path] [--download]\n");
                return 0;
            }
        }

        char jarbuf[512];
        if (download) {
            printf("Downloading latest Minecraft jar...\n");
            if (download_latest_jar(jarbuf, sizeof(jarbuf)) != 0) {
                fprintf(stderr, "Failed to download jar\n");
                return 1;
            }
            jar_path = jarbuf;
        }
        if (!jar_path) {
            /* default to latest downloaded jar */
            jar_path = "minecraftjar/latest.jar";
        }

        if (!file_exists(jar_path)) {
            fprintf(stderr, "JAR not found: %s\n", jar_path);
            return 1;
        }
        printf("Running extractor on %s...\n", jar_path);
        int r = run_extractor_with_jar(jar_path);
        if (r != 0) {
            fprintf(stderr, "Extractor failed (code %d)\n", r);
            return 1;
        }
        printf("Extractor finished.\n");
    } else if (strcmp(argv[1], "--total") == 0 || strcmp(argv[1], "-t") == 0) {
        int stack_size = STACK;
        int num_stacks = 0;
        int remaining_items = 0;
        int i = 2;

        while (i < argc) {
            if (strcmp(argv[i], "--stacks") == 0 || strcmp(argv[i], "-s") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "Error: --stacks requires a number\n");
                    return 1;
                }
                num_stacks = atoi(argv[i + 1]);
                i += 2;
            } else if (strcmp(argv[i], "--items") == 0 || strcmp(argv[i], "-i") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "Error: --items requires a number\n");
                    return 1;
                }
                remaining_items = atoi(argv[i + 1]);
                i += 2;
            } else if (strcmp(argv[i], "--stacks-small") == 0) {
                stack_size = SMALL_STACK;
                i++;
            } else {
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
    } else {
        fprintf(stderr, "Unknown option: %s\n", argv[1]);
        print_usage();
        return 1;
    }
    return 0;
}

/* remember, never trust the government. */
