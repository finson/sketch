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

#include <FirmataExt.h>
#include <limits.h>

//----------------------------------------------------------------------------

FirmataExtClass::FirmataExtClass()
{
  featureCount = 0;
  previousTime[0] = 0;
  previousTime[1] = 0;
  intervalTime[0] = DEFAULT_UPDATE_INTERVAL;
  intervalTime[1] = DEFAULT_REPORT_INTERVAL;
}

void FirmataExtClass::addSelectedFeatures(FirmataFeature *featureArray[])
{

// Install the user-selected FirmataFeatures.

  int selectionIndex = 0;
  while (selectedFeatures[selectionIndex] != 0) {
    if (featureCount < MAX_FEATURE_COUNT) {
      features[featureCount++] = featureArray[selectionIndex++];
    }
  }
}

void FirmataExtClass::dispatchReset()
{

  previousTime[0] = micros();
  previousTime[1] = millis();
  intervalTime[0] = DEFAULT_UPDATE_INTERVAL;
  intervalTime[1] = DEFAULT_REPORT_INTERVAL;

  for (byte i = 0; i < featureCount; i++) {
    features[i]->reset();
  }
}

void FirmataExtClass::dispatchTimers() {
  unsigned long elapsedTime;

  currentTime[0] = micros();
  currentTime[1] = millis();

  for (int idx=0; idx<2; idx++) {
    if (currentTime[idx] >= previousTime[idx]) {
      elapsedTime = currentTime[idx]-previousTime[idx];
    } else {
      elapsedTime = (ULONG_MAX - previousTime[idx])+(currentTime[idx]+1);
    }

    if (elapsedTime >= intervalTime[idx]) {
      if (idx==0) {
        for (int n = 0; n < featureCount; n++) {
          features[n]->update(elapsedTime);
        }
      } else {
        for (int n = 0; n < featureCount; n++) {
          features[n]->report(elapsedTime);
        }
      }
      previousTime[idx] = currentTime[idx];
    }
  }
}

boolean FirmataExtClass::dispatchSetPinMode(byte pin, int mode)
{
  boolean known = false;
  for (byte i = 0; i < featureCount; i++) {
    known |= features[i]->handlePinMode(pin, mode);
  }
  return known;
}

boolean FirmataExtClass::dispatchFeatureSysex(byte command, byte argc, byte* argv)
{
  if (this->handleSysex(command, argc, argv)) {
    return true;
  }

  for (byte i = 0; i < featureCount; i++) {
    if (features[i]->handleSysex(command, argc, argv)) {
      return true;
    }
  }
  return false;
}

boolean FirmataExtClass::handleSysex(byte cmd, byte argc, byte* argv)
{
  switch (cmd) {
  case CAPABILITY_QUERY:
    Firmata.write(START_SYSEX);
    Firmata.write(CAPABILITY_RESPONSE);
    for (byte pin = 0; pin < TOTAL_PINS; pin++) {
      if (Firmata.getPinMode(pin) != IGNORE) {
        for (byte i = 0; i < featureCount; i++) {
          features[i]->handleCapability(pin);
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

  case SAMPLING_INTERVAL:
    if (argc >= 2) {
      intervalTime[1] = max(argv[0] + (argv[1] << 7), MINIMUM_REPORT_INTERVAL);
    }
    break;

  default:
    return false;
  }
  return true;
}

// make one static instance
FirmataExtClass FirmataExt;
