#ifndef I2CPort_h
#define I2CPort_h

#include "Port.h"
#include <Wire.h>

#define I2C_MIN_7BIT_ADDRESS 0x8
#define I2C_MAX_7BIT_ADDRESS 0x77

class I2CPort : public Port {

public:

  I2CPort();
  ~I2CPort();

  void writeUInt8(int addr, uint8_t reg, uint8_t value);
  void writeUInt16LE(int addr, uint8_t reg, uint16_t value);
  void writeUInt16BE(int addr, uint8_t reg, uint16_t value);
  void writeUInt32LE(int addr, uint8_t reg, uint32_t value);
  void writeUInt32BE(int addr, uint8_t reg, uint32_t value);

  uint8_t readUInt8(int addr, uint8_t reg);
  uint16_t readUInt16LE(int addr, uint8_t reg);
  uint16_t readUInt16BE(int addr, uint8_t reg);
  uint32_t readUInt32LE(int addr, uint8_t reg);
  uint32_t readUInt32BE(int addr, uint8_t reg);

private:
  void read2(int addr, uint8_t reg, uint8_t *bytesRead);
  void read4(int addr, uint8_t reg, uint8_t *bytesRead);
};

#endif
