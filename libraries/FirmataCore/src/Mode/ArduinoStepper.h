#ifndef ArduinoStepper_h
#define ArduinoStepper_h

#include <CommonStepper.h>
#include <Stepper.h>

/**
 * This class defines a set of methods for controlling a stepper motor, using
 * the basic, blocking, Stepper library the is standard with Arduino.
 */
class ArduinoStepper: public CommonStepper, Stepper {

public:

  //-----------------------------------------------

// Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2);
// Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
//                              int motor_pin_3, int motor_pin_4);
//
// AsyncStepper(byte interface = AsyncStepper::DRIVER, int steps_per_rev = 200,
//                   byte pin1 = 2, byte pin2 = 3, byte pin3 = 4, byte pin4 = 5);
//
// AccelStepper(uint8_t interface = AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);

  ArduinoStepper(int steps, int p1, int p2);
  ArduinoStepper(int steps, int p1, int p2, int p3, int p4);
  ~ArduinoStepper();

  //-----------------------------------------------

  void reset();

// Basic Firmata Stepper library

  void setSpeed(long whatSpeed);  // set desired constant speed
  void step(int number_of_steps); // blocking step control at constant speed

// AsyncStepper library

  void setStepsToMove(long steps_to_move, int speed, int accel, int decel);
  // update the stepper position
  // bool update();

// AccelStepper library

  void stop();

  boolean run();                  // async step control with accel/decel
  boolean runSpeed();             // async step control at constant speed
  boolean runSpeedToPosition();   // async step control at constant speed (calculates direction)

  void    runToPosition();                  // blocking step control with accel/decel
  void    runToNewPosition(long position);  // blocking step control with accel/decel

  void    moveTo(long absolute);  // set target position
  void    moveBy(long relative);  // set target position

  long    getCurrentPosition();
  long    getTargetPosition();
  long    getDistanceToGo();

  void    setSpeed(float speed);
  float   getSpeed();
  void    setMaxSpeed(float speed);
  float   getMaxSpeed();

  void    setAcceleration(float acceleration);
  void    setDeceleration(float deceleration);

  void    setCurrentPosition(long position);
  void    setMinPulseWidth(unsigned int minWidth);

  void    setPinsInverted(bool directionInvert = false, bool stepInvert = false, bool enableInvert = false);
  void    setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert);
  void    setEnablePin(uint8_t enablePinxff);

  void    disableOutputs();
  void    enableOutputs();

private:
    /// Number of pins on the stepper motor. Permits 2 or 4. 2 pins is a
    /// bipolar, and 4 pins is a unipolar.
    uint8_t        theInterface;          // 0, 1, 2, 4, 8, See MotorInterfaceType

    /// Arduino pin number assignments for the 2 or 4 pins required to interface to the
    /// stepper motor or driver
    uint8_t        thePin[4];

    /// Whether thePin is inverted or not
    uint8_t        thePinInverted[4];

    /// The current absolution position in steps.
    long           theCurrentPos;    // Steps

    /// The target position in steps. The AccelStepper library will move the
    /// motor from theCurrentPos to theTargetPos, taking into account the
    /// max speed, acceleration and deceleration
    long           theTargetPos;     // Steps

    /// The current motos speed in steps per second
    /// Positive is clockwise
    float          theSpeed;         // Steps per second

    /// The maximum permitted speed in steps per second. Must be > 0.
    float          theMaxSpeed;

    /// The acceleration to use to accelerate or decelerate the motor in steps
    /// per second per second. Must be > 0
    float          theAcceleration;
    float          theSqrt_twoa; // Precomputed sqrt(2*_acceleration)

    /// The current interval between steps in microseconds.
    /// 0 means the motor is currently stopped with theSpeed == 0
    unsigned long  theStepInterval;

    /// The last step time in microseconds
    unsigned long  theLastStepTime;

    /// The minimum allowed pulse width in microseconds
    unsigned int   theMinPulseWidth;

    /// Is the enable pin inverted?
    bool           theEnableInverted;

    /// Enable pin for stepper driver, or 0xFF if unused.
    uint8_t        theEnablePin;

    /// The step counter for speed calculations
    long theN;

    /// Initial step size in microseconds
    float theC0;

    /// Last step size in microseconds
    float theCn;

    /// Min step size in microseconds based on maxSpeed
    float theCmin; // at max speed

    /// Current direction motor is spinning in
    boolean theDirection; // 1 == CW


};

#endif
