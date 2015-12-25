#ifndef DeviceDriver_h
#define DeviceDriver_h

#include <arduino.h>
#include "LogicalUnitInfo.h"
#include "DeviceError.h"

#define MAX_DEVICE_NAME_LENGTH (MAX_LU_NAME_LENGTH-3)

#define  DD_OPEN    0x00
#define  DD_STATUS  0x01
#define  DD_CONTROL 0x02
#define  DD_READ    0x03
#define  DD_WRITE   0x04
#define  DD_CLOSE   0x05

#define DDO_FORCE_OPEN 0x01

#define DDC_INIT 0

class DeviceFeature;

extern DeviceFeature deviceManager;

class DeviceDriver
{
  public:
    DeviceDriver(const char *nameRoot);

    virtual int open(int *handle, const char *name, int flags) = 0;
    virtual int status(int handle, int reg, int count, byte *buf) = 0;
    virtual int control(int handle, int reg, int count, byte *buf) = 0;
    virtual int read(int handle, int count, byte *buf) = 0;
    virtual int write(int handle, int count, byte *buf) = 0;
    virtual int close(int handle) = 0;

    virtual int millisecondTimeBase(unsigned long ms) { return ESUCCESS; }

  protected:
    char deviceName[MAX_DEVICE_NAME_LENGTH+1];
    int logicalUnitCount;
};

#endif
