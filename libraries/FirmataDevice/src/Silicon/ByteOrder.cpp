#include "ByteOrder.h"

void ByteOrder::x2LE(uint16_t src, uint8_t *dst) {
  dst[0] = (src & 0xFF);
  dst[1] = ((src >> 8) & 0xFF);
}

void ByteOrder::x2BE(uint16_t src, uint8_t *dst) {
  dst[0] = ((src >> 8) & 0xFF);
  dst[1] = (src & 0xFF);
}

void ByteOrder::x4LE(uint32_t src, uint8_t *dst) {
  dst[0] = (src & 0xFF);
  dst[1] = ((src >> 8) & 0xFF);
  dst[2] = ((src >> 16) & 0xFF);
  dst[3] = ((src >> 24) & 0xFF);
}

void ByteOrder::x4BE(uint32_t src, uint8_t *dst) {
  dst[0] = ((src >> 24) & 0xFF);
  dst[1] = ((src >> 16) & 0xFF);
  dst[2] = ((src >> 8) & 0xFF);
  dst[3] = (src & 0xFF);
}

uint16_t ByteOrder::p2LE(uint8_t *src) {
  return ((src[1] & 0xFF) << 8) | (src[0] & 0xFF);
}

uint16_t ByteOrder::p2BE(uint8_t *src) {
  return ((src[0] & 0xFF) << 8) | (src[1] & 0xFF);
}

uint32_t ByteOrder::p4LE(uint8_t *src) {
  return ((src[3] & 0xFF) << 24) | ((src[2] & 0xFF) << 16) |
         ((src[1] & 0xFF) << 8) | (src[0] & 0xFF);
}

uint32_t ByteOrder::p4BE(uint8_t *src) {
  return ((src[0] & 0xFF) << 24) | ((src[1] & 0xFF) << 16) |
         ((src[2] & 0xFF) << 8) | (src[3] & 0xFF);
}
