#ifndef StepperDriver_h
#define StepperDriver_h

#include <Device/DeviceDriver.h>

#define MAX_Stepper_LU_COUNT 8

// Stepper register definitions

#define STP_MoveR 0
#define STP_RPMSpeed 1

class StepperDriver: public DeviceDriver {

public:
    StepperDriver(const char *dName = "Stepper", int addrCount = MAX_Stepper_LU_COUNT);
    int open(const char *name, int flags = 0);

protected:
    LogicalUnitInfo logicalUnits[MAX_Stepper_LU_COUNT];

};


#endif
