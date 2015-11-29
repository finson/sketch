
#ifndef DeviceManager_h
#define DeviceManager_h

#include <FirmataFeature.h>

#define MAX_DEVICES 10
#define MAX_DEVICE_NAME_LENGTH 32

class DeviceManager: public FirmataFeature
{
  public:
    DeviceManager();
    void addDevice(DeviceDriver *device);

    void reset();
    void handleGetCapability(byte pin);
    boolean handleSetPinMode(byte pin, int mode);
    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

  private:

    DeviceDriver *devices[MAX_DEVICES];
    int16_t numDevices;

};

#endif

