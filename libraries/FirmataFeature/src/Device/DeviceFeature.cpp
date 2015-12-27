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

  // The first four bytes of argv for DEVICE_QUERY messages are: action,
  // reserved, handle-low, handle-high. They are all constrained to 7-bit
  // values and are not encoded.  The bytes that follow, if any, are the
  // parameter block. The parameter block is encoded with base-64 in the
  // sysex message body during transmission to and from this Firmata
  // server.

boolean DeviceFeature::handleFeatureSysex(byte command, byte argc, byte *argv)
{
  byte dpBlock[MAX_DPB_LENGTH];  // decoded parameter block

  if (command != DEVICE_QUERY) {
    return false;
  }

  int action = argv[0];
  int handle = (argv[3] << 8) | argv[2];

  int dpCount = base64_dec_len((char *)(argv + 4), argc-4);
  if (dpCount > MAX_DPB_LENGTH) {
    sendDeviceResponse(action, EMSGSIZE, handle);
    return true;
  }

  if (dpCount > 0) {
    dpCount = base64_decode((char *)dpBlock, (char *)(argv + 4), argc-4);
  }

  int status = dispatchDeviceAction(action, &handle, &dpCount, dpBlock);

  sendDeviceResponse(action, status, handle, dpCount, dpBlock);
  return true;
}

int DeviceFeature::dispatchDeviceAction(int action, int *handle, int *bodyCount, byte *body) {
  int flags;
  int deviceIndex;
  int status = ENODEV;
  int count;
  int reg;
  int unitHandle = (*handle & 0x7F);
  int deviceHandle = ((*handle >> 8) & 0x7F);

  switch (action) {
  case DD_OPEN:
    flags = *handle;
    for (deviceIndex = 0; deviceIndex < majorDeviceCount; deviceIndex++) {
      status = majorDevices[deviceIndex]->open(&unitHandle, (char *)body, flags);
      if (status == ESUCCESS) break;
      if (status == ENXIO || status == ENODEV) {
        continue;
      } else {
        break;
      }
    }
    *handle = (status == ESUCCESS) ? ((deviceIndex & 0x7F) << 8) | (unitHandle & 0x7F) : 0;
    *bodyCount = 0;
    return status;

  case DD_STATUS:
    count = ((body[1] & 0xFF) << 8) | (body[0] & 0xFF);
    reg =   ((body[3] & 0xFF) << 8) | (body[2] & 0xFF);
    status = majorDevices[deviceHandle]->status(unitHandle,reg,count,body);
    *bodyCount = (status == ESUCCESS) ? count : 0;
    return status;

  case DD_CONTROL:
    count = ((body[1] & 0xFF) << 8)  | (body[0] & 0xFF);
    reg =   ((body[3] & 0xFF) << 8)  | (body[2] & 0xFF);
    status = majorDevices[deviceHandle]->control(unitHandle, reg, count, body+4);
    *bodyCount = 0;
    return status;

  case DD_READ:
    count = ((body[1] & 0xFF) << 8) | ((body[0] & 0xFF));
    status = majorDevices[deviceHandle]->read(unitHandle,count,body);
    *bodyCount = (status == ESUCCESS) ? count : 0;
    return status;

  case DD_WRITE:
    count = ((body[1] & 0xFF) << 8) | ((body[0] & 0xFF));
    status = majorDevices[deviceHandle]->read(unitHandle,count,body+2);
    *bodyCount =  0;
    return status;

  case DD_CLOSE:
    status = majorDevices[deviceHandle]->close(unitHandle);
    *bodyCount =  0;
    return status;

  default:
    return ENOSYS;
  }
}

void DeviceFeature::sendDeviceResponse(int action, int status, int handle) {
  sendDeviceResponse(action,status,handle, 0 , 0);
}

void DeviceFeature::sendDeviceResponse(int action, int status, int handle, int dpCount, byte *dpBlock) {
  byte encodeBuffer[1+((MAX_DPB_LENGTH+2)/3)*4];

  Firmata.write(START_SYSEX);
  Firmata.write(DEVICE_RESPONSE);
  Firmata.write(action & 0x7F);
  Firmata.write(status & 0x7F);
  Firmata.write(handle & 0x7F);
  Firmata.write((handle >> 8) & 0x7F);
  if (dpCount > 0 && dpCount <= MAX_DPB_LENGTH) {
    int epCount = base64_encode((char *)encodeBuffer, (char *)dpBlock, dpCount);
    for (int idx=0; idx < epCount; idx++) {
      Firmata.write(encodeBuffer[idx]);
    }
  }
  Firmata.write(END_SYSEX);
}

//---------------------------------------------------------------------------

// The following methods implement the DeviceDriver aspects of this class.
// By this means, the DriverFeature can be controlled just like the individual
// device drivers are.

int DeviceFeature::open(int *handle, const char *name, int flags) {

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
  *handle = minorHandle;
  return ESUCCESS;
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
