#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "protocol.h"
#include "dns.h"
#include "packet.h"
#include "varint.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static int connect_to_endpoint(const mc_endpoint *endpoint) {
    for (struct addrinfo *ai = endpoint->addr; ai; ai = ai->ai_next) {
        int fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (fd < 0) {
            continue;
        }
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) {
            return fd;
        }
        close(fd);
    }
    return -1;
}

static int send_all(int fd, const void *buf, size_t len) {
    const uint8_t *ptr = buf;
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = send(fd, ptr + sent, len - sent, 0);
        if (n <= 0) {
            return -1;
        }
        sent += (size_t)n;
    }
    return 0;
}

static int recv_all(int fd, void *buf, size_t len) {
    uint8_t *ptr = buf;
    size_t recvd = 0;
    while (recvd < len) {
        ssize_t n = recv(fd, ptr + recvd, len - recvd, 0);
        if (n <= 0) {
            return -1;
        }
        recvd += (size_t)n;
    }
    return 0;
}

static int read_packet(int fd, mc_packet *pkt) {
    int32_t packet_len;
    if (mc_read_varint(fd, &packet_len) != 0 || packet_len <= 0 || packet_len > MC_PACKET_MAX) {
        return -1;
    }

    uint8_t *buf = malloc((size_t)packet_len);
    if (!buf) {
        return -1;
    }
    if (recv_all(fd, buf, (size_t)packet_len) != 0) {
        free(buf);
        return -1;
    }

    size_t used;
    int32_t packet_id;
    if (mc_read_varint_buf(buf, packet_len, &packet_id, &used) != 0) {
        free(buf);
        return -1;
    }

    pkt->length = packet_len - used;
    if (pkt->length > MC_PACKET_MAX) {
        free(buf);
        return -1;
    }
    memcpy(pkt->data, buf + used, pkt->length);
    free(buf);
    return packet_id;
}

static int send_handshake(int fd, const char *handshake_host, uint16_t port, int32_t protocol) {
    mc_packet pkt;
    pkt_begin(&pkt);
    pkt_write_varint(&pkt, 0);
    pkt_write_varint(&pkt, protocol);
    pkt_write_string(&pkt, handshake_host);
    pkt_write_u16(&pkt, port);
    pkt_write_varint(&pkt, 1);

    uint8_t out[MC_PACKET_MAX];
    size_t len = pkt_finish(&pkt, out, sizeof(out));
    return send_all(fd, out, len);
}

static int send_status_request(int fd) {
    mc_packet pkt;
    pkt_begin(&pkt);
    pkt_write_varint(&pkt, 0);
    uint8_t out[MC_PACKET_MAX];
    size_t len = pkt_finish(&pkt, out, sizeof(out));
    return send_all(fd, out, len);
}

static int send_ping(int fd, int64_t timestamp) {
    mc_packet pkt;
    pkt_begin(&pkt);
    pkt_write_varint(&pkt, 1);
    pkt_write_u64(&pkt, (uint64_t)timestamp);
    uint8_t out[MC_PACKET_MAX];
    size_t len = pkt_finish(&pkt, out, sizeof(out));
    return send_all(fd, out, len);
}

static int read_string_from_packet(const mc_packet *pkt, char **out_str) {
    size_t idx = 0;
    int32_t len;
    if (mc_read_varint_buf(pkt->data, pkt->length, &len, &idx) != 0) {
        return -1;
    }
    if (len < 0 || idx + (size_t)len > pkt->length) {
        return -1;
    }
    char *s = malloc((size_t)len + 1);
    if (!s) {
        return -1;
    }
    memcpy(s, pkt->data + idx, (size_t)len);
    s[len] = '\0';
    *out_str = s;
    return 0;
}

static const char *skip_json_whitespace(const char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
        p++;
    }
    return p;
}

static const char *skip_json_string(const char *s) {
    if (!s || *s != '"') {
        return NULL;
    }
    s++;
    while (*s) {
        if (*s == '\\') {
            s++;
            if (!*s) {
                return NULL;
            }
        } else if (*s == '"') {
            return s + 1;
        }
        s++;
    }
    return NULL;
}

