#ifndef PinMode_h
#define PinMode_h

#include <FirmataCore.h>

class PinMode
{
  public:
    virtual void handleGetCapability(byte pin) = 0;
    virtual boolean handleSetPinMode(byte pin, int mode) = 0;
    virtual void reset() = 0;
};

#endif
