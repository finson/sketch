#ifndef PinMessages_h
#define PinMessages_h

#include <FirmataCore.h>

class PinMessages
{
  public:
    virtual boolean handleFeatureSysex(byte command, byte argc, byte* argv) = 0;
};

#endif
