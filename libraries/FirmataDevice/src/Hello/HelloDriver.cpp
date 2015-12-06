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

HelloDriver::HelloDriver(char *dNameRoot, int count) :
  DeviceDriver(dNameRoot) {
  char buf[32];
  actualMinorHandleCount = min(MAX_HELLO_MINOR_HANDLE_COUNT, count);
  for (int idx = 0; idx < actualMinorHandleCount; idx++) {
    sprintf(buf, "%s:%1d", dNameRoot, idx);
    devices[idx].setDeviceName(buf);
    devices[idx].setWho("World.");
    devices[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int HelloDriver::open(char *name, int flags) {
  uint8_t theRegister;

  int handle;
  for (handle = 0; handle < actualMinorHandleCount; handle++) {
    if (strcmp(devices[handle].getDeviceName(), name) == 0) {
      break;
    }
  }

  if (handle == actualMinorHandleCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  DeviceInfo currentDevice = devices[handle];
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
  HelloDeviceInfo currentDevice = devices[handle];
  if (currentDevice.isOpen()) {
    currentDevice.setOpen(false);
    return 0;
  } else {
    return -1;
  }
}
