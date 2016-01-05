#include <ArduinoStepper.h>

/**
 * This class defines a set of methods for controlling a stepper motor, using
 * the basic, blocking, Stepper library the is standard with Arduino.
 *
 * These are the Stepper library methods available to us while implementing the
 * CommonStepper interface.

 * void setSpeed(long whatSpeed);
 * void step(int number_of_steps);   // blocking
 * int version(void);
 *
 */
class ArduinoStepper: public CommonStepper, Stepper {

public:

// Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2);
// Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
//                              int motor_pin_3, int motor_pin_4);
//
// AsyncStepper(byte interface = AsyncStepper::DRIVER, int steps_per_rev = 200,
//                   byte pin1 = 2, byte pin2 = 3, byte pin3 = 4, byte pin4 = 5);
//
// AccelStepper(uint8_t interface = AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);

  ArduinoStepper(int steps, int p1, int p2): Stepper(steps, p1, p2) {}
  ArduinoStepper(int steps, int p1, int p2, int p3, int p4): Stepper(steps, p1, p2, p3, p4) {}
  ~ArduinoStepper() {}

  //-----------------------------------------------

  // void reset();
  /**
   * \see [CommonStepper::reset()](@ref CommonStepper#reset())
   */
  void reset() {

  }

// Basic Firmata Stepper library

  void setSpeed(long whatSpeed);  // set desired constant speed in rpm
  void step(int number_of_steps); // blocking step control at constant speed

// AsyncStepper library

  void setStepsToMove(long steps_to_move, int speed, int accel, int decel);
  // update the stepper position
  // bool update();

// AccelStepper library

  // void stop();

  // boolean run();                  // async step control with accel/decel
  // boolean runSpeed();             // async step control at constant speed
  // boolean runSpeedToPosition();   // async step control at constant speed (calculates direction)

  // void    runToPosition();                  // blocking step control with accel/decel
  // void    runToNewPosition(long position);  // blocking step control with accel/decel

  // void    moveTo(long absolute);  // set target position
  // void    moveBy(long relative);  // set target position

  // long    getCurrentPosition();
  // long    getTargetPosition();
  // long    getDistanceToGo();

  //-----Speed------------------------------------------

  void setSpeed(float speed) {
    if (speed == theSpeed) return;
    speed = constrain(speed, -theMaxSpeed, theMaxSpeed);
    if (speed == 0.0) {  //======================================NO!
      theStepInterval = 0;
    } else {
      theStepInterval = fabs(1000000.0 / speed);
      theDirection = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    theSpeed = speed;
  }

  float getSpeed() {
    return theSpeed;
  }

  //-----MaxSpeed------------------------------------------

  void setMaxSpeed(float speed) {
    if (theMaxSpeed != speed) {
      theMaxSpeed = speed;
      theCmin = 1000000.0 / speed;
      // Recompute theN from current speed and adjust speed if accelerating or cruising
      if (theN > 0) {
        theN = (long)((theSpeed * theSpeed) / (2.0 * theAcceleration)); // Equation 16
        computeNewSpeed();
      }
    }
  }

  float   getMaxSpeed() {
    return theMaxSpeed;
  }

  //-----------------------------------------------

  // void    setAcceleration(float acceleration);
  // void    setDeceleration(float deceleration);

  // void    setCurrentPosition(long position);
  // void    setMinPulseWidth(unsigned int minWidth);

  // void    setPinsInverted(bool directionInvert = false, bool stepInvert = false, bool enableInvert = false);
  // void    setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert);
  // void    setEnablePin(uint8_t enablePinxff);

  // void    disableOutputs();
  // void    enableOutputs();

};

#endif
