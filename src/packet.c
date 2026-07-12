#include "packet.h"
#include <string.h>

void pkt_begin(mc_packet *pkt) {
    pkt->length = 0;
}

void pkt_write_varint(mc_packet *pkt, int32_t value) {
    uint8_t buf[8];
    size_t n = mc_write_varint(buf, value);
    if (pkt->length + n <= MC_PACKET_MAX) {
        memcpy(pkt->data + pkt->length, buf, n);
        pkt->length += n;
    }
}

void pkt_write_string(mc_packet *pkt, const char *s) {
    size_t len = strlen(s);
    pkt_write_varint(pkt, (int32_t)len);
    if (pkt->length + len <= MC_PACKET_MAX) {
        memcpy(pkt->data + pkt->length, s, len);
        pkt->length += len;
    }
}

void pkt_write_u16(mc_packet *pkt, uint16_t value) {
    if (pkt->length + 2 <= MC_PACKET_MAX) {
        pkt->data[pkt->length++] = (uint8_t)((value >> 8) & 0xFF);
        pkt->data[pkt->length++] = (uint8_t)(value & 0xFF);
    }
}

void pkt_write_u64(mc_packet *pkt, uint64_t value) {
    if (pkt->length + 8 <= MC_PACKET_MAX) {
        for (int i = 7; i >= 0; --i) {
            pkt->data[pkt->length++] = (uint8_t)((value >> (i * 8)) & 0xFF);
        }
    }
}

size_t pkt_finish(const mc_packet *pkt, uint8_t *out_buf, size_t out_cap) {
    size_t header_len = mc_write_varint(out_buf, (int32_t)pkt->length);
    if (header_len + pkt->length > out_cap) {
        return 0;
    }
    memcpy(out_buf + header_len, pkt->data, pkt->length);
    return header_len + pkt->length;
}
