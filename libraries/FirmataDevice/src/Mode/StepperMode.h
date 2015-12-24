
#ifndef StepperMode_h
#define StepperMode_h

#include <FirmataCore.h>
#include "FirmataFeature.h"

#define MAX_STEPPERS 6 // arbitrary value... may need to adjust

class StepperModeClass: public FirmataFeature
{
  public:

  StepperModeClass();

  // External access to capabilities

    boolean handleSetPinMode(byte pin, int mode);
    void handleGetCapability(byte pin);
    void reset();

    boolean handleFeatureSysex(byte command, byte argc, byte *argv) { return false;}

  // Implementation of capabilities

    void update();

  private:
    // FirmataStepper *stepper[MAX_STEPPERS];
    byte numSteppers;
};

extern StepperModeClass StepperMode;

#endif
