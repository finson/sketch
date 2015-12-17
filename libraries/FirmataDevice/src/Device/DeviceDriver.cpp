#include "DeviceDriver.h"


DeviceDriver::DeviceDriver(const char *nameRoot) {
  strlcpy(deviceName,nameRoot,(MAX_DEVICE_NAME_LENGTH+1));
  logicalUnitCount = 0;
};


// int DeviceDriver::open(int *handle, char *name, int flags) {
//   int lun;
//   for (lun = 0; lun < logicalUnitCount; lun++) {
//     if (strcmp(logicalUnits[lun].getLogicalUnitName(), name) == 0) {
//       break;
//     }
//   }
//   if (lun == logicalUnitCount) {
//     return ENXIO;
//   }

//   LogicalUnitInfo *currentDevice = &logicalUnits[lun];

//   if (flags & DDO_FORCE_OPEN != 0) {
//     currentDevice->setOpen(false);
//   }

//   if (currentDevice->isOpen()) {
//     return EADDRINUSE;
//   }

//   currentDevice->setOpen(true);
//   *handle = lun;
//   return ESUCCESS;
// }

// int DeviceDriver::status(int handle, int reg, int count, byte *buf){};
// int DeviceDriver::control(int handle, int reg, int count, byte *buf){};

// int DeviceDriver::read(int handle, int count, byte *buf){};
// int DeviceDriver::write(int handle, int count, byte *buf){};

// int DeviceDriver::close(int handle){};
