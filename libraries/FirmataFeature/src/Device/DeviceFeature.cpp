/*
  DeviceFeature.cpp - Firmata library
*/

#include "DeviceFeature.h"
#include <Base64.h>

extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

DeviceFeature::DeviceFeature(const char *dName) {
  dt = new DeviceTable(selectedDevices);
}

//---------------------------------------------------------------------------

void DeviceFeature::reset(){}

void DeviceFeature::handleGetCapability(byte pin){}

boolean DeviceFeature::handleSetPinMode(byte pin, int mode) {
  return false;
}

void DeviceFeature::update(unsigned long deltaMicros) {
  dt->update(deltaMicros);
}

void DeviceFeature::report(unsigned long deltaMillis) {
  dt->update(deltaMillis);
}

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
    sendDeviceResponse(handle, action, EMSGSIZE);
    return true;
  }

  if (dpCount > 0) {
    dpCount = base64_decode((char *)dpBlock, (char *)(argv + 6), argc - 6);
  }

  int status = dispatchDeviceAction(action, handle, dpCount, dpBlock);

  sendDeviceResponse(handle, action, status, dpBlock);
  return true;
}

//---------------------------------------------------------------------------

int DeviceFeature::dispatchDeviceAction(int action, int handle, int dpCount, byte *dpBlock) {
  // int deviceIndex = 0;
  int status = ENODEV;
  int count = 0;
  int reg = 0;
  // int unitHandle = (handle & 0x7F);
  // int deviceHandle = ((handle >> 7) & 0x7F);
  int flags = 0;

  switch (action) {
  case DD_OPEN:
    flags = handle;
    return dt->open((char *)dpBlock, flags);
    // for (deviceIndex = 0; deviceIndex < majorDeviceCount; deviceIndex++) {
    //   status = majorDevices[deviceIndex]->open((char *)dpBlock, flags);
    //   if (status == ENXIO || status == ENODEV) {
    //     continue;
    //   } else {
    //     break;
    //   }
    // }
    // return (status < 0) ? status : ((deviceIndex & 0x7F) << 7) | (status & 0x7F);

  case DD_STATUS:
    count = ((dpBlock[1] & 0xFF) << 8) | (dpBlock[0] & 0xFF);
    reg   = ((dpBlock[3] & 0xFF) << 8) | (dpBlock[2] & 0xFF);
    return dt->status(handle,reg, count, dpBlock);
    // return majorDevices[deviceHandle]->status(unitHandle, reg, count, dpBlock);

  case DD_CONTROL:
    count = ((dpBlock[1] & 0xFF) << 8)  | (dpBlock[0] & 0xFF);
    reg   = ((dpBlock[3] & 0xFF) << 8)  | (dpBlock[2] & 0xFF);
    return dt->control(handle, reg, count, dpBlock + 4);

    // return majorDevices[deviceHandle]->control(unitHandle, reg, count, dpBlock + 4);

  case DD_READ:
    count = ((dpBlock[1] & 0xFF) << 8) | ((dpBlock[0] & 0xFF));
    return dt->read(handle, count, dpBlock);
    // return majorDevices[deviceHandle]->read(unitHandle, count, dpBlock);

  case DD_WRITE:
    count = ((dpBlock[1] & 0xFF) << 8) | ((dpBlock[0] & 0xFF));
    return dt->read(handle, count, dpBlock + 2);
    // return majorDevices[deviceHandle]->read(unitHandle, count, dpBlock + 2);

  case DD_CLOSE:
    return dt->close(handle);
    // return majorDevices[deviceHandle]->close(unitHandle);

  default:
    return ENOSYS;
  }
}

//  dpB -> decoded parameter block
//  epB -> encoded parameter block

void DeviceFeature::sendDeviceResponse(int handle, int action, int status) {
  sendDeviceResponse(handle, action, status, 0);
}

void DeviceFeature::sendDeviceResponse(int handle, int action, int status, const byte *dpB) {
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
