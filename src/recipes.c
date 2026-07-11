#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file_all(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long sz = ftell(f);
    if (sz < 0) {
        fclose(f);
        return NULL;
    }
    rewind(f);
    char *buf = malloc(sz + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t n = fread(buf, 1, sz, f);
    buf[n] = '\0';
    fclose(f);
    return buf;
}

void print_recipe(const char *item, const char *recipes_path) {
    char full[256];
    if (strncmp(item, "minecraft:", 10) == 0) {
        strncpy(full, item, sizeof(full));
        full[sizeof(full) - 1] = '\0';
    } else {
        snprintf(full, sizeof(full), "minecraft:%s", item);
    }

    char *buf = read_file_all(recipes_path);
    if (!buf) {
        fprintf(stderr, "Could not open %s\n", recipes_path);
        return;
    }

    char *p = buf;
    int found = 0;
    while ((p = strstr(p, "\"result\"")) != NULL) {
        /* look for an "id": "..." inside the result object */
        char *idpos = strstr(p, "\"id\": \"");
        if (!idpos) {
            p = p + 8;
            continue;
        }
        char *idstart = idpos + strlen("\"id\": \"");
        char *idend = strchr(idstart, '"');
        if (!idend) {
            p = idpos + 1;
            continue;
        }
        size_t idlen = idend - idstart;
        size_t fulllen = strlen(full);
        size_t itemlen = strlen(item);
        int match = 0;
        if (idlen == fulllen && strncmp(idstart, full, idlen) == 0) {
            match = 1;
        } else if (idlen == itemlen && strncmp(idstart, item, idlen) == 0) {
            match = 1;
        } else if (idlen == (10 + itemlen) && strncmp(idstart, "minecraft:", 10) == 0 && strncmp(idstart + 10, item, itemlen) == 0) {
            match = 1;
        }
        if (!match) {
            p = idpos + 1;
            continue;
        }

        /* locate the enclosing "data" object for this result to avoid nearby recipes */
        char *data_key = NULL;
        char *b = p;
        while (b > buf) {
            if (strncmp(b, "\"data\"", 6) == 0) {
                data_key = b;
                break;
            }
            b--;
        }
        char *obj_start = NULL;
        if (data_key) {
            obj_start = strchr(data_key, '{');
        }
        if (!obj_start) {
            /* fallback: find previous '{' */
            b = p;
            while (b > buf && *b != '{') {
                b--;
            }
            obj_start = b;
        }
        if (!obj_start) {
            p = idpos + 1;
            continue;
        }
        int braces = 0;
        char *r = obj_start;
        for (; *r; ++r) {
            if (*r == '{') {
                braces++;
            } else if (*r == '}') {
                braces--;
                if (braces == 0) {
                    break;
                }
            }
        }
        char *obj_end = r;
        size_t win_len = (obj_end && obj_end > obj_start) ? (obj_end - obj_start + 1) : 0;
        char *win = NULL;
        if (win_len) {
            win = malloc(win_len + 1);
            if (!win) {
                break;
            }
            memcpy(win, obj_start, win_len);
            win[win_len] = '\0';
        }

        printf("Recipe for %s:\n", full);
        if (win && strstr(win, "crafting_shaped")) {
            printf("Type: shaped\n");
        } else if (win && strstr(win, "crafting_shapeless")) {
            printf("Type: shapeless\n");
        }

        /* result count */
        if (win) {
            char *countp = strstr(win, "\"count\"");
            if (countp) {
                char *colon = strchr(countp, ':');
                if (colon) {
                    int cnt = atoi(colon + 1);
                    printf("Output count: %d\n", cnt);
                }
            }
        }

        /* pattern */
        if (win && strstr(win, "\"pattern\"")) {
            printf("Pattern:\n");
            char *pat = strstr(win, "\"pattern\"");
            if (pat) {
                char *br = strchr(pat, '[');
                if (br) {
                    char *s = br + 1;
                    while (1) {
                        char *q = strchr(s, '"');
                        if (!q) {
                            break;
                        }
                        char *q2 = strchr(q + 1, '"');
                        if (!q2) {
                            break;
                        }
                        int len = q2 - (q + 1);
                        char *line = malloc(len + 1);
                        memcpy(line, q + 1, len);
                        line[len] = '\0';
                        printf("  %s\n", line);
                        free(line);
                        s = q2 + 1;
                        char *closebr = strchr(s, ']');
                        if (closebr && (strchr(s, '"') == NULL || closebr < strchr(s, '"'))) {
                            break;
                        }
                    }
                }
            }
        }

        /* key mapping */
        if (win && strstr(win, "\"key\"")) {
            printf("Key mapping:\n");
            char *kp = strstr(win, "\"key\"");
            char *kbrace = kp ? strchr(kp, '{') : NULL;
            if (kbrace) {
                char *s = kbrace + 1;
                while (1) {
                    char *q = strchr(s, '"');
                    if (!q) {
                        break;
                    }
                    char *q2 = strchr(q + 1, '"');
                    if (!q2) {
                        break;
                    }
                    char keychar = q[1];
                    char *colon = strchr(q2, ':');
                    if (!colon) {
                        break;
                    }
                    char *valq = strchr(colon, '"');
                    if (!valq) {
                        break;
                    }
                    char *valq2 = strchr(valq + 1, '"');
                    if (!valq2) {
                        break;
                    }
                    int len = valq2 - (valq + 1);
                    char *val = malloc(len + 1);
                    memcpy(val, valq + 1, len);
                    val[len] = '\0';
                    printf("  %c -> %s\n", keychar, val);
                    free(val);
                    s = valq2 + 1;
                    char *closebr = strchr(s, '}');
                    if (closebr && (strchr(s, '"') == NULL || closebr < strchr(s, '"'))) {
                        break;
                    }
                }
            }
        }

        /* ingredients (shapeless or subrecipes) */
        if (win && strstr(win, "\"ingredients\"")) {
            printf("Ingredients:\n");
            char *ing = strstr(win, "\"ingredients\"");
            if (ing) {
                char *br = strchr(ing, '[');
                if (br) {
                    char *s = br + 1;
                    while (1) {
                        char *q = strchr(s, '"');
                        if (!q) {
                            break;
                        }
                        char *q2 = strchr(q + 1, '"');
                        if (!q2) {
                            break;
                        }
                        int len = q2 - (q + 1);
                        char *val = malloc(len + 1);
                        memcpy(val, q + 1, len);
                        val[len] = '\0';
                        printf("  %s\n", val);
                        free(val);
                        s = q2 + 1;
                        char *closebr = strchr(s, ']');
                        if (closebr && (strchr(s, '"') == NULL || closebr < strchr(s, '"'))) {
                            break;
                        }
                    }
                }
            }
        }

        printf("\n");
        if (win) {
            free(win);
        }
        found++;

        p = idpos + 1;
    }
    if (!found) {
        printf("No recipe found for %s\n", full);
    }
    free(buf);
}

