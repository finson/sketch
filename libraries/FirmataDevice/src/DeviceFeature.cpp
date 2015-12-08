/*
  DeviceFeature.cpp - Firmata library
*/

#include <DeviceFeature.h>

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature(char *dNameRoot, int count) : DeviceDriver(dNameRoot), numDevices(0)
{
  actualMinorHandleCount = min(MAX_DEVICE_FEATURE_MINOR_HANDLE_COUNT, addrCount);
  for (int idx = 0; idx < actualMinorHandleCount; idx++) {
    sprintf(buf, "%s:%1d", dNameRoot, idx);
    devices[idx].setDeviceName(buf);
    devices[idx].setOpen(false);
  }
}

void DeviceFeature::setSelectedDevices(int flags) {
  numDevices = 0;
  devices[numDevices++] = this;

  int selectionIndex = 0;
  while (selectedDevices[selectionIndex] != 0) {
    if (numDevices < MAX_MAJOR_HANDLE_COUNT) {
      devices[numDevices++] = selectedDevices[selectionIndex];
    }
    selectionIndex += 1;
  }
}

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
  char msgBody[MAX_DEVICE_QUERY_BODY_LENGTH + 1];

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

  result = executeDeviceAction(action, argv[1], argv[2], msgBody);
  sendDeviceResponse(action, result);
  return true;
}

int DeviceFeature::executeDeviceAction(int act, int minor, int major, char *body) {
  int deviceIndex;
  int minorHandle;
  int result;
  int flags;

  switch (act) {
  case DD_OPEN:
    flags = (major << 8) | minor;
    for (deviceIndex = 0; deviceIndex < numDevices; deviceIndex++) {
      minorHandle = devices[deviceIndex]->open("Hello:0", flags);//TODO body instead of hello
      if (minorHandle != -1) break;
    }
    result = (minorHandle == -1) ? -1 : (((deviceIndex & 0x7F) << 8) | (minorHandle & 0x7F));
    break;
  case DD_STATUS:
    // use the handle to address the driver directly and capture response
    result = -1;
    break;
  case DD_CONTROL:
    result = devices[major]->control(minor,body[0],0,body);
    break;
  case DD_READ:
    result = -1;
    break;
  case DD_WRITE:
    result = -1;
    break;
  case DD_CLOSE:
    result = (devices[major]->close(minor) == -1) ? -1 : 0;
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
//
// The following methods implement the DeviceDriver aspect of this class.
// By this means, the DriverFeature can be controlled just like the individual
// device drivers are.

int open(char *name, int flags) {
  uint8_t theRegister;

  int minorHandle;
  for (minorHandle = 0; minorHandle < actualMinorHandleCount; minorHandle++) {
    if (strcmp(devices[minorHandle].getDeviceName(), name) == 0) {
      break;
    }
  }
  if (minorHandle == actualMinorHandleCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  DeviceInfo currentDevice = devices[minorHandle];
  if (currentDevice.isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }
  currentDevice.setOpen(true);
  return minorHandle;
}

int status(int handle, int reg, int count, byte *buf) {}

int control(int handle, int reg, int count, byte *buf) {
  int result;
  DeviceInfo currentDevice = devices[handle & 0x7F];
  if (!currentDevice.isOpen()) {
    return -1;
  }
  switch (reg) {
  case DDC_INIT:
    setSelectedDevices(flags);
    result = max(0,count);
    break;
  default:
    result = -1;
    break;
  }
  return result;

}

int read(int handle, int count, byte *buf) {}
int write(int handle, int count, byte *buf) {}

int close(int handle) {
  DeviceInfo currentDevice = devices[handle & 0x7F];
  if (currentDevice.isOpen()) {
    currentDevice.setOpen(false);
    return 0;
  } else {
    return -1;
  }
}
