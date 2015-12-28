/*
 * Firmata is a generic protocol for communicating with microcontrollers
 * from software on a host computer. It is intended to work with
 * any host computer software package.
 *
 * This StepperMode module is derived from the existing StepperFirmata code,
 * but with the Sysex functionality removed, in order to more cleanly support
 * the FirmataCore/FirmataFeature/FirmataDevice architecture.
 *
 * Doug Johnson Dec 2015
 *
 * Formatted using the GNU C formatting and indenting
 */


#include <FirmataCore.h>
#include "StepperMode.h"
// #include "utility/FirmataStepper.h"

StepperModeClass::StepperModeClass() {

}

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

void StepperModeClass::reset()
{
  for (byte i = 0; i < MAX_STEPPERS; i++) {
    // if (stepper[i]) {
    //   free(stepper[i]);
    //   stepper[i] = 0;
    // }
  }
  numSteppers = 0;
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void StepperModeClass::update()
{
  // if (numSteppers > 0) {
  //   // if one or more stepper motors are used, update their position
  //   for (byte i = 0; i < MAX_STEPPERS; i++) {
  //     if (stepper[i]) {
  //       bool done = stepper[i]->update();
  //       // send command to client application when stepping is complete
  //       if (done) {
  //         Firmata.write(START_SYSEX);
  //         Firmata.write(STEPPER_DATA);
  //         Firmata.write(i);
  //         Firmata.write(END_SYSEX);
  //       }
  //     }
  //   }
  // }
}

// make one instance to coordinate enable/disable and IO across the app.

StepperModeClass StepperMode;
