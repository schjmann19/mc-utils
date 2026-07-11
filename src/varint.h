#ifndef MC_VARINT_H
#define MC_VARINT_H

#include <stdint.h>
#include <stddef.h>

size_t mc_write_varint(uint8_t *buf, int32_t value);
int mc_read_varint(int fd, int32_t *value);
int mc_read_varint_buf(const uint8_t *buf, size_t buflen, int32_t *value, size_t *read_bytes);

#endif /* MC_VARINT_H */
