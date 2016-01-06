#ifndef StepperMode_h
#define StepperMode_h

#include <FirmataMode.h>

class StepperModeClass: public FirmataMode {

public:

  // Firmata access to capabilities

  boolean handleSetPinMode(byte pin, int mode);
  void handleGetCapability(byte pin);

};

#endif
