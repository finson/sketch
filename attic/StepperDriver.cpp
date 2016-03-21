#include "StepperDriver.h"

//---------------------------------------------------------------------------

StepperDriver::StepperDriver(const char *dName, const int lunCount) : DeviceDriver(dName,lunCount) {
  // char buf[MAX_LU_NAME_LENGTH + 1];
  // logicalUnitCount = min(MAX_Stepper_LU_COUNT, addrCount);
  // for (int idx = 0; idx < logicalUnitCount; idx++) {
  //   snprintf(buf, MAX_LU_NAME_LENGTH + 1, "%s:%1d", dName, idx);
  //   logicalUnits[idx].setLogicalUnitName(buf);
  //   logicalUnits[idx].setOpen(false);
  //   logicalUnits[idx].setDeviceObject(0);
  // }
}

//---------------------------------------------------------------------------

int StepperDriver::open(const char *name, int flags) {
  return DeviceDriver::open(name,flags);
}

