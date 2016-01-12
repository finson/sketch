#include "StepperDriverBasic.h"

/**
 * This class defines a stepper motor device driver, using
 * the basic blocking Stepper library that is standard with Arduino.
 *
 * These are the Stepper library methods available to us for this implementation.
 *
 * Stepper(steps, p1, p2)
 * Stepper(steps, p1, p2, p3, p4)
 * void setSpeed(long whatSpeed);
 * void step(int number_of_steps);   // blocking
 * int version(void);
 */

StepperDriverBasic::StepperDriverBasic(const char *dName, int addrCount) :
  StepperDriver(dName, addrCount) {

}

int StepperDriverBasic::control(int handle, int reg, int count, byte *buf) {
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  if (!currentUnit->isOpen()) {
    return ENOTCONN;
  }

  switch (reg) {
  case CCR_Configure: return controlCCR_Configure(handle, reg, count, buf);
  case STP_MoveR:     return controlSTP_MoveR(handle, reg, count, buf);
  case STP_RPMSpeed:  return controlSTP_RPMSpeed(handle, reg, count, buf);
  default:            return ENOTSUP;
  }
}

int StepperDriverBasic::controlCCR_Configure(int handle, int reg, int count, byte *buf) {
  int stepCount, pin1, pin2, pin3, pin4;
  Stepper *motor;

  if ((count != 6) && (count != 10)) {
    return EMSGSIZE;
  }

  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];

  void *deviceObject = currentUnit->getDeviceObject();
  if (deviceObject != 0) {
    return EBUSY;
  }

  stepCount = getInt16(buf[0], buf[1]);
  pin1 = getInt16(buf[2], buf[3]);
  pin2 = getInt16(buf[4], buf[5]);
  if (count == 6) {
    motor = new Stepper(stepCount, pin1, pin2);
  } else if (count == 10) {
    pin3 = getInt16(buf[6], buf[7]);
    pin4 = getInt16(buf[8], buf[9]);
    motor = new Stepper(stepCount, pin1, pin2, pin3, pin4);
  }
  currentUnit->setDeviceObject(motor);
  return count;
}

int StepperDriverBasic::controlSTP_MoveR(int handle, int reg, int count, byte *buf) {
  Stepper *motor;
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];

  Stepper *deviceObject = static_cast<Stepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) {
    return EBADSLT;
  }

  if (count != 3) {
    return EMSGSIZE;
  }

  deviceObject->step(getInt16(buf[0],buf[1]));
  return count;
}

int StepperDriverBasic::controlSTP_RPMSpeed(int handle, int reg, int count, byte *buf) {
  Stepper *motor;
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];

  Stepper *deviceObject = static_cast<Stepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) {
    return EBADSLT;
  }

  if (count != 2) {
    return EMSGSIZE;
  }

  deviceObject->setSpeed(getInt16(buf[0],buf[1]));
  return count;
}


int StepperDriverBasic::status(int handle, int reg, int count, byte *buf) {return ENOSYS;}
//   StepperLUI *currentUnit = &logicalUnits[handle & 0x7F];
//   if (!currentUnit->isOpen()) {
//     return ENOTCONN;
//   }
//   if (static_cast<StepperRegister>(reg) == StepperRegister::RESOLUTION) {
//     if (count != 1) {
//       return EMSGSIZE;
//     }
//   } else if (count != 2) {
//     return EMSGSIZE;
//   }

//   int address = currentUnit->getDeviceAddress();
//   if (count == 1) {
//     buf[0] = I2CMode.read8(address, reg);
//   } else {
//     int v = I2CMode.read16(address, reg);
//     buf[0] = (v >> 8) & 0xFF;
//     buf[1] = v & 0xFF;
//   }
//   return count;
// }

int StepperDriverBasic::read(int handle, int count, byte *buf) {return ENOSYS;}
//   StepperLUI *currentUnit = &logicalUnits[handle & 0x7F];
//   if (!currentUnit->isOpen()) {
//     return ENOTCONN;
//   }

//   if (count != 2) {
//     return EMSGSIZE;
//   }

//   int address = currentUnit->getDeviceAddress();
//   int reg = static_cast<uint8_t>(StepperRegister::AMBIENT_TEMP);
//   int v = I2CMode.read16(address, reg);
//   buf[0] = (v >> 8) & 0xFF;
//   buf[1] = v & 0xFF;