static const char *parse_json_string_at(const char *src, char *dst, size_t dstcap) {
    if (!src || *src != '"' || !dst || dstcap == 0) {
        return NULL;
    }
    size_t len = strlen(dst);
    src++;
    while (*src && *src != '"') {
        if (len + 1 >= dstcap) {
            return NULL;
        }
        if (*src == '\\') {
            src++;
            if (!*src) {
                break;
            }
            switch (*src) {
                case '"': dst[len++] = '"'; break;
                case '\\': dst[len++] = '\\'; break;
                case '/': dst[len++] = '/'; break;
                case 'b': dst[len++] = '\b'; break;
                case 'f': dst[len++] = '\f'; break;
                case 'n': dst[len++] = '\n'; break;
                case 'r': dst[len++] = '\r'; break;
                case 't': dst[len++] = '\t'; break;
                case 'u':
                    for (int i = 0; i < 4 && *src; ++i) {
                        src++;
                    }
                    dst[len++] = '?';
                    break;
                default:
                    dst[len++] = *src;
                    break;
            }
        } else {
            dst[len++] = *src;
        }
        src++;
    }
    if (*src != '"') {
        return NULL;
    }
    dst[len] = '\0';
    return src + 1;
}

static char legacy_color_code(const char *color) {
    if (!color || !color[0]) {
        return '\0';
    }
    if (strcmp(color, "black") == 0) {
        return '0';
    }
    if (strcmp(color, "dark_blue") == 0) {
        return '1';
    }
    if (strcmp(color, "dark_green") == 0) {
        return '2';
    }
    if (strcmp(color, "dark_aqua") == 0) {
        return '3';
    }
    if (strcmp(color, "dark_red") == 0) {
        return '4';
    }
    if (strcmp(color, "dark_purple") == 0) {
        return '5';
    }
    if (strcmp(color, "gold") == 0) {
        return '6';
    }
    if (strcmp(color, "gray") == 0) {
        return '7';
    }
    if (strcmp(color, "dark_gray") == 0) {
        return '8';
    }
    if (strcmp(color, "blue") == 0) {
        return '9';
    }
    if (strcmp(color, "green") == 0) {
        return 'a';
    }
    if (strcmp(color, "aqua") == 0) {
        return 'b';
    }
    if (strcmp(color, "red") == 0) {
        return 'c';
    }
    if (strcmp(color, "light_purple") == 0) {
        return 'd';
    }
    if (strcmp(color, "yellow") == 0) {
        return 'e';
    }
    if (strcmp(color, "white") == 0) {
        return 'f';
    }
    return '\0';
}

static size_t append_section_code(char *out, size_t outcap, char code) {
    if (!out || outcap < 3 || code == '\0') {
        return 0;
    }
    size_t len = strlen(out);
    if (len + 2 >= outcap) {
        return 0;
    }
    out[len++] = (char)0xC2;
    out[len++] = (char)0xA7;
    out[len++] = code;
    out[len] = '\0';
    return 2;
}

static const char *skip_json_value(const char *p);

static const char *parse_description_value(const char *p, char *out, size_t outcap, char *color, size_t colorcap);

