#ifndef StepperAPI_h
#define StepperAPI_h

#include <Device/DeviceAPI.h>

/**
 * This class defines the methods that are available to any
 * caller that wishes to control stepper motors.  Not all device driver implementations can
 * support all the defined methods fully, but they provide the
 * best implementation they can, given the constraints of their underlying
 * library.
 */

  /**
   * Set the desired speed in Steps Per Second (SPS).
   * For a 200-step per revolution motor, typical SPS values are in the
   * range 1..2000.  This yields an equivalent RPM range of 0.3 .. 600
   * revolutions per minute and step time targets of
   * 1,000,000..500 microseconds per step.
   *
   * @param targetSPS Desired speed in SPS.  Positive is clockwise,
   * negative is counter-clockwise.
   */

  /**
   * Set the desired speed in Revolutions Per Minute (RPM).
   * For a 200-step per revolution motor, typical RPM values are in the
   * range 1..600.  This yields an equivalent SPS range of 3..2000
   * steps per second and step time targets of
   * 1,000,000 .. 500 microseconds per step.
   *
   * @param targetRPM Desired speed in RPM.  Positive is clockwise,
   * negative is counter-clockwise.
   */


//-------------------------------------------------------------------------

class StepperAPI : public DeviceAPI {

public:

  class Positions;
  class Speeds;
  class Accelerations;
  enum class Dir;

  void getPositions(Positions *pos);

  void moveToAbsoluteTarget(long absoluteSteps, bool block = false);  // start stepper moving
  void moveToRelativeTarget(long deltaSteps, bool block = false);

  void getSPSSpeeds(Speeds *spd);
  void setSPSTargetSpeed(long targetSPS);
  void getSPSAccelerations(Accelerations *acc);
  void setSPSAccelerations(long acceleration, long deceleration);

  void getRPMSpeeds(Speeds *spd);
  void setRPMSpeed(long targetRPM);
  void getRPMAccelerations(Accelerations *acc);
  void setRPMAccelerations(long acceleration, long deceleration);

  void stop();
  void reset();

  void setMinPulseWidth(long minWidth);


};
#endif
