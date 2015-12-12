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

HelloDriver::HelloDriver(char *dName, int count) :
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

int HelloDriver::open(char *name, int flags) {
  uint8_t theRegister;

  int minorHandle;
  for (minorHandle = 0; minorHandle < logicalUnitCount; minorHandle++) {
    Firmata.sendString(logicalUnits[minorHandle].getLogicalUnitName());
    if (strcmp(logicalUnits[minorHandle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }

  if (minorHandle == logicalUnitCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  LogicalUnitInfo *currentDevice = &logicalUnits[minorHandle];
  if (currentDevice->isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }
  currentDevice->setOpen(true);
  return minorHandle;
}

int HelloDriver::status(int handle, int reg, int count, byte *buf) {
  return -1;
}

int HelloDriver::control(int handle, int reg, int count, byte *buf) {
  return -1;
}

int HelloDriver::read(int handle, int count, byte *buf) {
  return -1;
}
int HelloDriver::write(int handle, int count, byte *buf) {
  return -1;
}

int HelloDriver::close(int handle) {
  LogicalUnitInfo *currentDevice = &logicalUnits[(handle & 0x7F)];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
    return 0;
  } else {
    return -1;
  }
}
