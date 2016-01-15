#include "HelloDriver.h"

#include <limits.h>

/**
 * This class defines a simple device driver as a sort of
 * HelloWorld component for device drivers and their usage.
 * It also has some general code analysis capabilities and
 * provides a place to perform timing tests and the like.
 */

//---------------------------------------------------------------------------

HelloDriver::HelloDriver(const char *dName, int count) :
  DeviceDriver(dName,count) {

// Set up to calculate loop time average

  previousTime[0] = 0;
  previousTime[1] = 0;
  isSampleBufferFull = false;
  sampleIndex = 0;
}

//---------------------------------------------------------------------------

int HelloDriver::open(const char *name, int flags) {
  int lun = DeviceDriver::open(name,flags);
  if (lun < 0) return lun;

  logicalUnits[lun] = new HelloLUI();
  return lun;
}

// int HelloDriver::status(int handle, int reg, int count, byte *buf) {
//   if (static_cast<int>(HelloRegister::AVG_REPORT_INTERVAL) == reg) {
//     if (count != 4) {
//       return EMSGSIZE;
//     } else if (isSampleBufferFull) {
//       unsigned long avg = calculateAverageInterval();
//       buf[0] = avg & 0xFF;
//       buf[1] = (avg >> 8) & 0xFF;
//       buf[2] = (avg >> 16) & 0xFF;
//       buf[3] = (avg >> 24) & 0xFF;
//       return 4;
//     } else {
//       return ENODATA;
//     }
//   }
//   return ENOTSUP;
// }

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
  return DeviceDriver::close(handle);
}

//---------------------------------------------------------------------------

int HelloDriver::microsecondTimeBase(unsigned long deltaMicros) {

}

//---------------------------------------------------------------------------

// Collect a millisecond interval (report()) duration sample.  The sample array
// is actually 0..SAMPLE_COUNT, and the useful samples are in 1..SAMPLE_COUNT.

int HelloDriver::millisecondTimeBase(unsigned long deltaMillis) {
  currentTime[0] = millis();

  unsigned long elapsedTime;

  if (currentTime[0] >= previousTime[0]) {
    elapsedTime = currentTime[0]-previousTime[0];
  } else {
    elapsedTime = (ULONG_MAX - (previousTime[0]-currentTime[0])) + 1;
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

//---------------------------------------------------------------------------

int HelloDriver::status(int handle, int reg, int count, byte *buf) {
  HelloLUI *currentUnit = static_cast<HelloLUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  case CDR_DriverVersion: return statusCDR_DriverVersion(handle, reg, count, buf);
  default: return ENOTSUP;
  }
}

int HelloDriver::statusCDR_DriverVersion(int handle, int reg, int count, byte *buf) {
  return DeviceDriver::buildVersionResponse(driverSemVer,driverName,count,buf);
}
