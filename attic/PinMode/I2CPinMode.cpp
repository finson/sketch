/*
  I2CPinMode.cpp - Firmata library
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

#include "I2CPinMode.h"

//----------------------------------------------------------------------------

I2CPinMode::I2CPinMode()
{
    enabled = 0;
}

void I2CPinMode::handleGetCapability(byte pin)
{
  if (IS_PIN_I2C(pin)) {
    Firmata.write(I2C);
    Firmata.write(1);  // to do: determine appropriate value
  }
}

boolean I2CPinMode::handleSetPinMode(byte pin, int mode)
{
  if (IS_PIN_I2C(pin)) {
    if (mode == I2C) {
      // the user must call I2C_CONFIG to enable I2C for a device
      return true;
    } else if (isEnabled()) {
      // disable i2c so pins can be used for other functions
      // the following if statements should reconfigure the pins properly
      if (Firmata.getPinMode(pin) == I2C) {
        disableI2CPins();
      }
    }
  }
  return false;
}

void I2CPinMode::reset()
{
  // queryIndex = -1;
  // i2cReadDelayTime = 0;
  // I2CPort.disableI2CPins();

  disableI2CPins();
  enabled = 0;

  // if (isI2CEnabled) {
  //   disableI2CPins();
  // }
}

bool I2CPinMode::isEnabled() {
  return (enabled > 0);
}

void I2CPinMode::enableI2CPins() {
  if (!isEnabled()) {
    byte i;
    for (i = 0; i < TOTAL_PINS; i++) {
      if (IS_PIN_I2C(i)) {
        if (Firmata.getPinMode(i) != IGNORE) {
          // mark pins as i2c so they are ignore in non i2c data requests
          Firmata.setPinMode(i, I2C);
          pinMode(i, I2C);
        }
      }
    }
    Wire.begin();
  }
  enabled += 1;
}

/* disable the i2c pins so they can be used for other functions */
void I2CPinMode::disableI2CPins()
{
  if (isEnabled()) {
    Wire.end();
    enabled -= 1;
  }
}

void I2CPinMode::write8(int addr, uint8_t reg, uint8_t val) {
  if (!isEnabled()) return;

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)val);
  Wire.endTransmission();
}

uint8_t I2CPinMode::read8(int addr, uint8_t reg) {
  uint16_t val;
  if (!isEnabled()) return 0;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)1);
  val = Wire.read();
  return val;
}

void I2CPinMode::write16(int addr, uint8_t reg, uint16_t val) {
  if (!isEnabled()) return;

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write(val >> 8);
  Wire.write(val & 0xFF);
  Wire.endTransmission();
}

uint16_t I2CPinMode::read16(int addr, uint8_t reg) {
  char errorMsg[100] ;
  uint16_t val;
  int numBytes = 2;
  if (!isEnabled()) return 0;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)numBytes);

    // check to be sure correct number of bytes were returned by slave
  if (numBytes < Wire.available()) {
    sprintf(errorMsg, "I2C read16: Too many bytes received from device.  Exp: %1d, Act: %1d",numBytes,Wire.available());
    Firmata.sendString(errorMsg);
  } else if (numBytes > Wire.available()) {
    sprintf(errorMsg, "I2C read16: Too few bytes received from device.  Exp: %1d, Act: %1d",numBytes,Wire.available());
    Firmata.sendString(errorMsg);
  }

  val = Wire.read();
  val <<= 8;
  val |= Wire.read();
  return val;
}

// make one instance to coordinate I2C enable/disable and IO across the app.

I2CPinMode pmI2C;
