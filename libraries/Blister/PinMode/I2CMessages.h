/*
  I2CMessages.h - Firmata library
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

#ifndef I2CMessages_h
#define I2CMessages_h

#include "PinMessages.h"
#include "I2CPinMode.h"
#include <Wire.h>

#define I2C_WRITE B00000000
#define I2C_READ B00001000
#define I2C_READ_CONTINUOUSLY B00010000
#define I2C_STOP_READING B00011000
#define I2C_READ_WRITE_MODE_MASK B00011000
#define I2C_10BIT_ADDRESS_MODE_MASK B00100000

#define MAX_QUERIES 8

#define REGISTER_NOT_SPECIFIED -1

/* i2c data */
struct i2c_device_info {
  byte addr;
  int reg;
  byte bytes;
};

class I2CMessages: public PinMessages
{
  public:
    I2CMessages();

    boolean handleFeatureSysex(byte command, byte argc, byte* argv);

  private:
    signed char queryIndex;
    unsigned int i2cReadDelayTime;  // default delay time between i2c read request and Wire.requestFrom()

    /* for i2c read continuous more */
    i2c_device_info query[MAX_QUERIES];

    byte i2cRxData[32];

    void readAndReportData(byte address, int theRegister, byte numBytes);
    void handleI2CRequest(byte argc, byte *argv);
    boolean handleI2CConfig(byte argc, byte *argv);
};

extern I2CPinMode pmI2C;

#endif

