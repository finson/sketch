#include "I2CDeviceInfo.h"

I2CDeviceInfo::I2CDeviceInfo() : DeviceInfo("") {
    deviceAddress = 0;
}
I2CDeviceInfo::I2CDeviceInfo(char *name, int address) : DeviceInfo(name) {
    deviceAddress = address;
}

void I2CDeviceInfo::setDeviceAddress(int addr) {
  deviceAddress = addr;
}

int I2CDeviceInfo::getDeviceAddress() {
  return deviceAddress;
}
