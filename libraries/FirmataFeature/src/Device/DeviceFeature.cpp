/*
  DeviceFeature.cpp - Firmata library
*/

#include "DeviceFeature.h"

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature(char *dNameRoot, int count) : DeviceDriver(dNameRoot), majorDeviceCount(0)
{
char buf[MAX_DEVICE_NAME_LENGTH+1];

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

// Initialize the available DeviceFeature pseudo-devices

  minorDeviceCount = min(MAX_MGR_LU_COUNT, count);
  for (int idx = 0; idx < minorDeviceCount; idx++) {
    snprintf(buf, MAX_DEVICE_NAME_LENGTH+1, "%s:%1d", dNameRoot, idx);
    minorDevices[idx].setLogicalUnitName(buf);
    minorDevices[idx].setOpen(false);
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
  int result;
  byte msgBody[MAX_DEVICE_QUERY_BODY_LENGTH + 1];

  if (command != DEVICE_QUERY) {
    return false;
  }

  int action = argv[0];
  int inputLength = argc - 3;
  int outputLength = base64_dec_len((char *)(argv + 3), inputLength);
  if (outputLength > MAX_DEVICE_QUERY_BODY_LENGTH) {
    sendDeviceResponse(action, -1);
    return true;
  }

  //base64_decode(msgBody, (char *)(argv + 3), inputLength);

  result = dispatchDeviceAction(action, argv[1], argv[2], msgBody);
  sendDeviceResponse(action, result);
  return true;
}

int DeviceFeature::dispatchDeviceAction(int act, int minor, int major, byte *body) {
  int deviceIndex;
  int minorHandle;
  int result;
  int flags = (major << 8) | minor;

  switch (act) {
  case DD_OPEN:
    for (deviceIndex = 0; deviceIndex < majorDeviceCount; deviceIndex++) {
      minorHandle = majorDevices[deviceIndex]->open("Hello:0", flags);//TODO body instead of hello
      if (minorHandle != -1) break;
    }
    result = (minorHandle == -1) ? -1 : (((deviceIndex & 0x7F) << 8) | (minorHandle & 0x7F));
    break;
  case DD_STATUS:
    // use the handle to address the driver directly and capture response
    result = -1;
    break;
  case DD_CONTROL:
    result = majorDevices[major]->control(minor,body[0],0,body);
    break;
  case DD_READ:
    result = -1;
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

void DeviceFeature::sendDeviceResponse(int action, int status) {
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
  for (minorHandle = 0; minorHandle < minorDeviceCount; minorHandle++) {
    if (strcmp(minorDevices[minorHandle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }
  if (minorHandle == minorDeviceCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  LogicalUnitInfo currentDevice = minorDevices[minorHandle];
  if (currentDevice.isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }
  currentDevice.setOpen(true);
  return minorHandle;
}

int DeviceFeature::status(int handle, int reg, int count, byte *buf) {}

int DeviceFeature::control(int handle, int reg, int count, byte *buf) {
  int result;
  LogicalUnitInfo currentDevice = minorDevices[handle & 0x7F];
  if (!currentDevice.isOpen()) {
    return -1;
  }
  switch (reg) {
  case DDC_INIT:
    // setSelectedDevices();
    result = max(0,count);
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
  LogicalUnitInfo currentDevice = minorDevices[handle & 0x7F];
  if (currentDevice.isOpen()) {
    currentDevice.setOpen(false);
    return 0;
  } else {
    return -1;
  }
}
