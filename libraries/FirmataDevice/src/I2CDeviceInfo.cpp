#include "I2CDeviceInfo.h"

I2CDeviceInfo::I2CDeviceInfo(char *name, int address):
DeviceInfo(name),
channel(address) {

}

void I2CDeviceInfo::setDeviceAddress(int addr) {
  channel.setDeviceAddress(addr);
}

int I2CDeviceInfo::getDeviceAddress() {
  return channel.getDeviceAddress();
}

I2CReadWrite I2CDeviceInfo::getChannel() {
  return channel;
}
