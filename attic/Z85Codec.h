/**
 * The signatures defined in SevenBitCodec.h are the interface to methods that
 * provide encode / decode from 8-bit binary bytes to 7-bit transmittable
 * chars.  The initial implementation of this interface is Z85Codec, but
 * any other codec can be substituted if needed to match a particular
 * existing implementation or other constraint.
 */

#ifndef Z85_CODEC_H
#define Z85_CODEC_H

typedef unsigned char byte;

class Z85Codec : public SevenBitCodec {
public:
    virtual ~Z85Codec() {}
    virtual size_t encodedSize(size_t inputByteCount) = 0;
    virtual size_t decodedSize(size_t inputCharCount) = 0;

    virtual size_t encodeBytes(byte *src, size_t srcSize,  char *dst, size_t dstSize ) = 0;
    virtual size_t decodeChars(char *src, size_t srcSize,  byte *dst, size_t dstSize ) = 0;

};

#endif

