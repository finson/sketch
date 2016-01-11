#ifndef StepperDriver_h
#define StepperDriver_h

#include <Device/DeviceDriver.h>

#define MAX_Stepper_LU_COUNT 8

enum class StepperRegister;

class StepperDriver: public DeviceDriver {

public:
    StepperDriver(const char *dName = "Stepper", int addrCount = MAX_Stepper_LU_COUNT);
    int open(const char *name, int flags = 0);

protected:
    LogicalUnitInfo logicalUnits[MAX_Stepper_LU_COUNT];

};


#endif