static const char *parse_json_object(const char *p, char *out, size_t outcap, char *color, size_t colorcap) {
    (void)colorcap;
    if (!p || *p != '{') {
        return NULL;
    }
    p++;

    char local_color[32] = "";
    if (color && color[0]) {
        strncpy(local_color, color, sizeof(local_color) - 1);
        local_color[sizeof(local_color) - 1] = '\0';
    }

    const char *scan = p;
    while (1) {
        scan = skip_json_whitespace(scan);
        if (!scan) {
            return NULL;
        }
        if (*scan == '}') {
            break;
        }
        if (*scan != '"') {
            return NULL;
        }
        char key[64] = {0};
        scan = parse_json_string_at(scan, key, sizeof(key));
        if (!scan) {
            return NULL;
        }
        scan = skip_json_whitespace(scan);
        if (!scan || *scan != ':') {
            return NULL;
        }
        scan++;
        scan = skip_json_whitespace(scan);
        if (!scan) {
            return NULL;
        }
        if (strcmp(key, "color") == 0) {
            char temp_color[32] = "";
            scan = parse_json_string_at(scan, temp_color, sizeof(temp_color));
            if (!scan) {
                return NULL;
            }
            if (temp_color[0]) {
                strncpy(local_color, temp_color, sizeof(local_color) - 1);
                local_color[sizeof(local_color) - 1] = '\0';
            }
        } else {
            scan = skip_json_value(scan);
            if (!scan) {
                return NULL;
            }
        }
        scan = skip_json_whitespace(scan);
        if (*scan == ',') {
            scan++;
            continue;
        }
        if (*scan == '}') {
            break;
        }
        return NULL;
    }

    while (1) {
        p = skip_json_whitespace(p);
        if (!p) {
            return NULL;
        }
        if (*p == '}') {
            return p + 1;
        }
        if (*p != '"') {
            return NULL;
        }
        char key[64] = {0};
        p = parse_json_string_at(p, key, sizeof(key));
        if (!p) {
            return NULL;
        }
        p = skip_json_whitespace(p);
        if (*p != ':') {
            return NULL;
        }
        p++;
        p = skip_json_whitespace(p);
        if (!p) {
            return NULL;
        }
        if (strcmp(key, "text") == 0) {
            if (!out) {
                return NULL;
            }
            p = parse_description_value(p, out, outcap, local_color, sizeof(local_color));
            if (!p) {
                return NULL;
            }
        } else if (strcmp(key, "color") == 0) {
            p = skip_json_value(p);
            if (!p) {
                return NULL;
            }
        } else if (strcmp(key, "extra") == 0) {
            p = parse_description_value(p, out, outcap, local_color, sizeof(local_color));
            if (!p) {
                return NULL;
            }
        } else {
            p = skip_json_value(p);
            if (!p) {
                return NULL;
            }
        }
        p = skip_json_whitespace(p);
        if (*p == ',') {
            p++;
            continue;
        }
        if (*p == '}') {
            return p + 1;
        }
        return NULL;
    }
}

static const char *parse_json_array(const char *p, char *out, size_t outcap, char *color, size_t colorcap) {
    if (!p || *p != '[') {
        return NULL;
    }
    p++;
    while (1) {
        p = skip_json_whitespace(p);
        if (!p) {
            return NULL;
        }
        if (*p == ']') {
            return p + 1;
        }
        p = parse_description_value(p, out, outcap, color, colorcap);
        if (!p) {
            return NULL;
        }
        p = skip_json_whitespace(p);
        if (*p == ',') {
            p++;
            continue;
        }
        if (*p == ']') {
            return p + 1;
        }
        return NULL;
    }
}

static const char *parse_description_value(const char *p, char *out, size_t outcap, char *color, size_t colorcap) {
    p = skip_json_whitespace(p);
    if (!p) {
        return NULL;
    }
    if (*p == '"') {
        if (out && color && color[0]) {
            char code = legacy_color_code(color);
            if (code) {
                append_section_code(out, outcap, code);
            }
        }
        if (out) {
            return parse_json_string_at(p, out, outcap);
        }
        if (color && colorcap > 0) {
            return parse_json_string_at(p, color, colorcap);
        }
        return skip_json_string(p);
    }
    if (*p == '{') {
        return parse_json_object(p, out, outcap, color, colorcap);
    }
    if (*p == '[') {
        return parse_json_array(p, out, outcap, color, colorcap);
    }
    return NULL;
}

