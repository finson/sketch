#include "PeekDriver.h"
#include <limits.h>

const PROGMEM uint8_t PeekDriver::driverSemVer[] = {6,0,1,0,0,0,0};
const PROGMEM char PeekDriver::driverName[] = {"PeekDriver"};

/**
 * This PeekDriver class is a development tool to provide code analysis
 * capabilities and a place to perform timing tests and the like.
 */

//---------------------------------------------------------------------------

PeekDriver::PeekDriver(const char *dName, int count) :
  DeviceDriver(dName, count),
  previousTime{0, 0},
  currentTime{0, 0},
  sampleIndex(0),
  isSampleBufferFull(false) {}

//---------------------------------------------------------------------------

int PeekDriver::open(const char *name, int flags) {
  int lun = DeviceDriver::open(name, flags);
  if (lun < 0) return lun;

  logicalUnits[lun] = new PeekLUI();
  return lun;
}

//---------------------------------------------------------------------------

int PeekDriver::status(int handle, int reg, int count, byte *buf) {
  PeekLUI *currentUnit = static_cast<PeekLUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  case static_cast<int>(CDR::DriverVersion):
    return DeviceDriver::buildVersionResponse(PeekDriver::driverSemVer, PeekDriver::driverName, count, buf);
  case static_cast<int>(CDR::Debug):
    return statusCDR_Debug(handle, reg, count, buf);
  default:
    return ENOTSUP;
  }
}

int PeekDriver::control(int handle, int reg, int count, byte *buf) {
  return ENOSYS;
}

int PeekDriver::read(int handle, int count, byte *buf) {
  return ENOSYS;
}
int PeekDriver::write(int handle, int count, byte *buf) {
  return ENOSYS;
}

int PeekDriver::close(int handle) {
  return DeviceDriver::close(handle);
}

//---------------------------------------------------------------------------

int PeekDriver::microsecondTimeBase(unsigned long deltaMicros) {

}

//---------------------------------------------------------------------------

// Collect a millisecond interval (report()) duration sample.  The sample array
// is actually 0..SAMPLE_COUNT, and the useful samples are in 1..SAMPLE_COUNT.

int PeekDriver::millisecondTimeBase(unsigned long deltaMillis) {
  currentTime[0] = millis();

  unsigned long elapsedTime;

  if (currentTime[0] >= previousTime[0]) {
    elapsedTime = currentTime[0] - previousTime[0];
  } else {
    elapsedTime = (ULONG_MAX - (previousTime[0] - currentTime[0])) + 1;
  }

  samples[sampleIndex] = elapsedTime;
  isSampleBufferFull |= (sampleIndex == SAMPLE_COUNT);
  sampleIndex = 1 + ((sampleIndex) % SAMPLE_COUNT);
  previousTime[0] = currentTime[0];

  return ESUCCESS;
}

//---------------------------------------------------------------------------

int PeekDriver::statusCDR_Debug(int handle, int reg, int count, byte *buf) {
  if (count != 4) {
    return EMSGSIZE;
  } else if (isSampleBufferFull) {
    unsigned long avg = calculateAverageInterval();
    buf[0] = avg & 0xFF;
    buf[1] = (avg >> 8) & 0xFF;
    buf[2] = (avg >> 16) & 0xFF;
    buf[3] = (avg >> 24) & 0xFF;
    return 4;
  } else {
    return ENODATA;
  }

  return ENOTSUP;
}

unsigned long PeekDriver::calculateAverageInterval() {
  int sum = 0;
  for (int idx = 1; idx <= SAMPLE_COUNT; idx++) {
    sum += samples[idx];
  }
  return sum / SAMPLE_COUNT;
}
