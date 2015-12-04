#include "I2CDeviceInfo.h"

I2CDeviceInfo::I2CDeviceInfo(char *name, int address):
DeviceInfo(name),
theDeviceAddress(address) {

}

void I2CDeviceInfo::setDeviceAddress(int addr) {
  theDeviceAddress = addr;
}

int I2CDeviceInfo::getDeviceAddress() {
  return theDeviceAddress;
}
