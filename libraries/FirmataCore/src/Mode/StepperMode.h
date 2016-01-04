#ifndef StepperMode_h
#define StepperMode_h

#include <FirmataMode.h>

#define MAX_STEPPERS 6 // arbitrary value... may need to adjust
#
/**
 * This abstract class defines the methods that are available to a device
 * driver that wishes to control stepper motors.  The concrete subclasses
 * implement the methods as well as they can, depending on what underlying
 * library they use.  Not all libraries can support all the defined methods
 * fully, but the subclasses provide the best implementation they can, given
 * the constraints of the library.
 */

class StepperModeClass: public FirmataMode {

public:

  StepperModeClass();

  // Firmata access to capabilities

  virtual boolean handleSetPinMode(byte pin, int mode);
  virtual void handleGetCapability(byte pin);

  // Implementation of capabilities

  virtual void reset() = 0;
  virtual void update() = 0;

// Basic Firmata Stepper library

  virtual void setSpeed(long whatSpeed) = 0;  // set desired constant speed
  virtual void step(int number_of_steps) = 0; // blocking step control at constant speed

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


private:
  // FirmataStepper *stepper[MAX_STEPPERS];
  byte numSteppers;
};

#endif
