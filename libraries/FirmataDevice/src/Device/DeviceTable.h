
#ifndef DeviceTable_h
#define DeviceTable_h

#include <Device/DeviceDriver.h>

#define MAX_DPB_LENGTH 128  // decoded parameter block length (plain text)

class DeviceTable : public DeviceDriver {
public:
    DeviceTable(DeviceDriver *deviceArray[]);
    ~DeviceTable();

    int dispatchDeviceAction(int action, int handle, int dpCount, byte *dpBlock);

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

private:
    DECLARE_SEMVER

    int deviceCount;
    DeviceDriver **devices;

};

#endif

