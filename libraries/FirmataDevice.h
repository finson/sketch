#ifndef FirmataDevice_h
#define FirmataDevice_h

#include <FirmataFeature.h>

class FirmataDevice: public FirmataFeature
{
  public:
    virtual int open(char *deviceName) = 0;
    virtual boolean status(int handle) = 0;
    virtual boolean control(int handle) = 0;
    virtual boolean read(int handle) = 0;
    virtual boolean write(int handle) = 0;
    virtual boolean close(int handle) = 0;
};

#endif
