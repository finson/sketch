/*
  I2CFeature.cpp - Firmata library
  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2010-2011 Paul Stoffregen.  All rights reserved.
  Copyright (C) 2009 Shigeru Kobayashi.  All rights reserved.
  Copyright (C) 2009-2011 Jeff Hoefs.  All rights reserved.
  Copyright (C) 2013 Norbert Truchsess. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "I2CFeature.h"
#include "I2CPort.h"

//----------------------------------------------------------------------------

I2CFeature::I2CFeature() : queryIndex(-1), i2cReadDelayTime(0)
{
  // queryIndex = -1;
  // i2cReadDelayTime = 0;  // default delay time between i2c read request and Wire.requestFrom()
}

void I2CFeature::handleGetCapability(byte pin)
{
  if (IS_PIN_I2C(pin)) {
    Firmata.write(I2C);
    Firmata.write(1);  // to do: determine appropriate value
  }
}

boolean I2CFeature::handleSetPinMode(byte pin, int mode)
{
  if (IS_PIN_I2C(pin)) {
    if (mode == I2C) {
      // the user must call I2C_CONFIG to enable I2C for a device
      return true;
    } else if (I2CPort.isEnabled()) {
      // disable i2c so pins can be used for other functions
      // the following if statements should reconfigure the pins properly
      if (Firmata.getPinMode(pin) == I2C) {
        I2CPort.disableI2CPins();
      }
    }
  }
  return false;
}

boolean I2CFeature::handleFeatureSysex(byte command, byte argc, byte *argv)
{
  switch (command) {
  case I2C_REQUEST:
    if (I2CPort.isEnabled()) {
      handleI2CRequest(argc, argv);
      return true;
    }
  case I2C_CONFIG:
    return handleI2CConfig(argc, argv);
  }
  return false;
}

void I2CFeature::reset()
{
  queryIndex = -1;
  i2cReadDelayTime = 0;
  I2CPort.disableI2CPins();

  // if (isI2CEnabled) {
  //   disableI2CPins();
  // }
}

//----------------------------------------------------------------------------

boolean I2CFeature::handleI2CConfig(byte argc, byte *argv)
{
  unsigned int delayTime = (argv[0] + (argv[1] << 7));

  if (delayTime > 0) {
    i2cReadDelayTime = delayTime;
  }

  I2CPort.enableI2CPins();
  return I2CPort.isEnabled();
}

void I2CFeature::handleI2CRequest(byte argc, byte *argv)
{
  byte mode;
  byte slaveAddress;
  byte data;
  int slaveRegister;
  mode = argv[1] & I2C_READ_WRITE_MODE_MASK;
  if (argv[1] & I2C_10BIT_ADDRESS_MODE_MASK) {
    Firmata.sendString("10-bit addressing not supported");
    return;
  }
  else {
    slaveAddress = argv[0];
  }

  switch (mode) {
  case I2C_WRITE:
    Wire.beginTransmission(slaveAddress);
    for (byte i = 2; i < argc; i += 2) {
      data = argv[i] + (argv[i + 1] << 7);
#if ARDUINO >= 100
      Wire.write(data);
#else
      Wire.send(data);
#endif
    }
    Wire.endTransmission();
    delayMicroseconds(70);
    break;
  case I2C_READ:
    if (argc == 6) {
      // a slave register is specified
      slaveRegister = argv[2] + (argv[3] << 7);
      data = argv[4] + (argv[5] << 7);  // bytes to read
      readAndReportData(slaveAddress, (int)slaveRegister, data);
    }
    else {
      // a slave register is NOT specified
      data = argv[2] + (argv[3] << 7);  // bytes to read
      readAndReportData(slaveAddress, (int)REGISTER_NOT_SPECIFIED, data);
    }
    break;
  case I2C_READ_CONTINUOUSLY:
    if ((queryIndex + 1) >= MAX_QUERIES) {
      // too many queries, just ignore
      Firmata.sendString("too many queries");
      break;
    }
    if (argc == 6) {
      // a slave register is specified
      slaveRegister = argv[2] + (argv[3] << 7);
      data = argv[4] + (argv[5] << 7);  // bytes to read
    }
    else {
      // a slave register is NOT specified
      slaveRegister = (int)REGISTER_NOT_SPECIFIED;
      data = argv[2] + (argv[3] << 7);  // bytes to read
    }
    queryIndex++;
    query[queryIndex].addr = slaveAddress;
    query[queryIndex].reg = slaveRegister;
    query[queryIndex].bytes = data;
    break;
  case I2C_STOP_READING:
    byte queryIndexToSkip;
    // if read continuous mode is enabled for only 1 i2c device, disable
    // read continuous reporting for that device
    if (queryIndex <= 0) {
      queryIndex = -1;
    } else {
      // if read continuous mode is enabled for multiple devices,
      // determine which device to stop reading and remove it's data from
      // the array, shifiting other array data to fill the space
      for (byte i = 0; i < queryIndex + 1; i++) {
        if (query[i].addr == slaveAddress) {
          queryIndexToSkip = i;
          break;
        }
      }

      for (byte i = queryIndexToSkip; i < queryIndex + 1; i++) {
        if (i < MAX_QUERIES) {
          query[i].addr = query[i + 1].addr;
          query[i].reg = query[i + 1].reg;
          query[i].bytes = query[i + 1].bytes;
        }
      }
      queryIndex--;
    }
    break;
  default:
    break;
  }
}

void I2CFeature::readAndReportData(byte address, int theRegister, byte numBytes) {
  char errorMsg[100] ;
  // allow I2C requests that don't require a register read
  // for example, some devices using an interrupt pin to signify new data available
  // do not always require the register read so upon interrupt you call Wire.requestFrom()

  if (numBytes == 2) {
    sprintf(errorMsg, "I2C info: address: %1d, theRegister: %1d, numBytes: %1d", address, theRegister, numBytes);
    Firmata.sendString(errorMsg);

    int value = I2CPort.read16(address, theRegister);

    i2cRxData[0] = address;
    i2cRxData[1] = theRegister;
    i2cRxData[2] = (value >> 8) & 0xFF;
    i2cRxData[3] = value & 0xFF;

  } else {

    if (theRegister != REGISTER_NOT_SPECIFIED) {
      Wire.beginTransmission(address);
#if ARDUINO >= 100
      Wire.write((byte)theRegister);
#else
      Wire.send((byte)theRegister);
#endif
      Wire.endTransmission();
      // do not set a value of 0
      if (i2cReadDelayTime > 0) {
        // delay is necessary for some devices such as WiiNunchuck
        delayMicroseconds(i2cReadDelayTime);
      }
    } else {
      theRegister = 0;  // fill the register with a dummy value
    }

    Wire.requestFrom(address, numBytes);  // all bytes are returned in requestFrom

    // check to be sure correct number of bytes were returned by slave
    if (numBytes < Wire.available()) {
      sprintf(errorMsg, "I2C: Too many bytes received from device.  Exp: %1d, Act: %1d", numBytes, Wire.available());
      Firmata.sendString(errorMsg);
    } else if (numBytes > Wire.available()) {
      sprintf(errorMsg, "I2C: Too few bytes received from device.  Exp: %1d, Act: %1d", numBytes, Wire.available());
      Firmata.sendString(errorMsg);
    }

    i2cRxData[0] = address;
    i2cRxData[1] = theRegister;
    i2cRxData[2] = 0xF2;  // for debug
    i2cRxData[3] = 0xF3;  // for debug

    for (int i = 0; i < numBytes && Wire.available(); i++) {
#if ARDUINO >= 100
      i2cRxData[2 + i] = Wire.read();
#else
      i2cRxData[2 + i] = Wire.receive();
#endif
    }
  }
  // send slave address, register and received bytes
  Firmata.sendSysex(I2C_REPLY, numBytes + 2, i2cRxData);
}

// void I2CFeature::report()
// {
//   // report i2c data for all device with read continuous mode enabled
//   if (queryIndex > -1) {
//     for (byte i = 0; i < queryIndex + 1; i++) {
//       readAndReportData(query[i].addr, query[i].reg, query[i].bytes);
//     }
//   }
// }
