#include "StepperAPI.h"

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

class StepperAPI::Positions {
  long target;
  long current;
};

class StepperAPI::Speeds {
  long target;
  long current;
};

class StepperAPI::Accelerations {
  long accel;
  long decel;
};

enum class StepperAPI::Dir {
  CCW = 0,  ///< Counter-Clockwise
  CW  = 1   ///< Clockwise
};

void StepperAPI::getPositions(Positions *pos) {}

void StepperAPI::moveToAbsoluteTarget(long absoluteSteps, bool block) {} // start stepper moving
void StepperAPI::moveToRelativeTarget(long deltaSteps, bool block) {}

void StepperAPI::getSPSSpeeds(Speeds *spd) {}
void StepperAPI::setSPSTargetSpeed(long targetSPS) {}

void StepperAPI::getSPSAccelerations(Accelerations *acc) {}
void StepperAPI::setSPSAccelerations(long acceleration, long deceleration) {}
void StepperAPI::getRPMSpeeds(Speeds *spd) {}
void StepperAPI::setRPMSpeed(long targetRPM) {}

void StepperAPI::getRPMAccelerations(Accelerations *acc) {}
void StepperAPI::setRPMAccelerations(long acceleration, long deceleration) {}

void StepperAPI::stop() {}
void StepperAPI::reset() {}

void StepperAPI::setMinPulseWidth(long minWidth) {}
