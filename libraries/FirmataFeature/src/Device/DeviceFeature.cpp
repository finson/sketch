/*
  DeviceFeature.cpp - Firmata library
*/

#include "DeviceFeature.h"

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature(char *dName, int count) : DeviceDriver(dName), majorDeviceCount(0)
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

boolean DeviceFeature::handleFeatureSysex(byte command, byte argc, byte *argv)
{
  byte dpv[MAX_DPV_LENGTH];  // decoded parameter vector
  int result;

  if (command != DEVICE_QUERY) {
    return false;
  }

  // The first three bytes of argv are: action, handle-low, handle-high.  They
  // are all constrained to 7-bit values and are not encoded.  The bytes that
  // follow, if any, are the parameter vector. The parameter vector is encoded
  // with base-64 in the sysex message body during transmission to and from this
  // Firmata server.

  int action = argv[0];
  int dpc = base64_dec_len((char *)(argv + 3), argc-3);
  if (dpc > MAX_DPV_LENGTH) {
    sendDeviceResponse(action, -1);
    return true;
  }

  if (dpc > 0) {
    dpc = base64_decode((char *)dpv, (char *)(argv + 3), argc-3);
  }

  result = dispatchDeviceAction(action, argv[1], argv[2], dpc, dpv);

  sendDeviceResponse(action, result, dpv);
  return true;
}

int DeviceFeature::dispatchDeviceAction(int act, int minor, int major, byte *body) {
  int deviceIndex;
  int minorHandle;
  int result;
  int flags = (major << 8) | minor;
  int count;

  switch (act) {
  case DD_OPEN:
    for (deviceIndex = 0; deviceIndex < majorDeviceCount; deviceIndex++) {
      minorHandle = majorDevices[deviceIndex]->open((char *)body, flags);
      if (minorHandle != -1) break;
    }
    result = (minorHandle == -1) ? -1 : (((deviceIndex & 0x7F) << 8) | (minorHandle & 0x7F));
    break;
  case DD_STATUS:
    // use the handle to address the driver directly and capture response
    result = -1;
    break;
  case DD_CONTROL:
    result = majorDevices[major]->control(minor, body[0], 0, body);
    break;
  case DD_READ:
    count = ((body[1] & 0xFF) << 8) | ((body[0] & 0xFF));
    result = majorDevices[major]->read(minor,count,body);
    break;
  case DD_WRITE:
    result = -1;
    break;
  case DD_CLOSE:
    result = (majorDevices[major]->close(minor) == -1) ? -1 : 0;
    break;
  default:
    result = -1;
  }
  return result;
}

------> add the body of the message to this too.

void DeviceFeature::sendDeviceResponse(int action, int status, byte *dpv) {
  byte encodeBuffer[((MAX_DPV_LENGTH+2)/3)*4];

  Firmata.write(START_SYSEX);
  Firmata.write(DEVICE_RESPONSE);
  Firmata.write(action & 0x7F);
  Firmata.write(status & 0x7F);
  Firmata.write((status >> 8) & 0x7F);
  Firmata.write(END_SYSEX);
}

//---------------------------------------------------------------------------

// The following methods implement the DeviceDriver aspects of this class.
// By this means, the DriverFeature can be controlled just like the individual
// device drivers are.

int DeviceFeature::open(char *name, int flags) {
  uint8_t theRegister;

  int minorHandle;
  for (minorHandle = 0; minorHandle < logicalUnitCount; minorHandle++) {
    if (strcmp(logicalUnits[minorHandle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }
  if (minorHandle == logicalUnitCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  LogicalUnitInfo *currentDevice = &logicalUnits[minorHandle];
  if (currentDevice->isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }
  currentDevice->setOpen(true);
  return minorHandle;
}

int DeviceFeature::status(int handle, int reg, int count, byte *buf) {}

int DeviceFeature::control(int handle, int reg, int count, byte *buf) {
  int result;
  LogicalUnitInfo *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return -1;
  }
  switch (reg) {
  case DDC_INIT:
    // setSelectedDevices();
    result = max(0, count);
    break;
  default:
    result = -1;
    break;
  }
  return result;

}

int DeviceFeature::read(int handle, int count, byte *buf) {}
int DeviceFeature::write(int handle, int count, byte *buf) {}

int DeviceFeature::close(int handle) {
  LogicalUnitInfo *currentDevice = &logicalUnits[handle & 0x7F];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
    return 0;
  } else {
    return -1;
  }
}
