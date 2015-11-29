#ifndef DeviceDriver_h
#define DeviceDriver_h

#define  DD_OPEN    0x0000
#define  DD_STATUS  0x0001
#define  DD_CONTROL 0x0002
#define  DD_READ    0x0003
#define  DD_WRITE   0x0004
#define  DD_CLOSE   0x0005

class DeviceDriver
{
  public:
    virtual int open(String name) = 0;
    virtual int open(String name, int flags) = 0;

    virtual int status(int handle, int reg, int count, byte *buf) = 0;
    virtual int control(int handle, int reg, int count, byte *buf) = 0;

    virtual int read(int handle, int count, byte *buf) = 0;
    virtual int write(int handle, int count, byte *buf) = 0;

    virtual int close(int handle) = 0;
};

#endif
