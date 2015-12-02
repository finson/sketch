#ifndef I2CReadWrite_h
#define I2CReadWrite_h

#include <I2CBeginEnd.h>

class I2CReadWrite {

public:

  I2CReadWrite(int addr);
  void setDeviceAddress(int addr);
  int getDeviceAddress();

  void write8(uint8_t reg, uint8_t val);
  uint8_t read8(uint8_t reg);

  void write16(uint8_t reg, uint16_t val);
  uint16_t read16(uint8_t reg);

private:
  I2CBeginEnd i2cEnabler;
  int theDeviceAddress;
};

#endif