void list_items(const char *recipes_path) {
    char *buf = read_file_all(recipes_path);
    if (!buf) {
        fprintf(stderr, "Could not open %s\n", recipes_path);
        return;
    }

    /* Track seen items to avoid duplicates */
    char **seen = malloc(sizeof(char *) * 2000); /* max ~2000 unique items */
    int seen_count = 0;

    char *p = buf;
    while ((p = strstr(p, "\"result\"")) != NULL) {
        /* look for an "id": "..." inside the result object */
        char *idpos = strstr(p, "\"id\": \"");
        if (!idpos) {
            p = p + 8;
            continue;
        }
        char *idstart = idpos + strlen("\"id\": \"");
        char *idend = strchr(idstart, '"');
        if (!idend) {
            p = idpos + 1;
            continue;
        }
        size_t idlen = idend - idstart;

        /* extract the item ID */
        char *itemid = malloc(idlen + 1);
        if (!itemid) {
            break;
        }
        memcpy(itemid, idstart, idlen);
        itemid[idlen] = '\0';

        /* check if we've already seen this item */
        int already_seen = 0;
        for (int i = 0; i < seen_count; i++) {
            if (strcmp(seen[i], itemid) == 0) {
                already_seen = 1;
                break;
            }
        }

        if (!already_seen && seen_count < 2000) {
            printf("%s\n", itemid);
            seen[seen_count++] = itemid;
        } else {
            free(itemid);
        }

        p = idpos + 1;
    }

    for (int i = 0; i < seen_count; i++) {
        free(seen[i]);
    }
    free(seen);
    free(buf);
}
