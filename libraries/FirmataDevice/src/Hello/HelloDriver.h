#ifndef HelloDriver_h
#define HelloDriver_h

#include <Device/DeviceDriver.h>
#include "HelloLUI.h"

#define MAX_HELLO_TEXT_LENGTH 31

enum class HelloRegister {
  INTERJECTION = 0,
  OBJECT = 1,
};

/**
 * This class defines a simple device driver as an example
 * HelloWorld component for device drivers and their usage.
 */
class HelloDriver: public DeviceDriver {

public:

    HelloDriver(const char *unitName = "HLO", int count = 1);

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:

    static const PROGMEM uint8_t driverSemVer[];
    static const PROGMEM char driverName[];

};

#endif
