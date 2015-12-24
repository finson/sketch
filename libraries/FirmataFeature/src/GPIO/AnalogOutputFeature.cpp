/*
  AnalogOutputFeature.cpp - Firmata library
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

#include <FirmataCore.h>
#include "AnalogOutputFeature.h"

AnalogOutputFeature *AnalogOutputFeatureInstance;

void analogOutputWriteCallback(byte port, int value)
{
  AnalogOutputFeatureInstance->analogWrite(port, value);
}

AnalogOutputFeature::AnalogOutputFeature()
{
  AnalogOutputFeatureInstance = this;
  Firmata.attach(ANALOG_MESSAGE, analogOutputWriteCallback);
}

void AnalogOutputFeature::reset()
{

}

boolean AnalogOutputFeature::handleSetPinMode(byte pin, int mode)
{
  if (mode == PWM && IS_PIN_PWM(pin)) {
    pinMode(PIN_TO_PWM(pin), OUTPUT);
    analogWrite(PIN_TO_PWM(pin), 0);
    return true;
  }
  return false;
}

void AnalogOutputFeature::handleGetCapability(byte pin)
{
  if (IS_PIN_PWM(pin)) {
    Firmata.write(PWM);
    Firmata.write(8);
  }
}

boolean AnalogOutputFeature::handleFeatureSysex(byte command, byte argc, byte* argv)
{
  if (command == EXTENDED_ANALOG) {
    if (argc > 1) {
      int val = argv[1];
      if (argc > 2) val |= (argv[2] << 7);
      if (argc > 3) val |= (argv[3] << 14);
      analogWrite(argv[0], val);
      return true;
    }
  }
  return false;
}

void AnalogOutputFeature::analogWrite(byte pin, int value)
{
  if (pin < TOTAL_PINS) {
    switch (Firmata.getPinMode(pin)) {
#ifdef ServoFirmata_h
      case SERVO:
        if (IS_PIN_SERVO(pin)) {
          servoAnalogWrite(pin, value);
          Firmata.setPinState(pin, value);
        }
        break;
#endif
#ifdef AnalogOutputFeature_h
      case PWM:
        if (IS_PIN_PWM(pin)) {
          analogWrite(PIN_TO_PWM(pin), value);
          Firmata.setPinState(pin, value);
        }
        break;
#endif
    }
  }
}

