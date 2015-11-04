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

  FirmataExtClass::FirmataExtClass()
  {
//   Firmata.attach(SET_PIN_MODE, dispatchSetPinModeCallback);
//   Firmata.attach(dispatchFeatureSysexCallback);
    numFeatures = 0;
  }

  void FirmataExtClass::addFeature(FirmataFeature &capability)
  {
    if (numFeatures < MAX_FEATURES) {
      features[numFeatures++] = &capability;
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
    switch (command) {
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
      return true;
      break;

      default:
      for (byte i = 0; i < numFeatures; i++) {
        if (features[i]->handleFeatureSysex(command, argc, argv)) {
          return true;
        }
      }
      break;
    }
    return false;
  }

  void FirmataExtClass::reset()
  {
    for (byte i = 0; i < numFeatures; i++) {
      features[i]->reset();
    }
  }

//   void dispatchSetPinModeCallback(byte pin, int mode)
//   {
//     if (mode != IGNORE && !FirmataExt.dispatchSetPinMode(pin, mode)) {
//     Firmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
//   }
// }

// void dispatchFeatureSysexCallback(byte command, byte argc, byte* argv)
// {
//   if (!FirmataExt.dispatchFeatureSysex(command, argc, argv)) {
//     Firmata.sendString("Unhandled sysex command");
//   }
// }

// make one static instance
FirmataExtClass FirmataExt;

