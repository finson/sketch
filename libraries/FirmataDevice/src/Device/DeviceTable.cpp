#include "DeviceTable.h"

DEFINE_SEMVER(DeviceTable, 0, 1, 0)

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
 * DeviceDrivers themselves contain only a logical unit value.
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

  previousTime[0] = 0;
  previousTime[1] = 0;
  intervalTime[0] = DEFAULT_REPORT_INTERVAL;
  intervalTime[1] = DEFAULT_UPDATE_INTERVAL;

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

void DeviceTable::update(unsigned long deltaMicros) {
  for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++) {
    devices[deviceIndex]->update(deltaMicros);
  }
}

void DeviceTable::report(unsigned long deltaMillis) {
  for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++) {
    devices[deviceIndex]->report(deltaMillis);
  }
}

void DeviceTable::dispatchTimers() {
  unsigned long elapsedTime;

  currentTime[0] = micros();
  currentTime[1] = millis();

  for (int idx = 0; idx < 2; idx++) {
    if (currentTime[idx] >= previousTime[idx]) {
      elapsedTime = currentTime[idx] - previousTime[idx];
    } else {
      elapsedTime = (ULONG_MAX - previousTime[idx]) + (currentTime[idx] + 1);
    }

    if (elapsedTime >= intervalTime[idx]) {
      if (idx == 0) {
        update(elapsedTime);
      } else {
        report(elapsedTime);
      }
      previousTime[idx] = currentTime[idx];
    }
  }
}

// int DeviceTable::dispatchDeviceAction(int action, int handle, int dpCount, byte *dpBlock) {
//   int deviceIndex = 0;
//   int status = ENODEV;
//   int count = 0;
//   int reg = 0;
//   int unitHandle = (handle & 0x7F);
//   int deviceHandle = ((handle >> 7) & 0x7F);
//   int flags = 0;

//   switch (action) {
//   case DD_OPEN:
//     flags = handle;
//     for (deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++) {
//       status = devices[deviceIndex]->open((char *)dpBlock, flags);
//       if (status == ENXIO || status == ENODEV) {
//         continue;
//       } else {
//         break;
//       }
//     }
//     return (status < 0) ? status : ((deviceIndex & 0x7F) << 7) | (status & 0x7F);

//   case DD_STATUS:
//     count = from16LEToHost(dpBlock);
//     reg   = from16LEToHost(dpBlock + 2);
//     return devices[deviceHandle]->status(unitHandle, reg, count, dpBlock + 4);

//   case DD_CONTROL:
//     count = from16LEToHost(dpBlock);
//     reg   = from16LEToHost(dpBlock + 2);
//     return devices[deviceHandle]->control(unitHandle, reg, count, dpBlock + 4);

//   case DD_READ:
//     count = from16LEToHost(dpBlock);
//     return devices[deviceHandle]->read(unitHandle, count, dpBlock + 2);

//   case DD_WRITE:
//     count = from16LEToHost(dpBlock);
//     return devices[deviceHandle]->read(unitHandle, count, dpBlock + 2);

//   case DD_CLOSE:
//     return devices[deviceHandle]->close(unitHandle);

//   default:
//     return ENOSYS;
//   }
// }
