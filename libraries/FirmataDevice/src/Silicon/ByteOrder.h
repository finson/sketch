#ifndef ByteOrder_h
#define ByteOrder_h

#include <stdint.h>

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#error "ByteOrder is not implemented for BigEndian hosts yet."

# elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

// The byte order in the byte array is Little Endian.

#define fromHostTo16LE(value,extractedBytes) ByteOrder::x2LE((value),(extractedBytes))
#define fromHostTo32LE(value,extractedBytes) ByteOrder::x4LE((value),(extractedBytes))

#define from16LEToHost(bytesToPack) ByteOrder::p2LE((bytesToPack))
#define from32LEToHost(bytesToPack) ByteOrder::p4LE((bytesToPack))

// The byte order in the byte array is Big Endian.

#define fromHostTo16BE(value,extractedBytes) ByteOrder::x2BE((value),(extractedBytes))
#define fromHostTo32BE(value,extractedBytes) ByteOrder::x4BE((value),(extractedBytes))

#define from16BEToHost(bytesToPack) ByteOrder::p2BE((bytesToPack))
#define from32BEToHost(bytesToPack) ByteOrder::p4BE((bytesToPack))

# else
# error "Unrecognized __BYTE_ORDER__ value."
# endif

// These methods perform the extract, swap, and pack operations

class ByteOrder {

public:
  static void x2LE(uint16_t value, uint8_t *dst);
  static void x4LE(uint32_t src, uint8_t *dst);

  static uint16_t p2LE(uint8_t *src);
  static uint32_t p4LE(uint8_t *src);

  static void x2BE(uint16_t value, uint8_t *dst);
  static void x4BE(uint32_t src, uint8_t *dst);

  static uint16_t p2BE(uint8_t *src);
  static uint32_t p4BE(uint8_t *src);

};

#endif
