/*
  FirmataCore.h - Firmata library v2.7.0-beta - 2015-4-15
  Copyright (c) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (c) 2013 Norbert Truchsess. All rights reserved.
  Copyright (c) 2013-2015 Jeff Hoefs. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef Firmata_Core_h
#define Firmata_Core_h

#include <hal/Boards.h>  /* Hardware Abstraction Layer + Wiring/Arduino */

/* Version numbers for the protocol.  The protocol is still changing, so these
 * version numbers are important.  This number can be queried so that host
 * software can test whether it will be compatible with the currently
 * installed firmware. */

#define FIRMATA_MAJOR_VERSION   2 // for non-compatible changes
#define FIRMATA_MINOR_VERSION   5 // for backwards compatible changes
#define FIRMATA_BUGFIX_VERSION  0 // for bugfix releases

#define MAX_DATA_BYTES          64 // max number of data bytes in incoming messages
#define MINIMUM_SAMPLING_INTERVAL 10  // milliseconds
#define DEFAULT_SAMPLING_INTERVAL 19

// Core Direct command set (128-255/0x80-0xFF)

#define REPORT_VERSION          0xF9 // report protocol version (FirmataCore)
#define SYSTEM_RESET            0xFF // reset from MIDI (FirmataCore)

#define START_SYSEX             0xF0 // start a MIDI Sysex message (FirmataCore)
#define END_SYSEX               0xF7 // end a MIDI Sysex message (FirmataCore)

#define DIGITAL_MESSAGE         0x90 // send data for a digital pin (DigitalOutputFirmata)
#define ANALOG_MESSAGE          0xE0 // send data for an analog pin (or PWM) (AnalogOutputFirmata)
#define REPORT_ANALOG           0xC0 // enable analog input by pin # (AnalogInputFirmata)
#define REPORT_DIGITAL          0xD0 // enable digital input by port pair (DigitalInputFirmata)

#define SET_PIN_MODE            0xF4 // set a pin to INPUT/OUTPUT/PWM/etc (FirmataCore, then FirmataExt)

// Core Sysex command set with sub-commands: START_SYSEX, followed by (0-127/0x00-0x7F), ..., END_SYSEX
/* 0x00-0x0F reserved for user-defined commands */

// Sysex commands that do not have an associated pin mode

#define REPORT_FIRMWARE         0x79 // report name and version of the firmware (FirmataCore)
#define SAMPLING_INTERVAL       0x7A // set the poll rate of the main loop (FirmataCore)
#define STRING_DATA             0x71 // a string message with 14-bits per char (FirmataCore)

#define ANALOG_MAPPING_QUERY    0x69 // ask for mapping of analog to pin numbers (FirmataCore)
#define ANALOG_MAPPING_RESPONSE 0x6A // reply with mapping info (FirmataCore)
#define PIN_STATE_QUERY         0x6D // ask for a pin's current mode and value (FirmataCore)
#define PIN_STATE_RESPONSE      0x6E // reply with pin's current mode and value (FirmataCore)
#define CAPABILITY_QUERY        0x6B // ask for supported modes and resolution of all pins (FirmataExt)
#define CAPABILITY_RESPONSE     0x6C // reply with supported modes and resolution (FirmataExt)

// Sysex commands that have an associated pin mode and implement or extend the mode

#define EXTENDED_ANALOG         0x6F // analog write (PWM, Servo, etc) to any pin (AnalogOutputFeature)

// Other Sysex sub-commands

#define SYSEX_NON_REALTIME      0x7E // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME          0x7F // MIDI Reserved for realtime messages

#define TOTAL_SYSEX_COMMANDS    22   // MAX_FEATURES in FirmataExt is based on this (inflated) number

// pin modes
//#define INPUT                 0x00 // defined in wiring.h
//#define OUTPUT                0x01 // defined in wiring.h
#define ANALOG                  0x02 // analog pin in analogInput mode
#define PWM                     0x03 // digital pin in PWM output mode
// #define SERVO                   0x04 // digital pin in Servo output mode
// #define SHIFT                   0x05 // shiftIn/shiftOut mode
// #define I2C                     0x06 // pin included in I2C setup
// #define ONEWIRE                 0x07 // pin configured for 1-wire
// #define STEPPER                 0x08 // pin configured for stepper motor
// #define ENCODER                 0x09 // pin configured for encoders
#define IGNORE                  0x7F // pin configured to be ignored by digitalWrite and capabilityResponse
#define TOTAL_PIN_MODES         11

