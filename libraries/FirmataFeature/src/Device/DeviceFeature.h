
#ifndef DeviceFeature_h
#define DeviceFeature_h

#include <FirmataFeature.h>
#include <Device/DeviceDriver.h>
#include <Device/DeviceTable.h>

#define MAX_DPB_LENGTH 128  // decoded parameter block length (plain text)

// Firmata coding of DeviceDriver methods

#define  DD_OPEN    0x00
#define  DD_STATUS  0x01
#define  DD_CONTROL 0x02
#define  DD_READ    0x03
#define  DD_WRITE   0x04
#define  DD_CLOSE   0x05

class DeviceFeature : public FirmataFeature {
public:
    DeviceFeature();

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

    void update(unsigned long deltaMicros);
    void report(unsigned long deltaMillis);

private:
    DeviceTable *dt;

    int dispatchDeviceAction(int act, int handle, int pc, byte *pv);
    void sendDeviceResponse(int handle, int action, int status);
    void sendDeviceResponse(int handle, int action, int status, const byte *dpBlock);
};

#endif

