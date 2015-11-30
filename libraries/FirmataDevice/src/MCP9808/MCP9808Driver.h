#ifndef MCP9808Driver_h
#define MCP9808Driver_h

#include <DeviceDriver.h>

class MCP9808Driver: public DeviceDriver
{
  public:
    MCP9808Driver(char *dName, int baseAddr, int addrCount);

    int open(char *name);
    int open(char *name, int flags);

    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);

    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);

    int close(int handle);
};

#endif
