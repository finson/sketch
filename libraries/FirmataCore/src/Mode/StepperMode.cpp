/*
 * Firmata is a generic protocol for communicating with microcontrollers
 * from software on a host computer. It is intended to work with
 * any host computer software package.
 *
 * This StepperMode class and its subclasses are derived from the existing
 * StepperFirmata code, but with the Sysex functionality removed, in order
 * to more cleanly support the FirmataCore/FirmataFeature/FirmataDevice
 * architecture.
 *
 * Doug Johnson Dec 2015
 */


#include <FirmataCore.h>
#include "StepperMode.h"

//---------------------------------------------------------------------------

boolean StepperModeClass::handleSetPinMode(byte pin, int mode)
{
  if (mode == STEPPER) {
    if (IS_PIN_DIGITAL(pin)) {
      digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable PWM
      pinMode(PIN_TO_DIGITAL(pin), OUTPUT);
      return true;
    }
  }
  return false;
}

void StepperModeClass::handleGetCapability(byte pin)
{
  if (IS_PIN_DIGITAL(pin)) {
    Firmata.write(STEPPER);
    Firmata.write(21); //21 bits used for number of steps
  }
}

