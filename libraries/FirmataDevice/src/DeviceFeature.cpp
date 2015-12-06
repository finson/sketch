/*
  DeviceFeature.cpp - Firmata library
*/

#include <DeviceFeature.h>

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature()
{
  int selectionIndex = 0;
  while (selectedDevices[selectionIndex] != 0) {
    addDevice(selectedDevices[selectionIndex++]);
  }
}

void DeviceFeature::addDevice(DeviceDriver *device) {
  if (numDevices < MAX_MAJOR_HANDLE_COUNT) {
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
  char errorMsg[100] ;
  char msgBody[MAX_DEVICE_QUERY_BODY_LENGTH + 1];
  char buf[32];

  msgBody[0] = '\0';

  // sprintf(buf, "Free Ram: %1d", freeRam());
  // Firmata.sendString(buf);

  if (command == DEVICE_QUERY) {
    int result;
    int flags;

    int action = argv[0];
    int minorHandle = argv[1];
    int majorHandle = argv[2];

    Firmata.sendString("Hello 1 from DeviceFeature.cpp!");

    // sprintf(errorMsg, "DEVICE_QUERY info: action: %1d, minor Handle: %1d, majorHandle: %1d", action, minorHandle, majorHandle);
    // Firmata.sendString(errorMsg);

    // int inputLength = argc - 3;
    // int outputLength = base64_dec_len((char *)(argv + 3),inputLength);
    // if (outputLength > MAX_DEVICE_QUERY_BODY_LENGTH) {
    //   sendDeviceResponse(action, -1);
    //   return true;
    // }

    //base64_decode(msgBody, (char *)(argv + 3), inputLength);

    // sprintf(errorMsg, "DEVICE_QUERY info: inputLength: %1d, outputLength: %1d", inputLength, outputLength);
    // Firmata.sendString(errorMsg);

    //sprintf(errorMsg, "DEVICE_QUERY info: msgBody: %s", msgBody);
    Firmata.sendString("Hello 2 from DeviceFeature.cpp.");

    result = -1;
    switch (action) {
    case DD_OPEN:
      Firmata.sendString("Hello 3 from DeviceFeature.cpp.");
      flags = (majorHandle << 8) | minorHandle;
      for (majorHandle = 0; majorHandle < numDevices; majorHandle++) {
        minorHandle = devices[majorHandle]->open("Hello:0", flags);
        if (minorHandle != -1) break;
      }
      result = (majorHandle == numDevices) ? -1 : (((majorHandle << 8) & 0x7F) | (minorHandle & 0x7F));
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
      result = devices[majorHandle]->close(minorHandle);
      break;
    default:
      ;// unknown action code
    }
    sendDeviceResponse(action, result);
    Firmata.sendString("Hello 4 from DeviceFeature.cpp.");
    return true;
  } else {
    Firmata.sendString("Hello 5 from DeviceFeature.cpp.");
    return false;
  }
}

void DeviceFeature::sendDeviceResponse(int action, int status) {
  Firmata.write(START_SYSEX);
  Firmata.write(DEVICE_RESPONSE);
  Firmata.write(action & 0x7F);
  Firmata.write(status & 0x7F);
  Firmata.write((status >> 8) & 0x7F);
  Firmata.write(END_SYSEX);
}
