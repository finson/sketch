#ifndef StepperMode_h
#define StepperMode_h

#include <FirmataMode.h>
#include <CommonStepper.h>

/**
 * This abstract class defines the methods that are available to a device
 * driver that wishes to control stepper motors.  The concrete subclasses
 * implement the methods as well as they can, depending on what underlying
 * library they use.  Not all libraries can support all the defined methods
 * fully, but the subclasses provide the best implementation they can, given
 * the constraints of their library.
 */

class StepperModeClass: public FirmataMode {

public:

  // Firmata access to capabilities

  boolean handleSetPinMode(byte pin, int mode);
  void handleGetCapability(byte pin);

  // Implementation of capabilities

  void addStepper(CommonStepper motor);

  void step(int n);
  void reset();
  void update()


  //-----------------------------------------------

/**
 * Called on a microsecond based interval in order to update stepper positions.
 * @param deltaMicros Length, in microseconds, of the interval since the last call to this
 * method.
 */
  virtual void update(unsigned long deltaMicros) = 0;
/**
 * Called on a millisecond based interval in order to report stepper completion.
 * @param deltaMillis Length, in milliseconds, of the interval since the last call to this method.
 */
  virtual void report(unsigned long deltaMillis) {};

// Basic Firmata Stepper library

  virtual void setSpeed(long whatSpeed) = 0;  // set desired constant speed
  virtual void step(int number_of_steps) = 0; // blocking step control at constant speed

// AsyncStepper library

  virtual void setStepsToMove(long steps_to_move, int speed, int accel = 0, int decel = 0);
  // update the stepper position
  // virtual bool update();

// AccelStepper library

  virtual void stop() = 0;

  virtual boolean run() = 0;                  // async step control with accel/decel
  virtual boolean runSpeed() = 0;             // async step control at constant speed
  virtual boolean runSpeedToPosition() = 0;   // async step control at constant speed (calculates direction)

  virtual void    runToPosition() = 0;                  // blocking step control with accel/decel
  virtual void    runToNewPosition(long position) = 0;  // blocking step control with accel/decel

  virtual void    moveTo(long absolute) = 0;  // set target position
  virtual void    moveBy(long relative) = 0;  // set target position

  virtual long    getCurrentPosition() = 0;
  virtual long    getTargetPosition() = 0;
  virtual long    getDistanceToGo() = 0;

  virtual void    setSpeed(float speed) = 0;
  virtual float   getSpeed() = 0;
  virtual void    setMaxSpeed(float speed) = 0;
  virtual float   getMaxSpeed() = 0;

  virtual void    setAcceleration(float acceleration) = 0;
  virtual void    setDeceleration(float deceleration) = 0;

  virtual void    setCurrentPosition(long position) = 0;
  virtual void    setMinPulseWidth(unsigned int minWidth) = 0;

  virtual void    setPinsInverted(bool directionInvert = false, bool stepInvert = false, bool enableInvert = false) = 0;
  virtual void    setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert) = 0;
  virtual void    setEnablePin(uint8_t enablePin = 0xff) = 0;

  virtual void    disableOutputs() = 0;
  virtual void    enableOutputs() = 0;

};

#endif
