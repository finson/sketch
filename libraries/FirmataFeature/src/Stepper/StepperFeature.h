/*
  StepperFeature.h - Firmata library
  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2010-2011 Paul Stoffregen.  All rights reserved.
  Copyright (C) 2009 Shigeru Kobayashi.  All rights reserved.
  Copyright (C) 2009-2013 Jeff Hoefs.  All rights reserved.
  Copyright (C) 2013 Norbert Truchsess. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef StepperFeature_h
#define StepperFeature_h

#include <FirmataFeature.h>
#include <StepperMode.h>

#define MAX_STEPPERS 6 // arbitrary value... may need to adjust
#define STEPPER_CONFIG 0
#define STEPPER_STEP 1

class StepperFeature: public FirmataFeature
{
  public:
    StepperFeature();

    boolean handleFeatureSysex(byte command, byte argc, byte *argv);

    void update();
    void report();

    void reset();

    // pin mode is handled by StepperMode

    void handleGetCapability(byte pin) {}
    boolean handleSetPinMode(byte pin, int mode) {return false;}

  private:
    AsyncStepper *stepper[MAX_STEPPERS];
    byte numSteppers;
};

#endif
