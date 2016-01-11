#include "StepperDriver.h"

//---------------------------------------------------------------------------

StepperDriver::StepperDriver(const char *dName, int addrCount) : DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  logicalUnitCount = min(MAX_Stepper_LU_COUNT, addrCount);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setOpen(false);
    logicalUnits[idx].setDeviceObject(0);
  }
}

//---------------------------------------------------------------------------

int StepperDriver::open(const char *name, int flags) {
  int lun;
  for (lun = 0; lun < logicalUnitCount; lun++) {
    if (strcmp(logicalUnits[lun].getLogicalUnitName(), name) == 0) {
      break;
    }
  }
  if (lun == logicalUnitCount) {
    return ENXIO;
  }

  LogicalUnitInfo *currentDevice = &logicalUnits[lun];

  if ((flags & DDO_FORCE_OPEN) != 0) {
    currentDevice->setOpen(false);
  }

  if (currentDevice->isOpen()) {
    return EADDRINUSE;
  }

  currentDevice->setDeviceObject(0);
  currentDevice->setOpen(true);
  return lun;
}

