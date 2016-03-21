#include "StepperDriverAsync.h"


/**
 * This class defines a stepper motor device driver using the asynchronous
 * FirmataStepper library that is standard with Firmata.  The library was
 * renamed AsyncStepper for this use but otherwise it is the exact same code
 * as comes standard with Firmata.
 *
 * These are the AsyncStepper library methods available to us for this
 * implementation:
 *
 * AsyncStepper(byte interface, int steps_per_rev, byte pin1, byte pin2,
 *   byte pin3, byte pin4);
 * void setStepsToMove(long steps_to_move, int speed, int accel = 0, int decel = 0);
 * bool update();
 * byte version(void);
 */

//---------------------------------------------------------------------------

StepperDriverAsync::StepperDriverAsync(const char *dName, int addrCount) :
  DeviceDriver(dName, addrCount) {
}

//---------------------------------------------------------------------------

int StepperDriverAsync::open(const char *name, int flags) {
  int lun = DeviceDriver::open(name,flags);
  if (lun < 0) return lun;

  logicalUnits[lun] = new StepperDriverAsyncLUI();
  return lun;
}


int StepperDriverAsync::control(int handle, int reg, int count, byte *buf) {
  StepperDriverAsyncLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
  if (!currentUnit->isOpen()) return ENOTCONN;

  switch (reg) {
  case CDR_Configure: return controlCDR_Configure(handle, reg, count, buf);
  case STP_MoveR:     return controlSTP_MoveR(handle, reg, count, buf);
  case STP_RPMSpeed:  return controlSTP_RPMSpeed(handle, reg, count, buf);
  default:            return ENOTSUP;
  }
}

int StepperDriverAsync::controlCDR_Configure(int handle, int reg, int count, byte *buf) {
  int interface, stepCount, pin1, pin2, pin3, pin4;
  AsyncStepper *motor;

  if (count < 7) return EMSGSIZE;

  StepperDriverAsyncLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
  void *deviceObject = currentUnit->getDeviceObject();
  if (deviceObject != 0) return EBUSY;

  interface = getInt8LE(buf);
  stepCount = getInt16LE(&buf[1]);
  pin1 = getInt16LE(&buf[3]);
  pin2 = getInt16LE(&buf[5]);

  if (interface == static_cast<int>(StepperInterface::EZStepper)) {
    return ENOTSUP;
  } else if (interface == static_cast<int>(StepperInterface::TWO_WIRE)) {
    if (count == 7) {
      motor = new AsyncStepper(AsyncStepper::TWO_WIRE,stepCount, pin1, pin2);
    } else {
      return EMSGSIZE;
    }
  } else if (interface == static_cast<int>(StepperInterface::FOUR_WIRE)) {
    if (count == 11) {
      pin3 = getInt16LE(&buf[7]);
      pin4 = getInt16LE(&buf[9]);
      motor = new AsyncStepper(AsyncStepper::FOUR_WIRE,stepCount, pin1, pin2, pin3, pin4);
    } else {
      return EMSGSIZE;
    }
  } else {
    return ENOTSUP;
  }
  currentUnit->setDeviceObject(motor);
  currentUnit->setRPMSpeed(0);
  currentUnit->setRPMAccelerations(0,0);
  return count;
}

int StepperDriverAsync::controlSTP_RPMSpeed(int handle, int reg, int count, byte *buf) {
  AsyncStepper *motor;
  StepperDriverAsyncLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
  AsyncStepper *deviceObject = static_cast<AsyncStepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) return EBADSLT;
  if (count != 4) return EMSGSIZE;

  currentUnit->setRPMSpeed(getInt32LE(buf));
  return count;
}

int StepperDriverAsync::controlSTP_MoveR(int handle, int reg, int count, byte *buf) {
  AsyncStepper *motor;
  StepperDriverAsyncLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
  AsyncStepper *deviceObject = static_cast<AsyncStepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) return EBADSLT;
  if (count != 5) return EMSGSIZE;

  deviceObject->setStepsToMove((int)getInt32LE(buf),currentUnit->getRPMSpeed(),
    currentUnit->getRPMAcceleration(),currentUnit->getRPMDeceleration());
  return count;
}

//---------------------------------------------------------------------------

int StepperDriverAsync::status(int handle, int reg, int count, byte *buf) {
  StepperDriverAsyncLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
  if (!currentUnit->isOpen()) return ENOTCONN;

  switch (reg) {
  case CDR_DriverVersion: return statusCDR_DriverVersion(handle, reg, count, buf);
  case CDR_LibraryVersion: return statusCDR_LibraryVersion(handle, reg, count, buf);
  default: return ENOTSUP;
  }
}

int StepperDriverAsync::statusCDR_DriverVersion(int handle, int reg, int count, byte *buf) {
  return buildVersionResponse(driverSemVer,driverName,count,buf);
}

