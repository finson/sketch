#ifndef ArduinoAsyncStepper_h
#define ArduinoAsyncStepper_h

#include <Arduino.h>
#include "CommonStepper.h"
#include <Stepper.h>
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
class ArduinoAsyncStepper: public CommonStepper, Stepper {

//-----------------------------------------------

public:

  //-----Setup------------------------------------------

  ArduinoAsyncStepper(int steps, int p1, int p2);
  ArduinoAsyncStepper(int steps, int p1, int p2, int p3, int p4);
  ~ArduinoAsyncStepper();

  //-----Move the motor (blocking) -----------------------------------------

  void runToPosition();                  // blocking step control with accel/decel
  void runToNewPosition(long position);  // blocking step control with accel/decel

  //----- implemented

  void setCurrentPosition(long position);
  void moveTo(long absolute);
  void moveBy(long relative);
  long getCurrentPosition();
  long getTargetPosition();
  long getDistanceToGo();

  void setSPSSpeed(long targetSPS);
  long getSPSSpeed();

  void setMaxSPSSpeed(long maxSPS);
  long getMaxSPSSpeed();

  void setRPMSpeed(long targetRPM);
  long getRPMSpeed();

  void setMaxRPMSpeed(long maxRPM);
  long getMaxRPMSpeed();

  //----- Implemented but ignored by Arduino Stepper implementation

  void setAcceleration(long acceleration);
  void setDeceleration(long deceleration);

  void setMinPulseWidth(long minWidth);

  //----- to be implemented

  void setPinsInverted(bool directionInvert = false, bool stepInvert = false, bool enableInvert = false);
  void setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert);
  void setEnablePin(uint8_t enablePinxff);

  void disableOutputs();
  void enableOutputs();

private:

//-----

  /// Number of pins on the stepper motor. Permits 2 or 4. 2 pins is a
  /// bipolar, and 4 pins is a unipolar.
  uint8_t theInterface;          // 0, 1, 2, 4, 8, See MotorInterfaceType

  /// Arduino pin number assignments for the 2 or 4 pins required to interface to the
  /// stepper motor or driver
  uint8_t thePin[4];

  /// Whether thePin is inverted or not
  uint8_t thePinInverted[4];

  /// Number of steps in one revolution

  long stepsPerRevolution;

//-----

  /// The current motor speed in steps per second.  ~1-2000 SPS for 200-step
  /// motor, or 0.3-600 RPM, or 1,000,000 to 500 microseconds per step.
  /// Positive is clockwise.
  long theSpeed;         // in steps per second

  /// The maximum permitted speed in steps per second. Must be > 0.
  long theMaxSpeed;

  /// Current direction motor is spinning in
  Direction theDirection;

//-----

  /// The current interval between steps in microseconds.
  /// 0 means the motor is currently stopped with theSpeed == 0
  unsigned long theStepInterval;

  /// Initial step size in microseconds
  float theC0;

  /// Last step size in microseconds
  float theCn;

  /// Min step size in microseconds based on maxSpeed
  float theCmin; // at max speed

  /// The current absolution position in steps.
  long theCurrentPosition;    // Steps

  /// The target position in steps. The AccelStepper library will move the
  /// motor from theCurrentPosition to theTargetPosition, taking into account
  /// the max speed, acceleration and deceleration
  long theTargetPosition;     // Steps

//----- Not used by Arduino Stepper library

  /// The acceleration to use to accelerate or decelerate the motor in steps
  /// per second per second. Must be > 0
  long theAcceleration;
  long theDeceleration;
  long theSqrt_twoa; // Precomputed sqrt(2*_acceleration)
  long theSqrt_twod; // Precomputed sqrt(2*_deceleration)

  /// The minimum allowed pulse width in microseconds
  unsigned int theMinPulseWidth;

  /// The last step time in microseconds
  unsigned long theLastStepTime;

  /// Is the enable pin inverted?
  bool theEnableInverted;

  /// Enable pin for stepper driver, or 0xFF if unused.
  uint8_t theEnablePin;

  /// The step counter for speed calculations
  long theN;

};

#endif
