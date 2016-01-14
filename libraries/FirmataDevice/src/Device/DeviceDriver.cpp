#include "DeviceDriver.h"

DeviceDriver::DeviceDriver(const char *nameRoot) {
  strlcpy(deviceName,nameRoot,(MAX_DEVICE_NAME_LENGTH+1));
  logicalUnitCount = 0;
};

int DeviceDriver::microsecondUpdate(unsigned long deltaMicros) {
  return ESUCCESS;
}

int DeviceDriver::millisecondReport(unsigned long deltaMillis) {
  return ESUCCESS;
}

int DeviceDriver::buildVersionResponse(const byte *semver,const char *name,int count, byte *buf) {

  int nameLength = strlen_P(name);
  int packetSize = pgm_read_byte_near(&semver[0]);

  if (count < (1 + packetSize + nameLength + 1)) return EMSGSIZE;

  int byteIndex = 0;
  buf[byteIndex++] = packetSize;

  // version

  for (int idx=0; idx<packetSize; idx++) {
    buf[byteIndex++] = pgm_read_byte_near(&semver[idx+1]);
  }

  // name (including terminating null)

  for (int idx=0;idx<=nameLength;idx++) {
    buf[byteIndex++] = pgm_read_byte_near(&name[idx]);
  }

  return byteIndex;
}


