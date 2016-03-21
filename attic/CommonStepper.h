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

  virtual long getCurrentPosition() = 0;
  virtual long getTargetPosition() = 0;
  virtual long getDistanceToGo() = 0;

  //----- Synchronous motor control (blocking) ------------------------------

  virtual void runToPosition() = 0;                  // blocking step control with accel/decel
  virtual void runToNewPosition(long position) = 0;  // blocking step control with accel/decel

  //----- Asynchronous motor control (non-blocking) -------------------------

  virtual void moveTo(long absolute) = 0;  // set target position
  virtual void moveBy(long relative) = 0;  // set target position

/**
 * Called repeatedly by the application program at the expiration of a
 * microsecond based interval to perform real time updates of position and
 * control.
 * @param deltaMicros Length, in microseconds, of the interval since the last call to this
 * method.
 */
  virtual void microsecondUpdate(unsigned long deltaMicros);

/**
 * Called repeatedly by the application program at the expiration of a
 * millisecond based interval to provide an opportunity for reporting and
 * other lower frequency tasks.
 * @param deltaMillis Length, in milliseconds, of the interval since the last call to this method.
 */
  virtual void millisecondReport(unsigned long deltaMillis);

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
