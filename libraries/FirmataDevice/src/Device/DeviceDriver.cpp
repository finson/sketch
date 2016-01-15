#include <Device/DeviceDriver.h>

//---------------------------------------------------------------------------

DeviceDriver::DeviceDriver(const char *r, const int count) :
rootName(r),
logicalUnitCount(count),
logicalUnits(new LogicalUnitInfo*[count]())
{};

//---------------------------------------------------------------------------

int DeviceDriver::microsecondUpdate(unsigned long deltaMicros) {
  return ESUCCESS;
}

int DeviceDriver::millisecondReport(unsigned long deltaMillis) {
  return ESUCCESS;
}

//---------------------------------------------------------------------------

int DeviceDriver::open(const char *name, int flags) {
  int lun;

  int unitNameLength = strcspn(name,":");
  if (!(strlen(rootName) == unitNameLength) && strncmp(rootName,name,unitNameLength)) {
    return ENODEV;
  }

  lun = atoi(&name[unitNameLength+1]);
  if (lun < 0 || lun >= logicalUnitCount) {
    return ENXIO;
  }

  if (logicalUnits[lun] == 0) {
    return lun;
  }

  if ((flags & DDO_FORCE_OPEN) == 0) {
    return EADDRINUSE;
  } else {
    delete logicalUnits[lun];
    return lun;
  }
  return ENXIO;
}

int DeviceDriver::close(int handle) {
  LogicalUnitInfo *currentDevice = logicalUnits[(handle & 0x7F)];
  if (currentDevice != 0) delete currentDevice;
  return ESUCCESS;
}

//---------------------------------------------------------------------------

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


