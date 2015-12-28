#ifndef StepperMode_h
#define StepperMode_h

#include <FirmataMode.h>

#define MAX_STEPPERS 6 // arbitrary value... may need to adjust

class StepperModeClass: public FirmataMode {

public:

  StepperModeClass();

  // Firmata access to capabilities

  boolean handleSetPinMode(byte pin, int mode);
  void handleGetCapability(byte pin);

  // Implementation of capabilities

  void reset();
  void update();

private:
  // FirmataStepper *stepper[MAX_STEPPERS];
  byte numSteppers;
};

extern StepperModeClass StepperMode;

#endif
