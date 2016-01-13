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

//---------------------------------------------------------------------------

StepperDriverBasic::StepperDriverBasic(const char *dName, int addrCount) :
  StepperDriver(dName, addrCount) {
}

//---------------------------------------------------------------------------

int StepperDriverBasic::control(int handle, int reg, int count, byte *buf) {
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  if (!currentUnit->isOpen()) return ENOTCONN;

  switch (reg) {
  case CDR_Configure: return controlCDR_Configure(handle, reg, count, buf);
  case STP_MoveR:     return controlSTP_MoveR(handle, reg, count, buf);
  case STP_RPMSpeed:  return controlSTP_RPMSpeed(handle, reg, count, buf);
  default:            return ENOTSUP;
  }
}

int StepperDriverBasic::controlCDR_Configure(int handle, int reg, int count, byte *buf) {
  int interface, stepCount, pin1, pin2, pin3, pin4;
  Stepper *motor;

  if (count < 7) return EMSGSIZE;

  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  void *deviceObject = currentUnit->getDeviceObject();
  if (deviceObject != 0) return EBUSY;

  interface = getInt8LE(buf);
  if (interface == 0) return ENOTSUP;

  stepCount = getInt16LE(&buf[1]);
  pin1 = getInt16LE(&buf[3]);
  pin2 = getInt16LE(&buf[5]);

  if (interface == 1) {
    if (count == 7) {
      motor = new Stepper(stepCount, pin1, pin2);
    } else {
      return EMSGSIZE;
    }
  } else if (interface == 2) {
    if (count == 11) {
      pin3 = getInt16LE(&buf[7]);
      pin4 = getInt16LE(&buf[9]);
      motor = new Stepper(stepCount, pin1, pin2, pin3, pin4);
    } else {
      return EMSGSIZE;
    }
  } else {
    return ENOTSUP;
  }

  currentUnit->setDeviceObject(motor);
  return count;
}

int StepperDriverBasic::controlSTP_MoveR(int handle, int reg, int count, byte *buf) {
  Stepper *motor;
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  Stepper *deviceObject = static_cast<Stepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) return EBADSLT;
  if (count != 5) return EMSGSIZE;

  deviceObject->step((int)getInt32LE(buf));
  return count;
}

int StepperDriverBasic::controlSTP_RPMSpeed(int handle, int reg, int count, byte *buf) {
  Stepper *motor;
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  Stepper *deviceObject = static_cast<Stepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) return EBADSLT;
  if (count != 4) return EMSGSIZE;

  deviceObject->setSpeed(getInt16LE(buf));
  return count;
}

//---------------------------------------------------------------------------

int StepperDriverBasic::status(int handle, int reg, int count, byte *buf) {
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  if (!currentUnit->isOpen()) return ENOTCONN;

  switch (reg) {
  case CDR_DriverVersion: return statusCDR_DriverVersion(handle, reg, count, buf);
  default:                return ENOTSUP;
  }
}

int StepperDriverBasic::statusCDR_DriverVersion(int handle, int reg, int count, byte *buf) {
  Stepper *motor;
  LogicalUnitInfo *currentUnit = &logicalUnits[handle & 0x7F];
  Stepper *deviceObject = static_cast<Stepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) return EBADSLT;
  if (count < (2 + (VERSION_PACKET_COUNT*VERSION_PACKET_SIZE))) return EMSGSIZE;

  int byteIndex = 0;
  buf[byteIndex++] = VERSION_PACKET_COUNT;
  buf[byteIndex++] = VERSION_PACKET_SIZE;

  // Device Driver version (packet 1)

  for (int idx=0; idx<VERSION_PACKET_SIZE; idx++) {
    buf[byteIndex++] = pgm_read_byte_near(&stepperDriverBasicSemVer[idx]);
  }

  // Stepper library version (packet 2)

  buf[byteIndex++] = (uint8_t)(deviceObject->version());
  for (int idx=1;idx<VERSION_PACKET_SIZE;idx++) {
    buf[byteIndex++] = 0;
  }

  return byteIndex;
}

//---------------------------------------------------------------------------

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
