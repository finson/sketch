
#ifndef DeviceFeature_h
#define DeviceFeature_h

#include <FirmataFeature.h>
#include <DeviceDriver.h>
#include <Base64.h>

#define MAX_MAJOR_HANDLE_COUNT 10
#define MAX_DEVICE_NAME_LENGTH 32
#define MAX_DEVICE_QUERY_BODY_LENGTH 128

class DeviceFeature: public FirmataFeature
{
  public:
    DeviceFeature();
    void addDevice(DeviceDriver *device);

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

  private:

    DeviceDriver *devices[MAX_MAJOR_HANDLE_COUNT];
    int16_t numDevices;
    void sendSysexResponse(int action, int status);

};

#endif

