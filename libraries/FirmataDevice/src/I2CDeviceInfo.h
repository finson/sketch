#ifndef I2CDeviceInfo_h
#define I2CDeviceInfo_h

#include <DeviceInfo.h>

class I2CDeviceInfo: public DeviceInfo {

public:

  I2CDeviceInfo();
  I2CDeviceInfo(char *name, int address);
  void setDeviceAddress(int addr);
  int getDeviceAddress();

private:
  int deviceAddress;

};

#endif
