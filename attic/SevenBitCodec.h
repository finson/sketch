/**
 * The signatures defined here are the interface to methods that
 * provide encode / decode from an array of 8-bit binary bytes to a
 * somewhat longer array of bytes containing only 7-bit transmittable
 * values.  Note that some codecs require that the length of the source
 * array be a multiple of some chunk size greater than 1.
 */

#ifndef SEVEN_BIT_CODEC_H
#define SEVEN_BIT_CODEC_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

typedef unsigned char byte;

class SevenBitCodec {
public:
    virtual ~SevenBitCodec() {}

    virtual size_t chunkSize() = 0;
    virtual size_t encodedSize(size_t count8) = 0;
    virtual size_t decodedSize(size_t count7) = 0;

    virtual size_t encode8to7(byte *src, size_t srcSize,  char *dst, size_t dstSize ) = 0;
    virtual size_t decode7to8(char *src, size_t srcSize,  byte *dst, size_t dstSize ) = 0;

};

#endif

