#ifndef FirmataFeature_h
#define FirmataFeature_h

#include <TransportFirmata.h>

class FirmataFeature
{
  public:
    virtual void handleGetCapability(byte pin) = 0;
    virtual boolean handleSetPinMode(byte pin, int mode) = 0;
    virtual boolean handleFeatureSysex(byte command, byte argc, byte* argv) = 0;
    virtual void reset() = 0;
};

#endif
