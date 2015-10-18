/*
  FirmataFeatureCommands.h - Firmata library v2.7.0-beta - 2015-4-15
  Copyright (c) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (c) 2013 Norbert Truchsess. All rights reserved.
  Copyright (c) 2013-2015 Jeff Hoefs. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef Firmata_Feature_Commands_h
#define Firmata_Feature_Commands_h

// pin modes
//#define INPUT                 0x00 // defined in wiring.h
//#define OUTPUT                0x01 // defined in wiring.h
// #define ANALOG                  0x02 // analog pin in analogInput mode
// #define PWM                     0x03 // digital pin in PWM output mode
#define SERVO                   0x04 // digital pin in Servo output mode
#define SHIFT                   0x05 // shiftIn/shiftOut mode
#define I2C                     0x06 // pin included in I2C setup
#define ONEWIRE                 0x07 // pin configured for 1-wire
#define STEPPER                 0x08 // pin configured for stepper motor
#define ENCODER                 0x09 // pin configured for encoders
// #define IGNORE                  0x7F // pin configured to be ignored by digitalWrite and capabilityResponse
// #define TOTAL_PIN_MODES         11

// extended command set using sysex (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for user-defined commands */

// Sysex commands that have an associated pin mode and implement or extend the mode

#define ENCODER_DATA            0x61 // reply with encoders current positions (FirmataFeature ??)
#define SERVO_CONFIG            0x70 // set max angle, minPulse, maxPulse, freq (FirmataFeature ServoFirmata)
#define STEPPER_DATA            0x72 // control a stepper motor (FirmataFeature StepperFirmata)
#define ONEWIRE_DATA            0x73 // send an OneWire read/write/reset/select/skip/search request (FirmataFeature OneWireFirmata)
#define SHIFT_DATA              0x75 // a bitstream to/from a shift register (FirmataFeature ??)
#define I2C_REQUEST             0x76 // send an I2C read/write request (FirmataFeature I2CFirmata)
#define I2C_REPLY               0x77 // a reply to an I2C read request (FirmataFeature I2CFirmata)
#define I2C_CONFIG              0x78 // config I2C settings such as delay times and power pins (FirmataFeature I2CFirmata)

// Sysex commands that do not have an associated pin mode

#define SCHEDULER_DATA          0x7B // send a createtask/deletetask/addtotask/schedule/querytasks/querytask request to the scheduler (FirmataFeature FirmataScheduler)


#endif

