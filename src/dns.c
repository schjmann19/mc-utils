#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "dns.h"
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <resolv.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static bool is_ip_literal(const char *host) {
    struct in6_addr a6;
    struct in_addr a4;
    return inet_pton(AF_INET, host, &a4) == 1 || inet_pton(AF_INET6, host, &a6) == 1;
}

int mc_resolve(const char *hostname, uint16_t default_port, mc_endpoint *out) {
    if (!hostname || !out) {
        return -1;
    }
    memset(out, 0, sizeof(*out));
    strncpy(out->requested_host, hostname, MC_HOSTNAME_MAX - 1);
    out->requested_host[MC_HOSTNAME_MAX - 1] = '\0';
    out->port = default_port ? default_port : MC_DEFAULT_PORT;

    if (is_ip_literal(hostname)) {
        strncpy(out->host, hostname, MC_HOSTNAME_MAX - 1);
        out->host[MC_HOSTNAME_MAX - 1] = '\0';
    } else {
        unsigned char answer[65536];
        char srv_name[MC_HOSTNAME_MAX + 12];
        snprintf(srv_name, sizeof(srv_name), "_minecraft._tcp.%s", hostname);
        int ancount = res_query(srv_name, C_IN, T_SRV, answer, sizeof(answer));
        if (ancount > 0) {
            ns_msg handle;
            if (ns_initparse(answer, ancount, &handle) == 0) {
                int n = ns_msg_count(handle, ns_s_an);
                for (int i = 0; i < n; i++) {
                    ns_rr rr;
                    if (ns_parserr(&handle, ns_s_an, i, &rr) == 0 && ns_rr_type(rr) == ns_t_srv) {
                        const unsigned char *rdata = ns_rr_rdata(rr);
                        uint16_t port = ns_get16(rdata + 4);
                        char target[MC_HOSTNAME_MAX];
                        if (dn_expand(answer, answer + ancount, rdata + 6, target, sizeof(target)) >= 0) {
                            strncpy(out->host, target, MC_HOSTNAME_MAX - 1);
                            out->host[MC_HOSTNAME_MAX - 1] = '\0';
                            out->port = port;
                            break;
                        }
                    }
                }
            }
        }
        if (out->host[0] == '\0') {
            strncpy(out->host, hostname, MC_HOSTNAME_MAX - 1);
            out->host[MC_HOSTNAME_MAX - 1] = '\0';
        }
    }

    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    char portbuf[16];
    snprintf(portbuf, sizeof(portbuf), "%u", out->port);
    if (getaddrinfo(out->host, portbuf, &hints, &out->addr) != 0) {
        return -1;
    }
    return 0;
}

void mc_endpoint_free(mc_endpoint *endpoint) {
    if (!endpoint) {
        return;
    }
    if (endpoint->addr) {
        freeaddrinfo(endpoint->addr);
    }
    endpoint->addr = NULL;
}