//   return count;
// }

int StepperDriverBasic::write(int handle, int count, byte *buf) {return ENOSYS;}
//   return ENOSYS;
// }

int StepperDriverBasic::close(int lun) {return ENOSYS;}
//   StepperLUI *currentUnit = &logicalUnits[lun & 0x7F];
//   if (currentUnit->isOpen()) {
//     currentUnit->setOpen(false);
//     I2CMode.disableI2CPins();
//   }
//   return ESUCCESS;
// }


#if 0

//-----Setup------------------------------------------

StepperDriverBasic::StepperDriverBasic(int steps, int p1, int p2):
  Stepper(steps, p1, p2),
  stepsPerRevolution(steps) {}

StepperDriverBasic::StepperDriverBasic(int steps, int p1, int p2, int p3, int p4):
  Stepper(steps, p1, p2, p3, p4),
  stepsPerRevolution(steps) {}

StepperDriverBasic::~StepperDriverBasic() {}

//-----Move the motor (blocking) -----------------------------------------

void StepperDriverBasic::runToPosition() {
  step(theTargetPosition - theCurrentPosition);
  theCurrentPosition += theTargetPosition - theCurrentPosition;
}

void StepperDriverBasic::runToNewPosition(long position) {
  moveTo(position);
  runToPosition();
}

//-----Position (0..number_of_steps-1)-----------------------------------------

void StepperDriverBasic::setCurrentPosition(long position) {
  theCurrentPosition = position % stepsPerRevolution;
}

void StepperDriverBasic::moveTo(long absolute) {
  if (theCurrentPosition != absolute) {
    theTargetPosition = absolute % stepsPerRevolution;
  }
}
void StepperDriverBasic::moveBy(long relative) {
  moveTo(theCurrentPosition + relative);
}

long StepperDriverBasic::getCurrentPosition() {
  return theCurrentPosition;
}

long StepperDriverBasic::getTargetPosition() {
  return theTargetPosition;
}

long StepperDriverBasic::getDistanceToGo() {
  return theTargetPosition - theCurrentPosition;
}


//-----SPS------------------------------------------

/**
 * @see [CommonStepper::setSPSSpeed(long)](@ref CommonStepper#setSPSSpeed(long))
 */

void StepperDriverBasic::setSPSSpeed(long targetSPS) {
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

long StepperDriverBasic::getSPSSpeed() {
  return theSpeed;
}

void StepperDriverBasic::setMaxSPSSpeed(long maxSPS) {
  if (maxSPS == theMaxSpeed) return;

  theMaxSpeed = maxSPS;
  theCmin = 1000000.0 / maxSPS;
  // Recompute theN from current speed and adjust speed if accelerating or cruising
  if (theN > 0) {
    theN = (long)((theSpeed * theSpeed) / (2.0 * theAcceleration)); // Equation 16
  }

}

long StepperDriverBasic::getMaxSPSSpeed() {
  return theMaxSpeed;
}

//-----RPM------------------------------------------

/**
 * @see [CommonStepper::setRPMSpeed(long)](@ref CommonStepper#setRPMSpeed(long))
 */

void StepperDriverBasic::setRPMSpeed(long targetRPM) {
  setSPSSpeed(targetRPM * stepsPerRevolution / 60);
}

long StepperDriverBasic::getRPMSpeed() {
  return theSpeed * 60 / stepsPerRevolution;
}

void StepperDriverBasic::setMaxRPMSpeed(long maxRPM) {
  setMaxSPSSpeed(maxRPM * stepsPerRevolution / 60);
}

long StepperDriverBasic::getMaxRPMSpeed() {
  return theMaxSpeed * 60 / stepsPerRevolution;
}

//----- to be implemented

void StepperDriverBasic::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert) {}
void StepperDriverBasic::setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert) {}
void StepperDriverBasic::setEnablePin(uint8_t enablePinxff) {}

void StepperDriverBasic::disableOutputs() {}
void StepperDriverBasic::enableOutputs() {}

//-----Acceleration / Deceleration (not used by Arduino Stepper) ------------------------------------------

void StepperDriverBasic::setAcceleration(long acceleration) {}
void StepperDriverBasic::setDeceleration(long deceleration) {}

//-----EZ Stepper pulse width (not used by Arduino Stepper) ------------------------------------------

void StepperDriverBasic::setMinPulseWidth(long minWidth) {}

#endif
