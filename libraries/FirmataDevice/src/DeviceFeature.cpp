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

  if (command == DEVICE_QUERY) {
    int result;
    int flags;

    int action = argv[0];
    int minorHandle = argv[1];
    int majorHandle = argv[2];

    // sprintf(errorMsg, "DEVICE_QUERY info: action: %1d, minor Handle: %1d, majorHandle: %1d", action, minorHandle, majorHandle);
    // Firmata.sendString(errorMsg);

    // int inputLength = argc - 3;
    // int outputLength = base64_dec_len((char *)(argv + 3),inputLength);
    // if (outputLength > MAX_DEVICE_QUERY_BODY_LENGTH) {
    //   sendSysexResponse(action, -1);
    //   return true;
    // }
    // //base64_decode(msgBody, (char *)(argv + 3), inputLength);

    // sprintf(errorMsg, "DEVICE_QUERY info: inputLength: %1d, outputLength: %1d", inputLength, outputLength);
    // Firmata.sendString(errorMsg);

    //strcpy(msgBody,"MCP9808:0");
    // msgBody[0] = 'M';
    // msgBody[1] = 'C';
    // msgBody[2] = 'P';
    // msgBody[3] = '9';
    // msgBody[4] = '8';
    // msgBody[5] = '0';
    // msgBody[6] = '8';
    // msgBody[7] = ':';
    // msgBody[8] = '0';
    // msgBody[9] = '\0';

    // sprintf(errorMsg, "Hello %s!","MCP9808");
 //   sprintf(errorMsg, "DEVICE_QUERY info: msgBody: %s", msgBody);
    Firmata.sendString("MCP9808");

    switch (action) {
    case DD_OPEN:
      flags = (majorHandle << 8) | minorHandle;
      for (majorHandle = 0; majorHandle < numDevices; majorHandle++) {
        minorHandle = devices[majorHandle]->open(msgBody, flags);
        if (minorHandle != -1) break;
      }
      result = (majorHandle == numDevices) ? -1 : (((majorHandle << 8) & 0x7F) | (minorHandle & 0x7F));
      sendSysexResponse(action, result);
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
      sendSysexResponse(action, result);
      break;
    default:
      ;// unknown action code
    }
    return true;
  } else {
    return false;
  }
}

void DeviceFeature::sendSysexResponse(int action, int status) {
  Firmata.write(START_SYSEX);
  Firmata.write(DEVICE_RESPONSE);
  Firmata.write(action & 0x7F);
  Firmata.write(status & 0x7F);
  Firmata.write((status << 8) & 0x7F);
  Firmata.write(END_SYSEX);
}
