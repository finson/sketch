#include "ByteSwap.h"

void ByteSwap::b2(uint8_t *dst, uint8_t *src) {
  dst[0] = src[1];
  dst[1] = src[0];
}

void ByteSwap::b4(uint8_t *dst, uint8_t *src) {
  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
}
