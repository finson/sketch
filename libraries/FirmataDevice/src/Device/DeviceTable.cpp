#include "DeviceTable.h"

DEFINE_SEMVER(DeviceTable, "DeviceTable", 0, 1, 0)

/**
 * This DeviceTable class provides single point access to all the loaded
 * DeviceDriver objects.  The open() method is used to identify the driver of
 * interest, and the returned handle is then supplied in future calls to the
 * rest of the methods.It implements all the DeviceDriver standard methods via
 * dispatch to the appropriate individual driver.
 *
 * Note that generally speaking a caller should use either DeviceTable methods
 * or direct calls to DeviceDriver objects, but not both.  This is because the
 * 14-bit handles returned by DeviceTable contain both a device index value and
 * a logical unit index value, whereas the 7-bit handles returned by the
 * DeviceDrivers contain only a logical unit value.
 */
//----------------------------------------------------------------------------

DeviceTable::DeviceTable(DeviceDriver *deviceArray[]) :
  DeviceDriver("Dispatcher", 0),
  deviceCount(0) {

  devices = deviceArray;

// Count the installed DeviceDrivers in the array.

  while (devices[deviceCount] != 0) {
    deviceCount += 1;
  }
}

DeviceTable::~DeviceTable() {}

//---------------------------------------------------------------------------

int DeviceTable::open(const char *name, int flags) {
  int deviceIndex;
  int status = ENODEV;
  for (deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++) {
    status = devices[deviceIndex]->open((char *)name, flags);
    if (status == ENXIO || status == ENODEV) {
      continue;
    } else {
      break;
    }
  }
  return (status < 0) ? status : ((deviceIndex & 0x7F) << 7) | (status & 0x7F);
}

int DeviceTable::control(int handle, int reg, int count, byte *buf) {
  return devices[getDeviceHandle(handle)]->control(getUnitHandle(handle), reg, count, buf);
}

int DeviceTable::status(int handle, int reg, int count, byte *buf) {
  return devices[getDeviceHandle(handle)]->status(getUnitHandle(handle), reg, count, buf);
}

int DeviceTable::read(int handle, int count, byte * buf) {
  return devices[getDeviceHandle(handle)]->read(getUnitHandle(handle), count, buf);
}

int DeviceTable::write(int handle, int count, byte * buf) {
  return devices[getDeviceHandle(handle)]->write(getUnitHandle(handle), count, buf);
}

int DeviceTable::close(int handle) {
  return devices[getDeviceHandle(handle)]->close(getUnitHandle(handle));
}

//----------------------------------------------------------------------------

int DeviceTable::dispatchDeviceAction(int action, int handle, int dpCount, byte *dpBlock) {
  int deviceIndex = 0;
  int status = ENODEV;
  int count = 0;
  int reg = 0;
  int unitHandle = (handle & 0x7F);
  int deviceHandle = ((handle >> 7) & 0x7F);
  int flags = 0;

  switch (action) {
  case DD_OPEN:
    flags = handle;
    for (deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++) {
      status = devices[deviceIndex]->open((char *)dpBlock, flags);
      if (status == ENXIO || status == ENODEV) {
        continue;
      } else {
        break;
      }
    }
    return (status < 0) ? status : ((deviceIndex & 0x7F) << 7) | (status & 0x7F);

  case DD_STATUS:
    count = getInt16LE(dpBlock);
    reg   = getInt16LE(dpBlock + 2);
    return devices[deviceHandle]->status(unitHandle, reg, count, dpBlock + 4);

  case DD_CONTROL:
    count = getInt16LE(dpBlock);
    reg   = getInt16LE(dpBlock + 2);
    return devices[deviceHandle]->control(unitHandle, reg, count, dpBlock + 4);

  case DD_READ:
    count = getInt16LE(dpBlock);
    return devices[deviceHandle]->read(unitHandle, count, dpBlock + 2);

  case DD_WRITE:
    count = getInt16LE(dpBlock);
    return devices[deviceHandle]->read(unitHandle, count, dpBlock + 2);

  case DD_CLOSE:
    return devices[deviceHandle]->close(unitHandle);

  default:
    return ENOSYS;
  }
}
