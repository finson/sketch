#ifndef MCP9808Driver_h
#define MCP9808Driver_h

#include <FirmataCore.h>
#include <DeviceDriver.h>
#include <Wire.h>

#define MAX_MCP9808_MINOR_HANDLE_COUNT 8

enum class MCP9808Register;

class MCP9808Driver: public DeviceDriver
{
public:
    MCP9808Driver(char *dNameRoot = "MCP9808", int baseAddr = 0x18, int addrCount = 8);
    MCP9808Driver(char *dNameRoot, int deviceAddresses[], int addrCount);

    int open(char *name, int flags = 0);

    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);

    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);

    int close(int handle);

    class MCP9808DeviceInfo: public DeviceInfo {

    public:
        MCP9808DeviceInfo() {
            theDeviceAddress = 0;
        }

        void setDeviceAddress(int addr) {
            theDeviceAddress = addr;
        }

        int getDeviceAddress() {
            return theDeviceAddress;
        }

    private:
        int theDeviceAddress;
    };

    MCP9808DeviceInfo minorDevices[MAX_MCP9808_MINOR_HANDLE_COUNT];

};


#endif
