/*
  DeviceFeature.cpp - Firmata library
*/

#include "DeviceFeature.h"

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature(const char *dName, int count) : DeviceDriver(dName), majorDeviceCount(0)
{
  char buf[MAX_DEVICE_NAME_LENGTH + 1];

// Copy the list of addresses of installed DeviceDrivers from SelectedFeatures.h

  majorDeviceCount = 0;
  majorDevices[majorDeviceCount++] = this;

  int selectionIndex = 0;
  while (selectedDevices[selectionIndex] != 0) {
    if (majorDeviceCount < MAX_MGR_DEVICE_COUNT) {
      majorDevices[majorDeviceCount++] = selectedDevices[selectionIndex];
    }
    selectionIndex += 1;
  }

// Initialize the available DeviceFeature logical units

  logicalUnitCount = min(MAX_MGR_LU_COUNT, count);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_DEVICE_NAME_LENGTH + 1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

// The following methods implement the FirmataFeature aspects of this class.

void DeviceFeature::reset()
{
}

void DeviceFeature::handleGetCapability(byte pin)
{
}

boolean DeviceFeature::handleSetPinMode(byte pin, int mode)
{
  return false;
}

void DeviceFeature::report() {
  for (int idx = 0; idx < majorDeviceCount; idx++) {
    majorDevices[idx]->millisecondTimeBase();
  }
}

//---------------------------------------------------------------------------

// The first six bytes of argv for DEVICE_QUERY messages are: action, reserved,
// handle-low, handle-high, reserved, reserved. They are all constrained to
// 7-bit values.The bytes that follow, if any, are the parameter block. The
// parameter block is encoded with base-64 in the sysex message body during
// transmission to and from this Firmata server.

//  dpB -> decoded parameter block
//  epB -> encoded parameter block

boolean DeviceFeature::handleFeatureSysex(byte command, byte argc, byte *argv) {

  byte dpBlock[1 + MAX_DPB_LENGTH]; // decoded parameter block

  if (command != DEVICE_QUERY) {
    return false;
  }

  int action = argv[0];
  int handle = (argv[3] << 7) | argv[2];

  int dpCount = base64_dec_len((char *)(argv + 6), argc - 6);
  if (dpCount > MAX_DPB_LENGTH) {
    sendDeviceResponse(action, handle, EMSGSIZE);
    return true;
  }

  if (dpCount > 0) {
    dpCount = base64_decode((char *)dpBlock, (char *)(argv + 6), argc - 6);
  }

  int status = dispatchDeviceAction(action, handle, dpCount, dpBlock);

  sendDeviceResponse(action, handle, status, dpBlock);
  return true;
}

int DeviceFeature::dispatchDeviceAction(int action, int handle, int dpCount, byte *dpBlock) {
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
    for (deviceIndex = 0; deviceIndex < majorDeviceCount; deviceIndex++) {
      status = majorDevices[deviceIndex]->open((char *)dpBlock, flags);
      if (status == ENXIO || status == ENODEV) {
        continue;
      } else {
        break;
      }
    }
    return (status < 0) ? status : ((deviceIndex & 0x7F) << 7) | (status & 0x7F);

  case DD_STATUS:
    count = ((dpBlock[1] & 0xFF) << 8) | (dpBlock[0] & 0xFF);
    reg   = ((dpBlock[3] & 0xFF) << 8) | (dpBlock[2] & 0xFF);
    return majorDevices[deviceHandle]->status(unitHandle, reg, count, dpBlock);

  case DD_CONTROL:
    count = ((dpBlock[1] & 0xFF) << 8)  | (dpBlock[0] & 0xFF);
    reg   = ((dpBlock[3] & 0xFF) << 8)  | (dpBlock[2] & 0xFF);
    return majorDevices[deviceHandle]->control(unitHandle, reg, count, dpBlock + 4);

  case DD_READ:
    count = ((dpBlock[1] & 0xFF) << 8) | ((dpBlock[0] & 0xFF));
    return majorDevices[deviceHandle]->read(unitHandle, count, dpBlock);

  case DD_WRITE:
    count = ((dpBlock[1] & 0xFF) << 8) | ((dpBlock[0] & 0xFF));
    return majorDevices[deviceHandle]->read(unitHandle, count, dpBlock + 2);

  case DD_CLOSE:
    return majorDevices[deviceHandle]->close(unitHandle);

  default:
    return ENOSYS;
  }
}

//  dpB -> decoded parameter block
//  epB -> encoded parameter block

void DeviceFeature::sendDeviceResponse(int action, int handle, int status) {
  sendDeviceResponse(action, handle, status, 0);
}

void DeviceFeature::sendDeviceResponse(int action, int handle, int status, const byte *dpB) {
  byte epB[1 + ((MAX_DPB_LENGTH + 2) / 3) * 4];

  Firmata.write(START_SYSEX);
  Firmata.write(DEVICE_RESPONSE);
  Firmata.write(action & 0x7F);
  Firmata.write(0);
  if (action == DD_OPEN) {
    Firmata.write(0);
    Firmata.write(0);
    Firmata.write(status & 0x7F);                   // status is handle or error
    Firmata.write((status >> 7) & 0x7F);
  } else {
    Firmata.write(handle & 0x7F);
    Firmata.write((handle >> 7) & 0x7F);
    Firmata.write(status & 0x7F);                   // status is bytecount or error
    Firmata.write((status >> 7) & 0x7F);

    if (status > 0 && status <= MAX_DPB_LENGTH) {   // status is bytecount
      int epCount = base64_encode((char *)epB, (char *)dpB, status);
      for (int idx = 0; idx < epCount; idx++) {
        Firmata.write(epB[idx]);
      }
    }
  }
  Firmata.write(END_SYSEX);
}

//---------------------------------------------------------------------------

// The following methods implement the DeviceDriver aspects of this class.
// By this means, the DriverFeature can be controlled just like the individual
// device drivers are.

int DeviceFeature::open(const char *name, int flags) {

  int minorHandle;
  for (minorHandle = 0; minorHandle < logicalUnitCount; minorHandle++) {
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

int DeviceFeature::status(int handle, int reg, int count, byte *buf) {
  return ESUCCESS;
}

int DeviceFeature::control(int handle, int reg, int count, byte *buf) {
  LogicalUnitInfo *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }
  switch (reg) {
  case DDC_INIT:
    return ESUCCESS;
  default:
    return ESUCCESS;
  }
}

int DeviceFeature::read(int handle, int count, byte *buf) {
  return ESUCCESS;
}

int DeviceFeature::write(int handle, int count, byte *buf) {
  return ESUCCESS;
}

int DeviceFeature::close(int handle) {
  LogicalUnitInfo *currentDevice = &logicalUnits[handle & 0x7F];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
  }
  return ESUCCESS;
}
