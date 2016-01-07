#ifndef CommonStepper_h
#define CommonStepper_h

#include <FirmataMode.h>

/**
 * This abstract class defines the methods that are available to any
 * caller that wishes to control stepper motors.  The concrete subclasses
 * implement the methods as well as they can, depending on what underlying
 * library they use.  Not all libraries can support all the defined methods
 * fully, but the subclasses provide the best implementation they can, given
 * the constraints of their library.
 */

class CommonStepper {

public:

  //----- Motor position (in steps) -----------------------------------------

  virtual void moveTo(long absolute) = 0;  // set target position
  virtual void moveBy(long relative) = 0;  // set target position

  virtual long getCurrentPosition() = 0;
  virtual long getTargetPosition() = 0;
  virtual long getDistanceToGo() = 0;

  //----- Synchronous motor control (blocking) ------------------------------

  virtual void runToPosition() = 0;                  // blocking step control with accel/decel
  virtual void runToNewPosition(long position) = 0;  // blocking step control with accel/decel

  //----- Asynchronous motor control (non-blocking) -------------------------

// FirmataExt calls these methods on short, regular intervals.  If the
// underlying library wants to be called on a microsecond- or millisecond-based
// interval, update() and report() are the places to make such calls in the
// overriding subclass implementation.


/**
 * Called on a microsecond based interval in order to update stepper positions.
 * @param deltaMicros Length, in microseconds, of the interval since the last call to this
 * method.
 */
  virtual void update(unsigned long deltaMicros);

/**
 * Called on a millisecond based interval in order to report stepper completion.
 * @param deltaMillis Length, in milliseconds, of the interval since the last call to this method.
 */
  virtual void report(unsigned long deltaMillis);

  //----- Stop all motors, reset the entire operation -----------------------

  virtual void stop();
  virtual void reset();

  //----- Motor speed and acceleration---------------------------------------

  /**
   * Set the desired constant speed in Revolutions Per Minute (RPM).
   * For a 200-step per revolution motor, typical RPM values are in the
   * range 1..600.  This yields an equivalent SPS range of 3..2000
   * samples per second and step time targets of
   * 1,000,000 .. 500 microseconds per step.
   *
   * @param targetRPM Desired speed in RPM.  Positive is clockwise,
   * negative is counter-clockwise.
   */

  virtual void setRPMSpeed(long targetRPM) = 0;
  virtual long getRPMSpeed() = 0;

  virtual void setMaxRPMSpeed(long maxRPM) = 0;
  virtual long getMaxRPMSpeed() = 0;

  /**
   * Set the desired constant speed in Steps Per Second (SPS).
   * For a 200-step per revolution motor, typical SPS values are in the
   * range 1..2000.  This yields an equivalent RPM range of 0.3 .. 600
   * revolutions per minute and step time targets of
   * 1,000,000..500 microseconds per step.
   *
   * @param targetSPS Desired speed in SPS.  Positive is clockwise,
   * negative is counter-clockwise.
   */

  virtual void setSPSSpeed(long targetSPS) = 0;
  virtual long getSPSSpeed() = 0;

  virtual void setMaxSPSSpeed(long maxSPS) = 0;
  virtual long getMaxSPSSpeed() = 0;

  virtual void setAcceleration(long acceleration) = 0;
  virtual void setDeceleration(long deceleration) = 0;

  virtual void setMinPulseWidth(unsigned int minWidth);

};

  //-------------------------------------------------------------------------

/**
 *  Direction indicator.  Symbolic names for the direction the motor is turning.
 */
enum class Direction {
  CCW = 0,  ///< Counter-Clockwise
  CW  = 1   ///< Clockwise
};

#endif
