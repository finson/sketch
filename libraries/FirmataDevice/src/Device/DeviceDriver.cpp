#include "DeviceDriver.h"

DeviceDriver::DeviceDriver(const char *nameRoot) {
  strlcpy(deviceName,nameRoot,(MAX_DEVICE_NAME_LENGTH+1));
  logicalUnitCount = 0;
};

int DeviceDriver::microsecondUpdate(unsigned long deltaMicros) {
  return ESUCCESS;
}

int DeviceDriver::millisecondReport(unsigned long deltaMillis) {
  return ESUCCESS;
}

