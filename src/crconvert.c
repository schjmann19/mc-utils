/*
 * CRConvert (Coordinate Relative Convert)
 * convert coords like `123 120 80` to `~23 ~20 ~-20` <- provided the user is positioned at `100 100 100`
 */

#include "../common_utils/simple_strings.h"
#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

str convert_hardcoded_coords_to_relative(str input_command, coord command_pos) {
    /* Split the command into space-separated parts */
    size_t count = 0;
    str *parts = str_split(&input_command, " ", &count);
    if (!parts || count == 0) {
        if (parts) {
            free(parts);
        }
        return str_clone(&input_command);
    }

    const char *cmd = cstr(&parts[0]);
    int start_idx = -1, end_idx = -1; /* inclusive indices of coord tokens */

    if (strcmp(cmd, "setblock") == 0 && count >= 4) {
        start_idx = 1;
        end_idx = 3; /* x y z */
    } else if (strcmp(cmd, "fill") == 0 && count >= 7) {
        start_idx = 1;
        end_idx = 6; /* x1 y1 z1 x2 y2 z2 */
    }

    if (start_idx >= 0) {
        for (int i = start_idx; i <= end_idx; ++i) {
            const char *tok = cstr(&parts[i]);
            if (!tok || tok[0] == '~' || tok[0] == '^' || tok[0] == '@') {
                continue; /* already relative or selector */
            }
            char *endp = NULL;
            long v = strtol(tok, &endp, 10);
            if (endp == tok) {
                continue; /* not a plain integer */
            }

            int coord_dim = (i - start_idx) % 3; /* 0=x,1=y,2=z */
            int base = (coord_dim == 0) ? command_pos.x : (coord_dim == 1) ? command_pos.y
                                                                           : command_pos.z;
            int offset = (int)(v - base);

            char buf[32];
            if (offset == 0) {
                snprintf(buf, sizeof(buf), "~");
            } else {
                snprintf(buf, sizeof(buf), "~%d", offset);
            }

            /* replace parts[i] */
            str_destroy(&parts[i]);
            parts[i] = str_create(buf);
        }
    }

    /* Reconstruct the command */
    str out = str_create("");
    for (size_t i = 0; i < count; ++i) {
        str_append(&out, cstr(&parts[i]));
        if (i + 1 < count) {
            str_append(&out, " ");
        }
    }

    /* cleanup */
    for (size_t i = 0; i < count; ++i) {
        str_destroy(&parts[i]);
    }
    free(parts);

    return out;
}
