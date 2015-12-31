
#ifndef DeviceFeature_h
#define DeviceFeature_h

#include <FirmataFeature.h>
#include <Device/DeviceDriver.h>
#include <Base64.h>

#define MAX_MGR_DEVICE_COUNT 10
#define MAX_MGR_LU_COUNT 1

#define MAX_DPB_LENGTH 128  // decoded parameter block length (plain text)

class DeviceFeature : public FirmataFeature, public DeviceDriver {
public:
    DeviceFeature(const char *dName, int count = 1);

    // Firmata Feature methods

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

    void report();
    void update() {}

    // For direct (non-Sysex) use by DeviceDrivers and other local objects

    int dispatchDeviceAction(int act, int minor, int *pc, byte *pv);

    // Device Driver methods

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:

    int majorDeviceCount;
    DeviceDriver *majorDevices[MAX_MGR_DEVICE_COUNT+1];
    LogicalUnitInfo logicalUnits[MAX_MGR_LU_COUNT];

    void sendDeviceResponse(int action, int handle, int status);
    void sendDeviceResponse(int action, int handle, int status, const byte *dpBlock);

};

#endif

