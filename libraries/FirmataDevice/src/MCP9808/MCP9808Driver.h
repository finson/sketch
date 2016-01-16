#ifndef MCP9808Driver_h
#define MCP9808Driver_h

#include <Device/DeviceDriver.h>
#include "MCP9808LUI.h"

enum class MCP9808Register {
  RESERVED,
  CONFIG,
  UPPER_TEMP,
  LOWER_TEMP,
  CRIT_TEMP,
  AMBIENT_TEMP,
  MANUF_ID,
  DEVICE_ID,
  RESOLUTION
};

class MCP9808Driver: public DeviceDriver {

public:
    MCP9808Driver(const char *unitName = "TC", int count = 8);

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:

    int controlCDR_Configure(int handle, int reg, int count, byte *buf);
    int statusCDR_DriverVersion(int handle, int reg, int count, byte *buf);

    static const PROGMEM uint8_t driverSemVer[];
    static const PROGMEM char driverName[];

};

#endif
