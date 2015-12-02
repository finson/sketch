#ifndef I2CDeviceInfo_h
#define I2CDeviceInfo_h

#include <DeviceInfo.h>
#include <I2CReadWrite.h>

class I2CDeviceInfo: public DeviceInfo {

public:

  I2CDeviceInfo(char *name = "NotNamed", int address = 0);
  void setDeviceAddress(int addr);
  int getDeviceAddress();
  I2CReadWrite getChannel();

private:
  I2CReadWrite channel;

};

#endif
