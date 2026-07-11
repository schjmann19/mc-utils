#include "varint.h"
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

size_t mc_write_varint(uint8_t *buf, int32_t value) {
    size_t i = 0;
    uint32_t v = (uint32_t)value;
    while (true) {
        if ((v & ~0x7F) == 0) {
            buf[i++] = (uint8_t)v;
            break;
        }
        buf[i++] = (uint8_t)((v & 0x7F) | 0x80);
        v >>= 7;
    }
    return i;
}

int mc_read_varint(int fd, int32_t *value) {
    uint8_t byte;
    int32_t result = 0;
    int shift = 0;
    for (int i = 0; i < 5; ++i) {
        ssize_t n = read(fd, &byte, 1);
        if (n <= 0) {
            return -1;
        }
        result |= (int32_t)(byte & 0x7F) << shift;
        if (!(byte & 0x80)) {
            *value = result;
            return 0;
        }
        shift += 7;
    }
    return -1;
}

int mc_read_varint_buf(const uint8_t *buf, size_t buflen, int32_t *value, size_t *read_bytes) {
    int32_t result = 0;
    int shift = 0;
    size_t i = 0;
    for (; i < buflen && i < 5; ++i) {
        uint8_t byte = buf[i];
        result |= (int32_t)(byte & 0x7F) << shift;
        if (!(byte & 0x80)) {
            *value = result;
            if (read_bytes) {
                *read_bytes = i + 1;
            }
            return 0;
        }
        shift += 7;
    }
    return -1;
}
