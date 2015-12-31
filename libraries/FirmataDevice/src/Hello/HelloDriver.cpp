#include "HelloDriver.h"

#include <limits.h>

enum class HelloRegister {
  INTERJECTION = 0,
  OBJECT = 1,
  AVG_REPORT_INTERVAL = 10
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

  previousTime[0] = 0;
  previousTime[1] = 0;
  isSampleBufferFull = false;
  sampleIndex = 0;
}

//---------------------------------------------------------------------------

int HelloDriver::open(const char *name, int flags) {

  int minorHandle;
  for (minorHandle = 0; minorHandle < logicalUnitCount; minorHandle++) {
    // Firmata.sendString(logicalUnits[minorHandle].getLogicalUnitName());
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

  currentDevice->setOpen(true);
  return minorHandle;
}

int HelloDriver::status(int handle, int reg, int count, byte *buf) {
  if (static_cast<int>(HelloRegister::AVG_REPORT_INTERVAL) == reg) {
    if (count != 4) {
      return EMSGSIZE;
    } else if (isSampleBufferFull) {
      unsigned long avg = calculateAverageInterval();
      buf[0] = avg & 0xFF;
      buf[1] = (avg >> 8) & 0xFF;
      buf[2] = (avg >> 16) & 0xFF;
      buf[3] = (avg >> 24) & 0xFF;
      return count;
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

//---------------------------------------------------------------------------

// Collect an interval sample.  The sample array is actually 0..SAMPLE_COUNT,
// and the useful samples are in 1..SAMPLE_COUNT.

int HelloDriver::millisecondTimeBase() {
  currentTime[0] = millis();
  currentTime[1] = micros();

  unsigned long elapsedTime;

  if (currentTime[0] >= previousTime[0]) {
    elapsedTime = currentTime[0]-previousTime[0];
  } else {
    elapsedTime = (ULONG_MAX - previousTime[0])+(currentTime[0]+1);
  }

  samples[sampleIndex] = elapsedTime;
  isSampleBufferFull |= (sampleIndex == SAMPLE_COUNT);
  sampleIndex = 1 + ((sampleIndex) % SAMPLE_COUNT);
  previousTime[0] = currentTime[0];

  return ESUCCESS;
}

//---------------------------------------------------------------------------

unsigned long HelloDriver::calculateAverageInterval() {
  int sum = 0;
  for (int idx = 1; idx <= SAMPLE_COUNT; idx++) {
    sum += samples[idx];
  }
  return sum / SAMPLE_COUNT;
}