static const char *skip_json_value(const char *p) {
    p = skip_json_whitespace(p);
    if (!p) {
        return NULL;
    }
    if (*p == '"') {
        return skip_json_string(p);
    }
    if (*p == '{') {
        p++;
        while (*p) {
            p = skip_json_whitespace(p);
            if (!p) {
                return NULL;
            }
            if (*p == '}') {
                return p + 1;
            }
            p = skip_json_string(p);
            if (!p) {
                return NULL;
            }
            p = skip_json_whitespace(p);
            if (*p != ':') {
                return NULL;
            }
            p++;
            p = skip_json_value(p);
            if (!p) {
                return NULL;
            }
            p = skip_json_whitespace(p);
            if (*p == ',') {
                p++;
                continue;
            }
            if (*p == '}') {
                return p + 1;
            }
            return NULL;
        }
        return NULL;
    }
    if (*p == '[') {
        p++;
        while (*p) {
            p = skip_json_whitespace(p);
            if (!p) {
                return NULL;
            }
            if (*p == ']') {
                return p + 1;
            }
            p = skip_json_value(p);
            if (!p) {
                return NULL;
            }
            p = skip_json_whitespace(p);
            if (*p == ',') {
                p++;
                continue;
            }
            if (*p == ']') {
                return p + 1;
            }
            return NULL;
        }
        return NULL;
    }
    if (*p == 't' && strncmp(p, "true", 4) == 0) {
        return p + 4;
    }
    if (*p == 'f' && strncmp(p, "false", 5) == 0) {
        return p + 5;
    }
    if (*p == 'n' && strncmp(p, "null", 4) == 0) {
        return p + 4;
    }
    if (*p == '-' || (*p >= '0' && *p <= '9')) {
        if (*p == '-') {
            p++;
        }
        while (*p >= '0' && *p <= '9') {
            p++;
        }
        if (*p == '.') {
            p++;
            while (*p >= '0' && *p <= '9') {
                p++;
            }
        }
        if (*p == 'e' || *p == 'E') {
            p++;
            if (*p == '+' || *p == '-') {
                p++;
            }
            while (*p >= '0' && *p <= '9') {
                p++;
            }
        }
        return p;
    }
    return NULL;
}

static int parse_description_fallback(const char *json, char *out, size_t outcap, char *color, size_t colorcap) {
    const char *desc = strstr(json, "\"description\"");
    if (!desc) {
        return -1;
    }
    const char *cursor = desc;
    if (out && outcap > 0) {
        out[0] = '\0';
    }
    if (color && colorcap > 0) {
        color[0] = '\0';
    }
    bool found = false;

    while ((cursor = strstr(cursor, "\"text\"")) != NULL) {
        const char *colon = strchr(cursor, ':');
        if (!colon) {
            break;
        }
        const char *p = skip_json_whitespace(colon + 1);
        if (*p == '"') {
            size_t current_len = out ? strlen(out) : 0;
            if (out && current_len + 1 < outcap) {
                char temp[512];
                const char *next = parse_json_string_at(p, temp, sizeof(temp));
                if (!next) {
                    break;
                }
                if (current_len + strlen(temp) + 1 >= outcap) {
                    break;
                }
                strcat(out, temp);
                found = true;
            }
        }
        cursor = colon + 1;
    }

    cursor = desc;
    while ((cursor = strstr(cursor, "\"color\"")) != NULL && color && colorcap > 0 && !color[0]) {
        const char *colon = strchr(cursor, ':');
        if (!colon) {
            break;
        }
        const char *p = skip_json_whitespace(colon + 1);
        if (*p == '"') {
            parse_json_string_at(p, color, colorcap);
            break;
        }
        cursor = colon + 1;
    }

    return found ? 0 : -1;
}

static int parse_description_field(const char *json, char *out, size_t outcap, char *color, size_t colorcap) {
    const char *desc = strstr(json, "\"description\"");
    if (!desc) {
        return -1;
    }
    const char *colon = strchr(desc, ':');
    if (!colon) {
        return -1;
    }
    const char *p = skip_json_whitespace(colon + 1);
    if (color && colorcap > 0) {
        color[0] = '\0';
    }
    if (!p) {
        return -1;
    }
    if (out) {
        out[0] = '\0';
    }
    p = parse_description_value(p, out, outcap, color, colorcap);
    if (p) {
        return 0;
    }
    return parse_description_fallback(json, out, outcap, color, colorcap);
}

