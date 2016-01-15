#ifndef HelloDriver_h
#define HelloDriver_h

#include <Device/DeviceDriver.h>
#include "HelloLUI.h"

#define MAX_HELLO_TEXT_LENGTH 31
#define SAMPLE_COUNT 16

enum class HelloRegister {
  INTERJECTION = 0,
  OBJECT = 1,
  AVG_REPORT_INTERVAL = 10
};

const PROGMEM uint8_t driverSemVer[] = {6,0,1,0,0,0,0};
const PROGMEM char driverName[] = {"HelloDriver"};

/**
 * This class defines a simple device driver as an example
 * HelloWorld component for device drivers and their usage.
 * It also has some general code analysis capabilities and
 * provides a place to perform timing tests and the like.
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

    int millisecondTimeBase(unsigned long milliDelta);
    int microsecondTimeBase(unsigned long microDelta);

private:

    int statusCDR_DriverVersion(int handle, int reg, int count, byte *buf);

    unsigned long previousTime[2];   // the time the last interval expired
    unsigned long currentTime[2];    // the current values from millis() and micros()
    unsigned long samples[SAMPLE_COUNT+1];
    int sampleIndex;
    boolean isSampleBufferFull;
    unsigned long calculateAverageInterval();

};

#endif
