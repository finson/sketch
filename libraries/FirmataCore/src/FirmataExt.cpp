/*
  FirmataExt.h - Firmata library
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
#include <FirmataExt.h>
#include <DeviceDriver.h>

extern FirmataFeature *selectedFeatures[];
extern DeviceDriver *selectedDevices[];

//----------------------------------------------------------------------------

FirmataExtClass::FirmataExtClass() : numFeatures(0)
{
}

void FirmataExtClass::addSelectedFeatures()
{
  byte msg[3];

// Install the user-selected FirmataFeatures.

  int selectionIndex = 0;
  while (selectedFeatures[selectionIndex] != 0) {
    if (numFeatures < MAX_FEATURES) {
      features[numFeatures++] = selectedFeatures[selectionIndex];
    }
    selectionIndex += 1;
  }

// Ask DeviceFeature to install the user-selected DeviceDrivers.
// If DeviceFeature is not installed, the Sysex command will just fall on
// the floor.

  msg[0] = DD_OPEN;
  msg[1] = 0;
  msg[2] = 0x7E;

  dispatchFeatureSysex(DEVICE_QUERY, 3, msg);
}

void FirmataExtClass::dispatchReset()
{
  for (byte i = 0; i < numFeatures; i++) {
    features[i]->reset();
  }
}

boolean FirmataExtClass::dispatchSetPinMode(byte pin, int mode)
{
  boolean known = false;
  for (byte i = 0; i < numFeatures; i++) {
    known |= features[i]->handleSetPinMode(pin, mode);
  }
  return known;
}

boolean FirmataExtClass::dispatchFeatureSysex(byte command, byte argc, byte* argv)
{
  if (this->handleFeatureSysex(command, argc, argv)) {
    return true;
  }

  for (byte i = 0; i < numFeatures; i++) {
    if (features[i]->handleFeatureSysex(command, argc, argv)) {
      return true;
    }
  }
  return false;
}

boolean FirmataExtClass::handleFeatureSysex(byte cmd, byte argc, byte* argv)
{
  switch (cmd) {
  case CAPABILITY_QUERY:
    Firmata.write(START_SYSEX);
    Firmata.write(CAPABILITY_RESPONSE);
    for (byte pin = 0; pin < TOTAL_PINS; pin++) {
      if (Firmata.getPinMode(pin) != IGNORE) {
        for (byte i = 0; i < numFeatures; i++) {
          features[i]->handleGetCapability(pin);
        }
      }
      Firmata.write(127);
    }
    Firmata.write(END_SYSEX);
    break;

  case PIN_STATE_QUERY:
    if (argc >= 1) {
      byte pin = argv[0];
      if (pin < TOTAL_PINS) {
        Firmata.write(START_SYSEX);
        Firmata.write(PIN_STATE_RESPONSE);
        Firmata.write(pin);
        Firmata.write(Firmata.getPinMode(pin));
        int pinState = Firmata.getPinState(pin);
        Firmata.write((byte)pinState & 0x7F);
        if (pinState & 0xFF80) Firmata.write((byte)(pinState >> 7) & 0x7F);
        if (pinState & 0xC000) Firmata.write((byte)(pinState >> 14) & 0x7F);
        Firmata.write(END_SYSEX);
      }
    }
    break;

  case ANALOG_MAPPING_QUERY:
    Firmata.write(START_SYSEX);
    Firmata.write(ANALOG_MAPPING_RESPONSE);
    for (byte pin = 0; pin < TOTAL_PINS; pin++) {
      Firmata.write(IS_PIN_ANALOG(pin) ? PIN_TO_ANALOG(pin) : 127);
    }
    Firmata.write(END_SYSEX);
    break;

  default:
    return false;
  }
  return true;
}

// make one static instance
FirmataExtClass FirmataExt;
