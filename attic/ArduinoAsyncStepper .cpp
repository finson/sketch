#include "ArduinoAsyncStepper.h"

/**
 * This class defines a set of methods for controlling a stepper motor, using
 * the non-blocking AsyncStepper library that comes with Firmata.
 *
 * These are the AsyncStepper library methods available to us while implementing the
 * CommonStepper interface:
 *
 * ArduinoAsyncStepper(int steps, int p1, int p2)
 * ArduinoAsyncStepper(int steps, int p1, int p2, int p3, int p4):
 * void setStepsToMove(long steps_to_move, int speed, int accel = 0, int decel = 0);
 * bool update();
 * byte version(void);
 */

  //-----Setup------------------------------------------

  ArduinoAsyncStepper::ArduinoAsyncStepper(int steps, int p1, int p2):
   Stepper(steps, p1, p2),
   stepsPerRevolution(steps) {}

  ArduinoAsyncStepper::ArduinoAsyncStepper(int steps, int p1, int p2, int p3, int p4):
  Stepper(steps, p1, p2, p3, p4),
  stepsPerRevolution(steps) {}

  ArduinoAsyncStepper::~ArduinoAsyncStepper() {}

  //-----Move the motor (blocking) -----------------------------------------

  void ArduinoAsyncStepper::runToPosition() {
    step(theTargetPosition-theCurrentPosition);
    theCurrentPosition += theTargetPosition-theCurrentPosition;
  }

  void ArduinoAsyncStepper::runToNewPosition(long position) {
    moveTo(position);
    runToPosition();
  }

  //-----Position (0..number_of_steps-1)-----------------------------------------

  void ArduinoAsyncStepper::setCurrentPosition(long position) {
    theCurrentPosition = position % stepsPerRevolution;
  }

  void ArduinoAsyncStepper::moveTo(long absolute) {
    if (theCurrentPosition != absolute) {
      theTargetPosition = absolute % stepsPerRevolution;
    }
}
  void ArduinoAsyncStepper::moveBy(long relative) {
    moveTo(theCurrentPosition + relative);
  }

  long ArduinoAsyncStepper::getCurrentPosition() {
    return theCurrentPosition;
  }

  long ArduinoAsyncStepper::getTargetPosition() {
    return theTargetPosition;
  }

  long ArduinoAsyncStepper::getDistanceToGo() {
    return theTargetPosition-theCurrentPosition;
  }


  //-----SPS------------------------------------------

  /**
   * @see [CommonStepper::setSPSSpeed(long)](@ref CommonStepper#setSPSSpeed(long))
   */

 void ArduinoAsyncStepper::setSPSSpeed(long targetSPS) {
    if (targetSPS == theSpeed) return;
    long ts = constrain(targetSPS, -theMaxSpeed, theMaxSpeed);
    if (ts == 0) {
      theStepInterval = 0;
    } else {
      theStepInterval = abs(1000000L / ts);
      theDirection = (ts > 0) ? Direction::CW : Direction::CCW;
    }
    theSpeed = ts;
}

  long ArduinoAsyncStepper::getSPSSpeed() {
    return theSpeed;
  }

  void ArduinoAsyncStepper::setMaxSPSSpeed(long maxSPS) {
    if (maxSPS == theMaxSpeed) return;

    theMaxSpeed = maxSPS;
    theCmin = 1000000.0 / maxSPS;
    // Recompute theN from current speed and adjust speed if accelerating or cruising
    if (theN > 0) {
      theN = (long)((theSpeed * theSpeed) / (2.0 * theAcceleration)); // Equation 16
    }

  }

  long ArduinoAsyncStepper::getMaxSPSSpeed() {
    return theMaxSpeed;
  }

  //-----RPM------------------------------------------

  /**
   * @see [CommonStepper::setRPMSpeed(long)](@ref CommonStepper#setRPMSpeed(long))
   */

  void ArduinoAsyncStepper::setRPMSpeed(long targetRPM) {
    setSPSSpeed(targetRPM * stepsPerRevolution / 60);
  }

  long ArduinoAsyncStepper::getRPMSpeed() {
    return theSpeed * 60 / stepsPerRevolution;
  }

  void ArduinoAsyncStepper::setMaxRPMSpeed(long maxRPM) {
    setMaxSPSSpeed(maxRPM * stepsPerRevolution / 60);
  }

  long ArduinoAsyncStepper::getMaxRPMSpeed() {
    return theMaxSpeed * 60 / stepsPerRevolution;
  }

  //----- to be implemented

  void ArduinoAsyncStepper::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert) {}
  void ArduinoAsyncStepper::setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert) {}
  void ArduinoAsyncStepper::setEnablePin(uint8_t enablePinxff) {}

  void ArduinoAsyncStepper::disableOutputs() {}
  void ArduinoAsyncStepper::enableOutputs() {}

  //-----Acceleration / Deceleration (not used by Arduino Stepper) ------------------------------------------

  void ArduinoAsyncStepper::setAcceleration(long acceleration) {}
  void ArduinoAsyncStepper::setDeceleration(long deceleration) {}

  //-----EZ Stepper pulse width (not used by Arduino Stepper) ------------------------------------------

  void ArduinoAsyncStepper::setMinPulseWidth(long minWidth) {}

