
#ifndef DeviceFeature_h
#define DeviceFeature_h

#include <FirmataFeature.h>
#include <Device/DeviceDriver.h>
#include <Device/DeviceTable.h>

#define MAX_MGR_DEVICE_COUNT 10
#define MAX_MGR_LU_COUNT 1

#define MAX_DPB_LENGTH 128  // decoded parameter block length (plain text)

class DeviceFeature : public FirmataFeature {
public:
    DeviceFeature(const char *dName);

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

    void update(unsigned long deltaMicros);
    void report(unsigned long deltaMillis);

    int dispatchDeviceAction(int act, int handle, int pc, byte *pv);
    void sendDeviceResponse(int handle, int action, int status);
    void sendDeviceResponse(int handle, int action, int status, const byte *dpBlock);

private:
    DeviceTable *dt;

};

#endif

