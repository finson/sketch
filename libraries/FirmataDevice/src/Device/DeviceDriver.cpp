#include "DeviceDriver.h"


DeviceDriver::DeviceDriver(const char *nameRoot) {
  strlcpy(deviceName,nameRoot,(MAX_DEVICE_NAME_LENGTH+1));
  logicalUnitCount = 0;
};


// int DeviceDriver::open(char *name, int flags) {};

// int DeviceDriver::status(int handle, int reg, int count, byte *buf){};
// int DeviceDriver::control(int handle, int reg, int count, byte *buf){};

// int DeviceDriver::read(int handle, int count, byte *buf){};
// int DeviceDriver::write(int handle, int count, byte *buf){};

// int DeviceDriver::close(int handle){};