static int parse_int_field(const char *json, const char *key, int *out) {
    const char *p = strstr(json, key);
    if (!p) {
        return -1;
    }
    p += strlen(key);
    p = strchr(p, ':');
    if (!p) {
        return -1;
    }
    p++;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
        p++;
    }
    int sign = 1;
    if (*p == '-') {
        sign = -1;
        p++;
    }
    if (*p < '0' || *p > '9') {
        return -1;
    }
    int value = 0;
    while (*p >= '0' && *p <= '9') {
        value = value * 10 + (*p - '0');
        p++;
    }
    *out = value * sign;
    return 0;
}

static int parse_status_json(mc_status *status, const char *json) {
    if (!status || !json) {
        return -1;
    }
    status->json = str_create(json);
    if (!status->json.data) {
        return -1;
    }
    status->online = true;
    status->players_online = -1;
    status->players_max = -1;
    status->protocol = -1;
    status->latency_ms = -1;
    status->version[0] = '\0';
    status->motd[0] = '\0';
    status->motd_color[0] = '\0';

    const char *ver = strstr(json, "\"version\"");
    if (ver) {
        const char *name = strstr(ver, "\"name\"");
        if (name) {
            const char *quote = strchr(name + 7, '"');
            if (quote) {
                const char *quote2 = strchr(quote + 1, '"');
                if (quote2 && quote2 > quote + 1) {
                    size_t len = quote2 - quote - 1;
                    if (len > sizeof(status->version) - 1) {
                        len = sizeof(status->version) - 1;
                    }
                    memcpy(status->version, quote + 1, len);
                    status->version[len] = '\0';
                }
            }
        }
        parse_int_field(ver, "\"protocol\"", &status->protocol);
    }

    const char *players = strstr(json, "\"players\"");
    if (players) {
        parse_int_field(players, "\"online\"", &status->players_online);
        parse_int_field(players, "\"max\"", &status->players_max);
    }

    if (parse_description_field(json, status->motd, sizeof(status->motd), status->motd_color, sizeof(status->motd_color)) != 0) {
        status->motd[0] = '\0';
        status->motd_color[0] = '\0';
    }

    return 0;
}

void mc_status_free(mc_status *status) {
    if (!status) {
        return;
    }
    str_destroy(&status->json);
}

static int query_status_with_protocol(const mc_endpoint *endpoint, int32_t protocol, mc_status *status) {
    int fd = connect_to_endpoint(endpoint);
    if (fd < 0) {
        return -1;
    }

    if (send_handshake(fd, endpoint->requested_host, endpoint->port, protocol) != 0) {
        close(fd);
        return -1;
    }
    if (send_status_request(fd) != 0) {
        close(fd);
        return -1;
    }

    mc_packet reply;
    int packet_id = read_packet(fd, &reply);
    if (packet_id < 0) {
        close(fd);
        return -1;
    }

    char *json = NULL;
    if (read_string_from_packet(&reply, &json) != 0) {
        close(fd);
        return -1;
    }

    if (parse_status_json(status, json) != 0) {
        free(json);
        close(fd);
        return -1;
    }
    free(json);

    struct timeval before, after;
    gettimeofday(&before, NULL);
    if (send_ping(fd, (int64_t)before.tv_sec * 1000 + before.tv_usec / 1000) == 0 && read_packet(fd, &reply) >= 0) {
        gettimeofday(&after, NULL);
        status->latency_ms = (int)((after.tv_sec - before.tv_sec) * 1000 + (after.tv_usec - before.tv_usec) / 1000);
    } else {
        status->latency_ms = -1;
    }

    close(fd);
    return 0;
}

int mc_status_query(const char *hostname, uint16_t port, mc_status *status) {
    if (!hostname || !status) {
        return -1;
    }
    memset(status, 0, sizeof(*status));

    mc_endpoint endpoint;
    if (mc_resolve(hostname, port, &endpoint) != 0) {
        return -1;
    }

    int rc = query_status_with_protocol(&endpoint, MC_PROTOCOL_UNKNOWN, status);
    if (rc != 0) {
        rc = query_status_with_protocol(&endpoint, MC_PROTOCOL_FALLBACK, status);
    }

    mc_endpoint_free(&endpoint);
    return rc;
}
