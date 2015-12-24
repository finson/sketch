#include "HelloDriver.h"

enum class HelloRegister {
  RESERVED,
  CONFIG,
  UPPER_TEMP,
  LOWER_TEMP,
  CRIT_TEMP,
  AMBIENT_TEMP,
  MANUF_ID,
  DEVICE_ID,
  RESOLUTION
};

//---------------------------------------------------------------------------

HelloDriver::HelloDriver(const char *dName, int count) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  logicalUnitCount = min(MAX_HELLO_LU_COUNT, count);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setWho("World.");
    logicalUnits[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int HelloDriver::open(int *handle, const char *name, int flags) {

  int minorHandle;
  for (minorHandle = 0; minorHandle < logicalUnitCount; minorHandle++) {
    Firmata.sendString(logicalUnits[minorHandle].getLogicalUnitName());
    if (strcmp(logicalUnits[minorHandle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }

  if (minorHandle == logicalUnitCount) {
    return ENXIO;
  }

  LogicalUnitInfo *currentDevice = &logicalUnits[minorHandle];

  if ((flags & DDO_FORCE_OPEN) != 0) {
    currentDevice->setOpen(false);
  }
  if (currentDevice->isOpen()) {
    return EADDRINUSE;
  }

  *handle = minorHandle;
  currentDevice->setOpen(true);
  return ESUCCESS;
}

int HelloDriver::status(int handle, int reg, int count, byte *buf) {
  return ENOSYS;
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
  LogicalUnitInfo *currentDevice = &logicalUnits[(handle & 0x7F)];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
  }
  return ESUCCESS;

}
