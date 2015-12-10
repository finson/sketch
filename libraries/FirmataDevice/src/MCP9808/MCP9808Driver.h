#ifndef MCP9808Driver_h
#define MCP9808Driver_h

#include <FirmataCore.h>
#include <Device/DeviceDriver.h>
#include <Wire.h>

#define MAX_MCP9808_LU_COUNT 8

enum class MCP9808Register;

class MCP9808Driver: public DeviceDriver
{
public:
    MCP9808Driver(char *dName = "MCP9808", int baseAddr = 0x18, int addrCount = 8);
    MCP9808Driver(char *dName, int deviceAddresses[], int addrCount);

    int open(char *name, int flags = 0);

    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);

    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);

    int close(int handle);

    class MCP9808LUI: public LogicalUnitInfo {

    public:
        MCP9808LUI(const char *name) : LogicalUnitInfo(name) {
            theDeviceAddress = 0;
        }

        MCP9808LUI() : LogicalUnitInfo() {
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

    MCP9808LUI minorDevices[MAX_MCP9808_LU_COUNT];

};


#endif
