#ifndef MCP9808Driver_h
#define MCP9808Driver_h

#include <Silicon/I2CPort.h>
#include <Device/DeviceDriver.h>
#include "MCP9808LUI.h"

enum class MCP9808Register : int {
  RESERVED = 0,
  CONFIG = 1,
  UPPER_TEMP = 2,
  LOWER_TEMP = 3,
  CRIT_TEMP = 4,
  AMBIENT_TEMP = 5,
  MANUF_ID = 6,
  DEVICE_ID = 7,
  RESOLUTION = 8
};

class MCP9808Driver: public DeviceDriver {

public:
    MCP9808Driver(const char *unitName = "TC", int count = 8, int base = 0x18);

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:
    DECLARE_SEMVER

    int statusCDR_Debug(int handle, int reg, int count, byte *buf);

    int baseAddress;
    I2CPort i2c;

};

#endif
