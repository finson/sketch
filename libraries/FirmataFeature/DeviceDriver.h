#ifndef DeviceDriver_h
#define DeviceDriver_h

#include <FirmataFeature.h>

#define  DD_OPEN    0x0000
#define  DD_STATUS  0x0001
#define  DD_CONTROL 0x0002
#define  DD_READ    0x0003
#define  DD_WRITE   0x0004
#define  DD_CLOSE   0x0005

class DeviceDriver
{
  public:

    int open(char *controllerName);
    // boolean status(int handle);
    // boolean control(int handle);
    // boolean read(int handle);
    // boolean write(int handle);
    int close(int handle);

};

#endif
