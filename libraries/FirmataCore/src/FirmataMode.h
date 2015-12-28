
#ifndef FirmataMode_h
#define FirmataMode_h

#include "FirmataFeature.h"

class FirmataMode: public FirmataFeature
{
public:

  // Firmata access to capabilities

  virtual boolean handleSetPinMode(byte pin, int mode) = 0;
  virtual void handleGetCapability(byte pin) = 0;
  virtual void reset() = 0;

  // Firmata sysex commands are not handled by Mode subclasses.

  boolean handleFeatureSysex(byte command, byte argc, byte *argv) final { return false;}

private:
};

#endif
