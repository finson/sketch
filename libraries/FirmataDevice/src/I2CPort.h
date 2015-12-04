#ifndef I2CPort_h
#define I2CPort_h

#include <Wire.h>
#include <FirmataFeature.h>

class I2CPortClass {

public:

  I2CPortClass();

  bool isEnabled();
  void enableI2CPins();
  void disableI2CPins();

  void write8(int addr, uint8_t reg, uint8_t val);
  uint8_t read8(int addr, uint8_t reg);

  void write16(int addr, uint8_t reg, uint16_t val);
  uint16_t read16(int addr, uint8_t reg);

private:

// Calls to enable/disable must be paired if user expects to actually
// disable I2C again at some point after it has been enabled.

  int enabled;

};

extern I2CPortClass I2CPort;

#endif
