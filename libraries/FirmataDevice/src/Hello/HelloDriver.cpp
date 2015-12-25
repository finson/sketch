#include "HelloDriver.h"

enum class HelloRegister {
  INTERJECTION = 0,
  OBJECT = 1,
  AVG_LOOP_TIME = 10
};

//---------------------------------------------------------------------------

HelloDriver::HelloDriver(const char *dName, int count) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH + 1];
  logicalUnitCount = min(MAX_HELLO_LU_COUNT, count);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH + 1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setWho("World.");
    logicalUnits[idx].setOpen(false);
  }

// Set up to calculate loop time average

  previousMS = 0;
  isSampleBufferFull = false;
  sampleIndex = 0;
  visitCount = 0;
}

// Collect an interval sample, ignoring the very first sample and any
// sample that happens after a rollover (once every 50 days or so).

int HelloDriver::millisecondTimeBase(unsigned long ms) {
  visitCount = (visitCount + 1) % 10;
 if (visitCount == 0) {
    if ((ms > previousMS)) {
      samples[sampleIndex] = ms - previousMS;
      isSampleBufferFull |= (sampleIndex == SAMPLE_COUNT);
      sampleIndex = 1 + ((sampleIndex) % SAMPLE_COUNT);
    }
    previousMS = ms;
 }
  return ESUCCESS;
}

//---------------------------------------------------------------------------

int HelloDriver::open(int *handle, const char *name, int flags) {

  int minorHandle;
  for (minorHandle = 0; minorHandle < logicalUnitCount; minorHandle++) {
    Firmata.sendString(logicalUnits[minorHandle].getLogicalUnitName());
    if (strcmp(logicalUnits[minorHandle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }

  if (minorHandle == logicalUnitCount) {
    return ENXIO;
  }

  LogicalUnitInfo *currentDevice = &logicalUnits[minorHandle];

  if ((flags & DDO_FORCE_OPEN) != 0) {
    currentDevice->setOpen(false);
  }
  if (currentDevice->isOpen()) {
    return EADDRINUSE;
  }

  *handle = minorHandle;
  currentDevice->setOpen(true);
  return ESUCCESS;
}

int HelloDriver::status(int handle, int reg, int count, byte *buf) {
  int sum = 0;
  if (static_cast<HelloRegister>(reg) == HelloRegister::AVG_LOOP_TIME) {
    if (count != 4) {
      return EMSGSIZE;
    } else if (isSampleBufferFull) {
      for (int idx = 1; idx <= SAMPLE_COUNT; idx++) {
        sum += samples[idx];
      }
      unsigned long avg = sum / SAMPLE_COUNT;
      buf[0] = avg & 0xFF;
      buf[1] = (avg >> 8) & 0xFF;
      buf[2] = (avg >> 16) & 0xFF;
      buf[3] = (avg >> 24) & 0xFF;
      return ESUCCESS;
    } else {
      return ENODATA;
    }
  }
  return ENOTSUP;
}

int HelloDriver::control(int handle, int reg, int count, byte *buf) {
  return ENOSYS;
}

int HelloDriver::read(int handle, int count, byte *buf) {
  return ENOSYS;
}
int HelloDriver::write(int handle, int count, byte *buf) {
  return ENOSYS;
}

int HelloDriver::close(int handle) {
  LogicalUnitInfo *currentDevice = &logicalUnits[(handle & 0x7F)];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
  }
  return ESUCCESS;
}
