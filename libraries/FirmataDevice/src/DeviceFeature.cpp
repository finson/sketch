/*
  DeviceFeature.cpp - Firmata library
*/

#include <DeviceFeature.h>

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature() : numDevices(0)
{
}

void DeviceFeature::addSelectedDevices() {
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
  char errorMsg[100] ;
  char msgBody[MAX_DEVICE_QUERY_BODY_LENGTH + 1];
  char buf[32];

  msgBody[0] = '\0';

  if (command != DEVICE_QUERY) {
    return false;
  }

  int result;
  int flags;

  int action = argv[0];
  int minorHandle = argv[1];
  int majorHandle = argv[2];

  Firmata.sendString("Marker 1 from DeviceFeature.cpp.");

  sprintf(errorMsg, "DEVICE_QUERY info: action: %1d, minor Handle: %1d, majorHandle: %1d", action, minorHandle, majorHandle);
  Firmata.sendString(errorMsg);

  // Does the caller want to address this DeviceFeature object directly?
  // If so, use the distinguished major handle value 0x7E to select the
  // DeviceFeature itself rather than one of the actual devices.

  // Any of the device action values can be adopted to control the
  // DeviceFeature as needed.

  if (majorHandle == 0x7E) {
    switch (action) {
    case DD_OPEN:
      addSelectedDevices();
      break;
    default:
      break;
    }
    return true;
  }

  int inputLength = argc - 3;
  int outputLength = base64_dec_len((char *)(argv + 3), inputLength);
  if (outputLength > MAX_DEVICE_QUERY_BODY_LENGTH) {
    // sendDeviceResponse(action, -1);
    return true;
  }

  //base64_decode(msgBody, (char *)(argv + 3), inputLength);

  // sprintf(errorMsg, "DEVICE_QUERY info: inputLength: %1d, outputLength: %1d", inputLength, outputLength);
  // Firmata.sendString(errorMsg);

  //sprintf(errorMsg, "DEVICE_QUERY info: msgBody: %s", msgBody);
  Firmata.sendString("Marker 2 from DeviceFeature.cpp.");

  result = -1;
  switch (action) {
  case DD_OPEN:
    Firmata.sendString("Marker 3 from DeviceFeature.cpp.");
    flags = (majorHandle << 8) | minorHandle;
    for (majorHandle = 0; majorHandle < numDevices; majorHandle++) {
      minorHandle = devices[majorHandle]->open("Hello:0", flags);
      if (minorHandle != -1) break;
    }
    result = (majorHandle == numDevices) ? -1 : (((majorHandle & 0x7F) << 8) | (minorHandle & 0x7F));
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
  Firmata.sendString("Marker 4 from DeviceFeature.cpp.");
  return true;
}

void DeviceFeature::sendDeviceResponse(int action, int status) {
  Firmata.write(START_SYSEX);
  Firmata.write(DEVICE_RESPONSE);
  Firmata.write(action & 0x7F);
  Firmata.write(status & 0x7F);
  Firmata.write((status >> 8) & 0x7F);
  Firmata.write(END_SYSEX);
}
