
#ifndef DeviceFeature_h
#define DeviceFeature_h

#include <FirmataFeature.h>
#include <Device/DeviceDriver.h>
#include <Base64.h>

#define MAX_MGR_DEVICE_COUNT 10
#define MAX_MGR_LU_COUNT 1

#define MAX_DEVICE_QUERY_BODY_LENGTH 128
#define DDC_INIT 0

class DeviceFeature : public FirmataFeature, public DeviceDriver {
public:
    DeviceFeature(char *dNameRoot, int count = 1);

    // Firmata Feature methods

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

    // For direct (non-Sysex) use by DeviceDrivers and other local objects

    int dispatchDeviceAction(int act, int minor, int major, byte *body);

    // Device Driver methods

    int open(char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:

    DeviceDriver *majorDevices[MAX_MGR_DEVICE_COUNT+1];
    int majorDeviceCount;

    void sendDeviceResponse(int action, int status);

    LogicalUnitInfo minorDevices[MAX_MGR_LU_COUNT];
};

#endif