extern "C" {
  // callback function types
  typedef void (*callbackFunction)(byte, int);
  typedef void (*stringCallbackFunction)(char *);
  typedef void (*sysexCallbackFunction)(byte command, byte argc, byte *argv);
//  typedef void (*systemResetCallbackFunction)(void);
//  typedef void (*delayTaskCallbackFunction)(long delay);
}

// TODO make it a subclass of a generic Serial/Stream base class
class FirmataClass
{
  public:
    FirmataClass();
    void begin();
    void begin(long);
    void begin(Stream &s);
    void reset(void);

    /* querying functions */
    void printVersion(void);
    void blinkVersion(void);
    void printFirmwareVersion(void);
    //void setFirmwareVersion(byte major, byte minor);  // see macro below
    void setFirmwareNameAndVersion(const char *name, byte major, byte minor);
    /* serial receive handling */
    int available(void);
    void processInput(void);
    void parse(unsigned char value);
    boolean isParsingMessage(void);
    boolean isResetting(void);
    /* serial send handling */
    void sendAnalog(byte pin, int value);
    void sendDigital(byte pin, int value); // TODO implement this
    void sendDigitalPort(byte portNumber, int portData);
    void sendString(const char *string);
    void sendString(byte command, const char *string);
    void sendSysex(byte command, byte bytec, byte *bytev);
    void write(byte c);

    /* attach & detach callback functions to messages */
    void attach(byte command, callbackFunction newFunction);
//    void attach(byte command, systemResetCallbackFunction newFunction);
    void attach(byte command, stringCallbackFunction newFunction);
    void attach(sysexCallbackFunction newFunction);
    void detach(byte command);

    /* access pin config */
    byte getPinMode(byte pin);
    void setPinMode(byte pin, byte config);

    /* access pin state */
    int getPinState(byte pin);
    void setPinState(byte pin, int state);

    /* Sampling interval */
    void setSamplingInterval(int interval);
    boolean elapsed();

  private:
    Stream *FirmataStream;
    /* firmware name and version */
    byte firmwareVersionCount;
    byte *firmwareVersionVector;
    /* input message handling */
    byte waitForData; // this flag says the next serial input will be data
    byte executeMultiByteCommand; // execute this after getting multi-byte data
    byte multiByteChannel; // channel data for multiByteCommands
    byte storedInputData[MAX_DATA_BYTES]; // multi-byte data
    /* sysex */
    boolean parsingSysex;
    int sysexBytesRead;
    /* pins configuration */
    byte pinConfig[TOTAL_PINS];     // configuration of every pin
    int pinState[TOTAL_PINS];       // any value that has been written

    /* timer variables */
    unsigned long currentMillis;   // store the current value from millis()
    unsigned long previousMillis;  // for comparison with currentMillis
    int samplingInterval;          // how often to run the main loop (in ms)

    boolean resetting;

    /* callback functions */
    callbackFunction currentAnalogCallback;         // AnalogOutputFeature
    callbackFunction currentDigitalCallback;        // DigitalOutputFeature
    callbackFunction currentReportAnalogCallback;   // AnalogInputFeature
    callbackFunction currentReportDigitalCallback;  // DigitalInputFeature

    // callbackFunction currentPinModeCallback;        // -unused- (was FirmataExt)
    stringCallbackFunction currentStringCallback;   // -unused-
    // sysexCallbackFunction currentSysexCallback;     // -unused- (was FirmataExt)

//   systemResetCallbackFunction currentSystemResetCallback;
//   delayTaskCallbackFunction delayTaskCallback;

    /* private methods ------------------------------ */
    boolean executeCoreSysex(void);
    void strobeBlinkPin(int count, int onInterval, int offInterval);
    void sendValueAsTwo7bitBytes(int value);
    void startSysex(void);
    void endSysex(void);
};

extern FirmataClass Firmata;
#endif

