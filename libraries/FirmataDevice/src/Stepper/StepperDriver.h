#ifndef StepperDriver_h
#define StepperDriver_h

#include <FirmataCore.h>
#include <Device/DeviceDriver.h>
#include <Wire.h>

#define MAX_Stepper_LU_COUNT 8

enum class StepperRegister;

class StepperDriver: public DeviceDriver
{
public:
    StepperDriver(const char *dName = "Stepper", int baseAddr = 0x18, int addrCount = 8);
    StepperDriver(const char *dName, int deviceAddresses[], int addrCount);

    int open(int *handle, const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

    class StepperLUI: public LogicalUnitInfo {

    public:
        StepperLUI(const char *name) : LogicalUnitInfo(name) {
            theDeviceAddress = 0;
        }

        StepperLUI() : LogicalUnitInfo() {
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

    StepperLUI logicalUnits[MAX_Stepper_LU_COUNT];

};


#endif
