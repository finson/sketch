#include "I2CDeviceInfo.h"

I2CDeviceInfo::I2CDeviceInfo() {
  theDeviceAddress = 0;
}


void I2CDeviceInfo::setDeviceAddress(int addr) {
  theDeviceAddress = addr;
}

int I2CDeviceInfo::getDeviceAddress() {
  return theDeviceAddress;
}
