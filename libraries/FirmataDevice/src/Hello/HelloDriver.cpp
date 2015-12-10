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
  minorDeviceCount = min(MAX_HELLO_LU_COUNT, count);
  for (int idx = 0; idx < minorDeviceCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    minorDevices[idx].setLogicalUnitName(buf);
    minorDevices[idx].setWho("World.");
    minorDevices[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int HelloDriver::open(char *name, int flags) {
  uint8_t theRegister;

  int handle;
  for (handle = 0; handle < minorDeviceCount; handle++) {
    Firmata.sendString(minorDevices[handle].getLogicalUnitName());
    if (strcmp(minorDevices[handle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }

  if (handle == minorDeviceCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  LogicalUnitInfo currentDevice = minorDevices[handle];
  if (currentDevice.isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }

  currentDevice.setOpen(true);
  return handle;
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
  LogicalUnitInfo currentDevice = minorDevices[handle];
  if (currentDevice.isOpen()) {
    currentDevice.setOpen(false);
    return 0;
  } else {
    return -1;
  }
}
