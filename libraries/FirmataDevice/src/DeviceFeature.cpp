/*
  DeviceFeature.cpp - Firmata library
*/

#include <DeviceFeature.h>

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature()
{
  numDevices = 0;
  while (selectedDevices[numDevices] != 0) {
    addDevice(selectedDevices[numDevices++]);
  }
}

void DeviceFeature::addDevice(DeviceDriver *device) {
  if (numDevices < MAX_DEVICES) {
    devices[numDevices++] = device;
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
  if (command == DEVICE_QUERY) {
      // decode msg to: action, handle/flags, and body
    int action = argv[0];  // edit after writing the decode stuff above
    switch (action) {
      case DD_OPEN:
      // loop through known devices and capture response
      break;
      case DD_STATUS:
            // use the handle to address the driver directly and capture response
      break;
      case DD_CONTROL:
      break;
      case DD_READ:
      break;
      case DD_WRITE:
      break;
      case DD_CLOSE:
      break;
      default:
      ;// unknown action code
    }
    // formulate, encode, and send response msg
    return true;
  }
  return false;
}