int StepperDriverAsync::statusCDR_LibraryVersion(int handle, int reg, int count, byte *buf) {
  AsyncStepper *motor;
  StepperDriverAsyncLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
  AsyncStepper *deviceObject = static_cast<AsyncStepper *>(currentUnit->getDeviceObject());
  if (deviceObject == 0) return EBADSLT;
  int status = buildVersionResponse(librarySemVer,libraryName,count,buf);
  if (status > 2) {
    buf[1] = motor->version();
  }
  return status;
}

//---------------------------------------------------------------------------

int StepperDriverAsync::read(int handle, int count, byte *buf) {return ENOSYS;}
//   StepperLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[handle & 0x7F]);
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

int StepperDriverAsync::write(int handle, int count, byte *buf) {return ENOSYS;}
//   return ENOSYS;
// }

int StepperDriverAsync::close(int lun) {return ENOSYS;}
//   StepperLUI *currentUnit = static_cast<StepperDriverAsyncLUI *>(&logicalUnits[lun & 0x7F]);
//   if (currentUnit->isOpen()) {
//     currentUnit->setOpen(false);
//     I2CMode.disableI2CPins();
//   }
//   return ESUCCESS;
// }


#if 0

//-----Setup------------------------------------------

StepperDriverAsync::StepperDriverAsync(int steps, int p1, int p2):
  Stepper(steps, p1, p2),
  stepsPerRevolution(steps) {}

StepperDriverAsync::StepperDriverAsync(int steps, int p1, int p2, int p3, int p4):
  Stepper(steps, p1, p2, p3, p4),
  stepsPerRevolution(steps) {}

StepperDriverAsync::~StepperDriverAsync() {}

//-----Move the motor (blocking) -----------------------------------------

void StepperDriverAsync::runToPosition() {
  step(theTargetPosition - theCurrentPosition);
  theCurrentPosition += theTargetPosition - theCurrentPosition;
}

void StepperDriverAsync::runToNewPosition(long position) {
  moveTo(position);
  runToPosition();
}

//-----Position (0..number_of_steps-1)-----------------------------------------

void StepperDriverAsync::setCurrentPosition(long position) {
  theCurrentPosition = position % stepsPerRevolution;
}

void StepperDriverAsync::moveTo(long absolute) {
  if (theCurrentPosition != absolute) {
    theTargetPosition = absolute % stepsPerRevolution;
  }
}
void StepperDriverAsync::moveBy(long relative) {
  moveTo(theCurrentPosition + relative);
}

long StepperDriverAsync::getCurrentPosition() {
  return theCurrentPosition;
}

long StepperDriverAsync::getTargetPosition() {
  return theTargetPosition;
}

long StepperDriverAsync::getDistanceToGo() {
  return theTargetPosition - theCurrentPosition;
}


//-----SPS------------------------------------------

/**
 * @see [CommonStepper::setSPSSpeed(long)](@ref CommonStepper#setSPSSpeed(long))


void StepperDriverAsync::setSPSSpeed(long targetSPS) {
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

long StepperDriverAsync::getSPSSpeed() {
  return theSpeed;
}

void StepperDriverAsync::setMaxSPSSpeed(long maxSPS) {
  if (maxSPS == theMaxSpeed) return;

  theMaxSpeed = maxSPS;
  theCmin = 1000000.0 / maxSPS;
  // Recompute theN from current speed and adjust speed if accelerating or cruising
  if (theN > 0) {
    theN = (long)((theSpeed * theSpeed) / (2.0 * theAcceleration)); // Equation 16
  }

}

long StepperDriverAsync::getMaxSPSSpeed() {
  return theMaxSpeed;
}

//-----RPM------------------------------------------

/**
 * @see [CommonStepper::setRPMSpeed(long)](@ref CommonStepper#setRPMSpeed(long))
 */

void StepperDriverAsync::setRPMSpeed(long targetRPM) {
  setSPSSpeed(targetRPM * stepsPerRevolution / 60);
}

long StepperDriverAsync::getRPMSpeed() {
  return theSpeed * 60 / stepsPerRevolution;
}

void StepperDriverAsync::setMaxRPMSpeed(long maxRPM) {
  setMaxSPSSpeed(maxRPM * stepsPerRevolution / 60);
}

long StepperDriverAsync::getMaxRPMSpeed() {
  return theMaxSpeed * 60 / stepsPerRevolution;
}

//----- to be implemented

void StepperDriverAsync::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert) {}
void StepperDriverAsync::setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert) {}
void StepperDriverAsync::setEnablePin(uint8_t enablePinxff) {}

void StepperDriverAsync::disableOutputs() {}
void StepperDriverAsync::enableOutputs() {}

//-----Acceleration / Deceleration (not used by Arduino Stepper) ------------------------------------------

void StepperDriverAsync::setAcceleration(long acceleration) {}
void StepperDriverAsync::setDeceleration(long deceleration) {}

//-----EZ Stepper pulse width (not used by Arduino Stepper) ------------------------------------------

void StepperDriverAsync::setMinPulseWidth(long minWidth) {}

#endif
