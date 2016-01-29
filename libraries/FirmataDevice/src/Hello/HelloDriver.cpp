#include "HelloDriver.h"

/**
 * This class defines a simple device driver as a sort of
 * HelloWorld component for device drivers and their usage.
 */

DEFINE_SEMVER(HelloDriver, 0, 1, 0)

//---------------------------------------------------------------------------

HelloDriver::HelloDriver(const char *dName, int count) :
  DeviceDriver(dName,count) {}

//---------------------------------------------------------------------------

int HelloDriver::open(const char *name, int flags) {
  int lun;
  int status = DeviceDriver::open(name,flags);
  if (status < 0) {
    return status;
  }

  lun = status;
  HelloLUI *currentUnit = new HelloLUI("World");

  // Any further validation of the current unit's appropriateness goes here ...

  logicalUnits[lun] = currentUnit;
  return lun;
}

/**
 * Read a status register on the device.
 */
int HelloDriver::status(int handle, int reg, int count, byte *buf) {

  HelloLUI *currentUnit = static_cast<HelloLUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  case static_cast<int>(CDR::DriverVersion):
    return DeviceDriver::buildVersionResponse(releaseVersion, scopeName, count, buf);

  case static_cast<int>(CDR::Debug):
    return statusCDR_Debug(handle, reg, count, buf);

  case static_cast<int>(HelloRegister::INTERJECTION):
    strlcpy(buf,currentUnit->getWhat(),count);
    return strlen(buf);

  case static_cast<int>(HelloRegister::OBJECT):
    strlcpy(buf,currentUnit->getWho(),count);
    return strlen(buf);

  default:
    return ENOTSUP;
  }
}

int HelloDriver::control(int handle, int reg, int count, byte *buf) {

  HelloLUI *currentUnit = static_cast<HelloLUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  case static_cast<int>(CDR::Debug):
    return controlCDR_Debug(handle, reg, count, buf);

  case static_cast<int>(HelloRegister::INTERJECTION):
    currentUnit->setWhat(buf);
    return strlen(buf);

  case static_cast<int>(HelloRegister::OBJECT):
    currentUnit->setWho(buf);
    return strlen(buf);

  default:
    return ENOTSUP;
  }
  return EPANIC;
}

int HelloDriver::read(int handle, int count, byte *buf) {
  HelloLUI *currentUnit = static_cast<HelloLUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  if (count < (strlen(currentUnit->getWho())+strlen(currentUnit->getWhat())+5)) {
    buf[0] = '\0';
    strcat(buf,currentUnit->getWhat());
    strcat(buf,", ");
    strcat(buf,currentUnit->getWho());
    strcat(buf,"!");
    return strlen(buf);
  }
}

int HelloDriver::write(int handle, int count, byte *buf) {
  return ENOSYS;
}

int HelloDriver::close(int handle) {
  return DeviceDriver::close(handle);
}

//---------------------------------------------------------------------------

int HelloDriver::statusCDR_Debug(int handle, int reg, int count, byte *buf) {
  return ENOTSUP;
}

int HelloDriver::controlCDR_Debug(int handle, int reg, int count, byte *buf) {
  return ENOTSUP;
}
