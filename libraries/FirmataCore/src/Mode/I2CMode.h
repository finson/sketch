#ifndef I2CMode_h
#define I2CMode_h

#include <FirmataMode.h>
#include <Wire.h>

class I2CModeClass: public FirmataMode {

public:

  I2CModeClass();

  // Firmata access to capabilities

  boolean handleSetPinMode(byte pin, int mode);
  void handleGetCapability(byte pin);

  // Implementation of capabilities

  void reset();
  bool isEnabled();
  void enableI2CPins();
  void disableI2CPins();

  void write8(int addr, uint8_t reg, uint8_t val);
  uint8_t read8(int addr, uint8_t reg);

  void write16(int addr, uint8_t reg, uint16_t val);
  uint16_t read16(int addr, uint8_t reg);

private:

// This 'enabled' flag is a count of the number of unpaired enable calls.
// Calls to enable/disable must be paired if user expects to actually
// disable I2C again at some point after it has been enabled.

  int enabled;

};

extern I2CModeClass I2CMode;

#endif
