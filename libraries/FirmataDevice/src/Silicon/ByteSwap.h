#ifndef ByteSwap_h
#define ByteSwap_h

#include <inttypes.h>

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

// The host byte order is Big Endian and is the same as the desired
// Big Endian byte order, so copy directly across.

#define fromHostTo16BE(x,y) memcpy((y),(x),2)
#define fromHostTo32BE(x,y) memcpy((y),(x),4)

#define from16BEToHost(x,y) memcpy((y),(x),2)
#define from32BEToHost(x,y) memcpy((y),(x),4)

// The host byte order is Big Endian and is not the same as the desired
// Little Endian byte order, so copy and swap.

#define fromHostTo16LE(x,y) ByteSwap::b2((y),(x))
#define fromHostTo32LE(x,y) ByteSwap::b4((y),(x))

#define from16LEToHost(x,y) ByteSwap::b2((y),(x))
#define from32LEToHost(x,y) ByteSwap::b4((y),(x))

# elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

// The host byte order is Little Endian and is the same as the desired
// Little Endian byte order, so copy directly across.

#define fromHostTo16LE(x,y) memcpy((y),(x),2)
#define fromHostTo32LE(x,y) memcpy((y),(x),4)

#define from16LEToHost(x,y) memcpy((y),(x),2)
#define from32LEToHost(x,y) memcpy((y),(x),4)

// The host byte order is Little Endian and is not the same as the desired
// Big Endian byte order, so copy and swap.

#define fromHostTo16BE(x,y) ByteSwap::b2((y),(x))
#define fromHostTo32BE(x,y) ByteSwap::b4((y),(x))

#define from16BEToHost(x,y) ByteSwap::b2((y),(x))
#define from32BEToHost(x,y) ByteSwap::b4((y),(x))

# else
# error "Unrecognized __BYTE_ORDER__ value."
# endif

class ByteSwap {

public:
  static void b2(uint8_t *dst, uint8_t *src);
  static void b4(uint8_t *dst, uint8_t *src);
};

#endif
