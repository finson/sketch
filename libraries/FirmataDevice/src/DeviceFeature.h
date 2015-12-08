
#ifndef DeviceFeature_h
#define DeviceFeature_h

#include <FirmataFeature.h>
#include "DeviceDriver.h"
#include <Base64.h>

#define MAX_MAJOR_HANDLE_COUNT 10
#define MAX_DEVICE_NAME_LENGTH 32
#define MAX_DEVICE_QUERY_BODY_LENGTH 128
#define MAX_DEVICE_FEATURE_MINOR_HANDLE_COUNT 1

class DeviceFeature: public FirmataFeature, public DeviceDriver
{
public:
    DeviceFeature(char *dNameRoot = "DeviceFeature", int count = 1);

    // Feature methods

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

    // Device Driver methods

    int open(char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:

    DeviceDriver *devices[MAX_MAJOR_HANDLE_COUNT+1];
    int16_t numDevices;
    void sendDeviceResponse(int action, int status);
    void setSelectedDevices(int flags);

    DeviceInfo devices[MAX_DEVICE_FEATURE_MINOR_HANDLE_COUNT];
    int actualMinorHandleCount = 0;
};

#endif

