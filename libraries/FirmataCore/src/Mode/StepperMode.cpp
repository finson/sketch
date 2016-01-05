/*
 * Firmata is a generic protocol for communicating with microcontrollers
 * from software on a host computer. It is intended to work with
 * any host computer software package.
 *
 * This StepperMode class and its subclasses are derived from the existing
 * StepperFirmata code, but with the Sysex decode/encode removed in order
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

//---------------------------------------------------------------------------


StepperModeClass::StepperModeClass(CommonStepper m){

}

    byte stepCommand, deviceNum, directionPin, stepPin, stepDirection, interface;
    byte motorPin3, motorPin4;
    unsigned int stepsPerRev;
    long numSteps;
    int stepSpeed;
    int accel;
    int decel;

      // need: interface,stepsPerRev (?), directionPin (pin1),stepPin (pin2), pin3, pin4
      // enable,

//      if (stepCommand == STEPPER_CONFIG)

      void addStepper(StepperMotor m) {
        interface = argv[2];
        stepsPerRev = (argv[3] + (argv[4] << 7));

        directionPin = argv[5]; // or motorPin1 for TWO_WIRE or FOUR_WIRE interface
        stepPin = argv[6]; // // or motorPin2 for TWO_WIRE or FOUR_WIRE interface
        if (Firmata.getPinMode(directionPin) == IGNORE || Firmata.getPinMode(stepPin) == IGNORE)
          return false;
        Firmata.setPinMode(directionPin, STEPPER);
        Firmata.setPinMode(stepPin, STEPPER);
        if (!stepper[deviceNum]) {
          numSteppers++;
        }
        if (interface == FirmataStepper::DRIVER || interface == FirmataStepper::TWO_WIRE) {
          stepper[deviceNum] = new FirmataStepper(interface, stepsPerRev, directionPin, stepPin);
        } else if (interface == FirmataStepper::FOUR_WIRE) {
          motorPin3 = argv[7];
          motorPin4 = argv[8];
          if (Firmata.getPinMode(motorPin3) == IGNORE || Firmata.getPinMode(motorPin4) == IGNORE)
            return false;
          Firmata.setPinMode(motorPin3, STEPPER);
          Firmata.setPinMode(motorPin4, STEPPER);
          stepper[deviceNum] = new FirmataStepper(interface, stepsPerRev, directionPin, stepPin, motorPin3, motorPin4);
        }
      }

//    if (stepCommand == STEPPER_STEP)
//    setStepsToMove(numSteps, stepSpeed, accel, decel);

      void step(int n) {
        stepDirection = argv[2];
        numSteps = (long)argv[3] | ((long)argv[4] << 7) | ((long)argv[5] << 14);
        stepSpeed = (argv[6] + (argv[7] << 7));

        if (stepDirection == 0) { numSteps *= -1; }
        if (stepper[deviceNum]) {
          if (argc >= 8 && argc < 12) {
            // num steps, speed (0.01*rad/sec)
            stepper[deviceNum]->setStepsToMove(numSteps, stepSpeed);
          } else if (argc == 12) {
            accel = (argv[8] + (argv[9] << 7));
            decel = (argv[10] + (argv[11] << 7));
            // num steps, speed (0.01*rad/sec), accel (0.01*rad/sec^2), decel (0.01*rad/sec^2)
            stepper[deviceNum]->setStepsToMove(numSteps, stepSpeed, accel, decel);
          }
        }
      }


/*==============================================================================
 * SETUP()
 *============================================================================*/

void StepperMode::reset()
{
  for (byte i = 0; i < MAX_STEPPERS; i++) {
    if (stepper[i]) {
      free(stepper[i]);
      stepper[i] = 0;
    }
  }
  numSteppers = 0;
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void StepperMode::update()
{
  if (numSteppers > 0) {
    // if one or more stepper motors are used, update their position
    for (byte i = 0; i < MAX_STEPPERS; i++) {
      if (stepper[i]) {
        bool done = stepper[i]->update();
        // send command to client application when stepping is complete
        if (done) {
          Firmata.write(START_SYSEX);
          Firmata.write(STEPPER_DATA);
          Firmata.write(i);
          Firmata.write(END_SYSEX);
        }
      }
    }
  }
}


