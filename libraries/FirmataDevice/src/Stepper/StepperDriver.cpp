#include "StepperDriver.h"
#include <Mode/StepperMode.h>
#include <Mode/I2CMode.h>

enum class StepperRegister {
  RESERVED,
  CONFIG,
  UPPER_TEMP,
  LOWER_TEMP,
  CRIT_TEMP,
  AMBIENT_TEMP,
  MANUF_ID,
  DEVICE_ID,
  RESOLUTION
};

//---------------------------------------------------------------------------

StepperDriver::StepperDriver(const char *dName, int baseAddr, int addrCount) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  logicalUnitCount = min(MAX_Stepper_LU_COUNT, addrCount);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setDeviceAddress(baseAddr + idx);
    logicalUnits[idx].setOpen(false);
  }
}

StepperDriver::StepperDriver(const char *dName, int deviceAddresses[], int addrCount) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  logicalUnitCount = min(MAX_Stepper_LU_COUNT, addrCount);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setDeviceAddress(deviceAddresses[idx]);
    logicalUnits[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int StepperDriver::open(const char *name, int flags) {
  uint8_t theRegister;
  int lun;
  for (lun = 0; lun < logicalUnitCount; lun++) {
    if (strcmp(logicalUnits[lun].getLogicalUnitName(), name) == 0) {
      break;
    }
  }
  if (lun == logicalUnitCount) {
    return ENXIO;
  }

  StepperLUI *currentDevice = &logicalUnits[lun];

  if ((flags & DDO_FORCE_OPEN) != 0) {
    currentDevice->setOpen(false);
  }

  if (currentDevice->isOpen()) {
    return EADDRINUSE;
  }

  I2CMode.enableI2CPins();

  int address = currentDevice->getDeviceAddress();
  theRegister = static_cast<uint8_t>(StepperRegister::MANUF_ID);
  if (I2CMode.read16(address, theRegister) != 0x0054) {
    I2CMode.disableI2CPins();
    return ECONNREFUSED;
  }

  theRegister = static_cast<uint8_t>(StepperRegister::DEVICE_ID);
  if (I2CMode.read16(address, theRegister) != 0x0400) {
    I2CMode.disableI2CPins();
    return ECONNREFUSED;
  }

  currentDevice->setOpen(true);
  return lun;
}
/**
 * Read a status register on the device. For the Stepper, there are nine
 * registers accessible.
 */
int StepperDriver::status(int handle, int reg, int count, byte *buf) {
  StepperLUI *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }
  if (static_cast<StepperRegister>(reg) == StepperRegister::RESOLUTION) {
    if (count != 1) {
      return EMSGSIZE;
    }
  } else if (count != 2) {
    return EMSGSIZE;
  }

  int address = currentDevice->getDeviceAddress();
  if (count == 1) {
    buf[0] = I2CMode.read8(address, reg);
  } else {
    int v = I2CMode.read16(address, reg);
    buf[0] = (v >> 8) & 0xFF;
    buf[1] = v & 0xFF;
  }
  return count;
}

int StepperDriver::control(int handle, int reg, int count, byte *buf) {
  StepperLUI *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }
  if (static_cast<StepperRegister>(reg) == StepperRegister::RESOLUTION) {
    if (count != 1) {
      return EMSGSIZE;
    }
  } else if (count != 2) {
    return EMSGSIZE;
  }

  int address = currentDevice->getDeviceAddress();
  if (count == 1) {
    I2CMode.write8(address, reg,buf[0]);
  } else {
    I2CMode.write16(address, reg, (((buf[0] & 0xFF) << 8) | (buf[1] & 0xFF)));
  }
  return count;
}

int StepperDriver::read(int handle, int count, byte *buf) {
  StepperLUI *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }

  if (count != 2) {
    return EMSGSIZE;
  }

  int address = currentDevice->getDeviceAddress();
  int reg = static_cast<uint8_t>(StepperRegister::AMBIENT_TEMP);
  int v = I2CMode.read16(address, reg);
  buf[0] = (v >> 8) & 0xFF;
  buf[1] = v & 0xFF;

  return count;
}

int StepperDriver::write(int handle, int count, byte *buf) {
  return ENOSYS;
}

int StepperDriver::close(int lun) {
  StepperLUI *currentDevice = &logicalUnits[lun & 0x7F];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
    I2CMode.disableI2CPins();
  }
  return ESUCCESS;
}
