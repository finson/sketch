#ifndef PeekDriver_h
#define PeekDriver_h

#include <Device/DeviceDriver.h>
#include "PeekLUI.h"

#define SAMPLE_COUNT 16

enum class PeekRegister {
  AVG_REPORT_INTERVAL = 10
};

/**
 * This PeekDriver class is a development tool to provide code analysis
 * capabilities and a place to perform timing tests and the like.
 */
class PeekDriver: public DeviceDriver {

public:

    PeekDriver(const char *unitName = "PEEK", int count = 1);

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

    int millisecondTimeBase(unsigned long milliDelta);
    int microsecondTimeBase(unsigned long microDelta);

private:

    int statusCDR_Debug(int handle, int reg, int count, byte *buf);

    unsigned long calculateAverageInterval();

    unsigned long previousTime[2];   // the time the last interval expired
    unsigned long currentTime[2];    // the current values from millis() and micros()
    unsigned long samples[SAMPLE_COUNT+1];
    int sampleIndex;
    bool isSampleBufferFull;

    static const PROGMEM uint8_t driverSemVer[];
    static const PROGMEM char driverName[];

};

#endif
