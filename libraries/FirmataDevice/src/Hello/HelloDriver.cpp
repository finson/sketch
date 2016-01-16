#include "HelloDriver.h"

/**
 * This class defines a simple device driver as a sort of
 * HelloWorld component for device drivers and their usage.
 */

const PROGMEM uint8_t HelloDriver::driverSemVer[] = {6,0,1,0,0,0,0};
const PROGMEM char HelloDriver::driverName[] = {"HelloDriver"};

//---------------------------------------------------------------------------

HelloDriver::HelloDriver(const char *dName, int count) :
  DeviceDriver(dName,count) {}

//---------------------------------------------------------------------------

int HelloDriver::open(const char *name, int flags) {
  int lun = DeviceDriver::open(name,flags);
  if (lun < 0) return lun;

  logicalUnits[lun] = new HelloLUI("World");
  return lun;
}

int HelloDriver::status(int handle, int reg, int count, byte *buf) {
  HelloLUI *currentUnit = static_cast<HelloLUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  case CDR_DriverVersion: return statusCDR_DriverVersion(handle, reg, count, buf);
  default: return ENOTSUP;
  }
}

int HelloDriver::control(int handle, int reg, int count, byte *buf) {
  return ENOSYS;
}

int HelloDriver::read(int handle, int count, byte *buf) {
  return ENOSYS;
}
int HelloDriver::write(int handle, int count, byte *buf) {
  return ENOSYS;
}

int HelloDriver::close(int handle) {
  return DeviceDriver::close(handle);
}

//---------------------------------------------------------------------------

int HelloDriver::statusCDR_DriverVersion(int handle, int reg, int count, byte *buf) {
  return DeviceDriver::buildVersionResponse(driverSemVer,driverName,count,buf);
}
