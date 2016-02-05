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

#ifndef FirmataExt_h
#define FirmataExt_h

#include <FirmataCore.h>
#include <FirmataFeature.h>

#define MAX_FEATURE_COUNT 20

#define MINIMUM_REPORT_INTERVAL 10     // milliseconds
#define DEFAULT_REPORT_INTERVAL 19

#define MINIMUM_UPDATE_INTERVAL 100    // microseconds
#define DEFAULT_UPDATE_INTERVAL 200

class FirmataExtClass
{
  public:
    FirmataExtClass();
    void addSelectedFeatures(FirmataFeature *featureArray[]);

    void dispatchReset();
    void dispatchTimers();
    boolean dispatchSetPinMode(byte pin, int mode);
    boolean dispatchFeatureSysex(byte command, byte argc, byte* argv);

  private:
    boolean handleSysex(byte cmd, byte argc, byte* argv);

    FirmataFeature *features[MAX_FEATURE_COUNT];
    int featureCount;

    unsigned long previousTime[2];   // the time the last interval expired
    unsigned long currentTime[2];    // the current values from millis() and micros()
    unsigned long intervalTime[2];   // length of time between calls to features
};

extern FirmataExtClass FirmataExt;

#endif
