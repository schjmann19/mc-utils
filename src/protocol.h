#ifndef MC_PROTOCOL_H
#define MC_PROTOCOL_H

#include "../../common_utils/simple_strings.h"
#include <stdbool.h>
#include <stdint.h>

#define MC_PROTOCOL_UNKNOWN -1
#define MC_PROTOCOL_FALLBACK 0

typedef struct {
    bool online;
    char version[64];
    char motd[256];
    char motd_color[32];
    int protocol;
    int players_online;
    int players_max;
    str json;
    int latency_ms;
} mc_status;

int mc_status_query(const char *hostname, uint16_t port, mc_status *status);
void mc_status_free(mc_status *status);

#endif /* MC_PROTOCOL_H */
