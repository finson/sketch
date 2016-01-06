#include "ArduinoStepper.h"

/**
 * This class defines a set of methods for controlling a stepper motor, using
 * the basic blocking Stepper library the is standard with Arduino.
 *
 * These are the Stepper library methods available to us while implementing the
 * CommonStepper interface.

 * void setSpeed(long whatSpeed);
 * void step(int number_of_steps);   // blocking
 * int version(void);
 *
 */

  //-----Setup------------------------------------------

  ArduinoStepper::ArduinoStepper(int steps, int p1, int p2): Stepper(steps, p1, p2) {}
  ArduinoStepper::ArduinoStepper(int steps, int p1, int p2, int p3, int p4): Stepper(steps, p1, p2, p3, p4) {}
  ArduinoStepper::~ArduinoStepper() {}

  //-----Move the motor (blocking) -----------------------------------------

  void ArduinoStepper::runToPosition() {
    step(theTargetPosition-step_number);
  }

  void ArduinoStepper::runToNewPosition(long position) {
    moveTo(position);
    runToPosition();
  }

  //-----Position (0..number_of_steps-1)-----------------------------------------

  void ArduinoStepper::setCurrentPosition(long position) {
    step_number = position % number_of_steps;
  }

  void ArduinoStepper::moveTo(long absolute) {
    if (step_number != absolute) {
      theTargetPosition = absolute % number_of_steps;
    }
}
  void ArduinoStepper::moveBy(long relative) {
    moveTo(step_number + relative);
  }

  long ArduinoStepper::getCurrentPosition() {
    return step_number;
  }

  long ArduinoStepper::getTargetPosition() {
    return theTargetPosition;
  }

  long ArduinoStepper::getDistanceToGo() {
    return theTargetPosition-step_number;
  }


  //-----SPS------------------------------------------

  /**
   * @see [CommonStepper::setSPSSpeed(long)](@ref CommonStepper#setSPSSpeed(long))
   */

 void ArduinoStepper::setSPSSpeed(long targetSPS) {
    if (targetSPS == theSpeed) return;
    long ts = constrain(targetSPS, -theMaxSpeed, theMaxSpeed);
    if (ts == 0) {
      theStepInterval = 0;
    } else {
      theStepInterval = abs(1000000L / ts);
      theDirection = (ts > 0) ? Direction.CW : Direction.CCW;
    }
    theSpeed = ts;
}

  long ArduinoStepper::getSPSSpeed() {
    return theSpeed;
  }

  void ArduinoStepper::setMaxSPSSpeed(long maxSPS) {
    if (maxSPS == theMaxSpeed) return;

    theMaxSpeed = maxSPS;
    theCmin = 1000000.0 / maxSPS;
    // Recompute theN from current speed and adjust speed if accelerating or cruising
    if (theN > 0) {
      theN = (long)((theSpeed * theSpeed) / (2.0 * theAcceleration)); // Equation 16
    }

  }

  long ArduinoStepper::getMaxSPSSpeed() {
    return theMaxSpeed;
  }

  //-----RPM------------------------------------------

  /**
   * @see [CommonStepper::setRPMSpeed(long)](@ref CommonStepper#setRPMSpeed(long))
   */

  void ArduinoStepper::setRPMSpeed(long targetRPM) {
    setSPSSpeed(targetRPM * number_of_steps / 60);
  }

  long ArduinoStepper::getRPMSpeed() {
    return theSpeed * 60 / number_of_steps;
  }

  void ArduinoStepper::setMaxRPMSpeed(long maxRPM) {
    setMaxSPSSpeed(maxRPM * number_of_steps / 60);
  }

  long ArduinoStepper::getMaxRPMSpeed() {
    return theMaxSpeed * 60 / number_of_steps;
  }

  //----- to be implemented

  void ArduinoStepper::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert) {}
  void ArduinoStepper::setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert) {}
  void ArduinoStepper::setEnablePin(uint8_t enablePinxff) {}

  void ArduinoStepper::disableOutputs() {}
  void ArduinoStepper::enableOutputs() {}

  //-----Acceleration / Deceleration (not used by Arduino Stepper) ------------------------------------------

  void ArduinoStepper::setAcceleration(long acceleration) {}
  void ArduinoStepper::setDeceleration(long deceleration) {}

  //-----EZ Stepper pulse width (not used by Arduino Stepper) ------------------------------------------

  void ArduinoStepper::setMinPulseWidth(long minWidth) {}

