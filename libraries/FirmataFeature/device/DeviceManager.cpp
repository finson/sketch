/*
  DeviceManager.cpp - Firmata library
*/

#include <DeviceManager.h>

//----------------------------------------------------------------------------

DeviceManager::DeviceManager()
{
    numDevices = 0;
}

void DeviceManager::addDevice(DeviceDriver *device) {
  if (numDevices < MAX_DEVICES) {
    devices[numDevices++] = device;
  }
}

void DeviceManager::reset()
{
}

void DeviceManager::handleGetCapability(byte pin)
{
}

boolean DeviceManager::handleSetPinMode(byte pin, int mode)
{
  return false;
}

boolean DeviceManager::handleFeatureSysex(byte command, byte argc, byte *argv)
{
  if (command == DEVICE_QUERY) {
      // decode msg to: action, handle/flags, and body
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
      // unknown action code
    }
    // formulate encode, and send response msg
    return true;
  }
  return false;
}
