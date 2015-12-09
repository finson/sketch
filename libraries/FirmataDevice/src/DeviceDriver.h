#ifndef DeviceDriver_h
#define DeviceDriver_h

#include <arduino.h>
#include "DeviceInfo.h"

#define  DD_OPEN    0x00
#define  DD_STATUS  0x01
#define  DD_CONTROL 0x02
#define  DD_READ    0x03
#define  DD_WRITE   0x04
#define  DD_CLOSE   0x05

class DeviceFeature;

extern DeviceFeature deviceManager;

class DeviceDriver
{
  public:
    DeviceDriver(char *nameRoot);

    virtual int open(char *name, int flags) = 0;

    virtual int status(int handle, int reg, int count, byte *buf) = 0;
    virtual int control(int handle, int reg, int count, byte *buf) = 0;

    virtual int read(int handle, int count, byte *buf) = 0;
    virtual int write(int handle, int count, byte *buf) = 0;

    virtual int close(int handle) = 0;

  protected:
    char *deviceNameRoot;
    int minorDeviceCount;
};

#endif
