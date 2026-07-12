#ifndef MC_DNS_H
#define MC_DNS_H

#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MC_HOSTNAME_MAX 256
#define MC_DEFAULT_PORT 25565

typedef struct {
    char host[MC_HOSTNAME_MAX];
    char requested_host[MC_HOSTNAME_MAX];
    uint16_t port;
    struct addrinfo *addr;
} mc_endpoint;

int mc_resolve(const char *hostname, uint16_t default_port, mc_endpoint *out);
void mc_endpoint_free(mc_endpoint *endpoint);

#endif /* MC_DNS_H */
