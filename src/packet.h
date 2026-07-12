#ifndef MC_PACKET_H
#define MC_PACKET_H

#include "varint.h"
#include <stddef.h>
#include <stdint.h>

#define MC_PACKET_MAX 65536

typedef struct {
    uint8_t data[MC_PACKET_MAX];
    size_t length;
} mc_packet;

void pkt_begin(mc_packet *pkt);
void pkt_write_varint(mc_packet *pkt, int32_t value);
void pkt_write_string(mc_packet *pkt, const char *s);
void pkt_write_u16(mc_packet *pkt, uint16_t value);
void pkt_write_u64(mc_packet *pkt, uint64_t value);
size_t pkt_finish(const mc_packet *pkt, uint8_t *out_buf, size_t out_cap);

#endif /* MC_PACKET_H */
