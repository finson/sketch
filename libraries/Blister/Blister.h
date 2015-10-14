/*
  Blister.h

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef Blister_h
#define Blister_h

#define BLISTER_MAJOR_VERSION   1 // for non-compatible changes
#define BLISTER_MINOR_VERSION   0 // for backwards compatible changes
#define BLISTER_BUGFIX_VERSION  0 // for bugfix releases

// TODO make it a subclass of a generic Serial/Stream base class
class BlisterClass
{
  public:
    BlisterClass();
    /* Arduino constructors */
    void begin();
    void begin(long);
    void begin(Stream &s);
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
    void attach(byte command, systemResetCallbackFunction newFunction);
    void attach(byte command, stringCallbackFunction newFunction);
    void attach(byte command, sysexCallbackFunction newFunction);
    void detach(byte command);
    /* delegate to Scheduler (if any) */
    void attachDelayTask(delayTaskCallbackFunction newFunction);
    void delayTask(long delay);
    /* access pin config */
    byte getPinMode(byte pin);
    void setPinMode(byte pin, byte config);
    /* access pin state */
    int getPinState(byte pin);
    void setPinState(byte pin, int state);


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
    byte pinConfig[TOTAL_PINS];         // configuration of every pin
    int pinState[TOTAL_PINS];           // any value that has been written

    boolean resetting;

    /* callback functions */
    callbackFunction currentAnalogCallback;
    callbackFunction currentDigitalCallback;
    callbackFunction currentReportAnalogCallback;
    callbackFunction currentReportDigitalCallback;
    callbackFunction currentPinModeCallback;
    systemResetCallbackFunction currentSystemResetCallback;
    stringCallbackFunction currentStringCallback;
    sysexCallbackFunction currentSysexCallback;
    delayTaskCallbackFunction delayTaskCallback;

    /* private methods ------------------------------ */
    void processSysexMessage(void);
    void systemReset(void);
    void strobeBlinkPin(int count, int onInterval, int offInterval);
    void sendValueAsTwo7bitBytes(int value);
    void startSysex(void);
    void endSysex(void);
};

extern FirmataClass Firmata;

/*==============================================================================
 * MACROS
 *============================================================================*/

/* shortcut for setFirmwareNameAndVersion() that uses __FILE__ to set the
 * firmware name.  It needs to be a macro so that __FILE__ is included in the
 * firmware source file rather than the library source file.
 */
#define setFirmwareVersion(x, y)   setFirmwareNameAndVersion(__FILE__, x, y)

#endif
