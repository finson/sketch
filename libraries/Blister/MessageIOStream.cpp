/*
  MessageIOStream.cpp
  An Arduino Stream class that wraps an IO stream of some sort to add parsing
  capabilities for Firmata command messages.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
 */

#include <MessageIOStream.h>

MessageIOStream::MessageIOStream(Stream *s) {
 ios = s;
}

//------------------------------------------------------------------------------

int MessageIOStream::available(void)
{
  return ios->available();
}

int MessageIOStream::read()
{
  return ios->read();
}

int MessageIOStream::peek()
{
  return ios->peek();
}

void MessageIOStream::flush()
{
  ios->flush();
}

size_t MessageIOStream::write(uint8_t c)
{
  return ios->write(c);
}

//------------------------------------------------------------------------------

void MessageIOStream::processInputStream(void)
{
  int inputData = this.read(); // this is 'int' to handle -1 when no data
  if (inputData != -1) {
    this.parse(inputData & 0xFF);
  }
}

void MessageIOStream::parse(int inputData)
{
  int command;
  char errorMsg[100] ;

  // TODO make sure it handles -1 properly

  if (parsingSysex) {
    if (inputData == END_SYSEX) {
      parsingSysex = false;
           sprintf(errorMsg, "Sysex command. %02x",storedInputData[0]);
          sendString(errorMsg);
     if (!executeCoreSysex(storedInputData[0], sysexBytesRead - 1, storedInputData + 1)) {
        if (!FirmataExt.dispatchFeatureSysex(storedInputData[0], sysexBytesRead - 1, storedInputData + 1) ) {
          sprintf(errorMsg, "Unrecognized sysex command. %02x",storedInputData[0]);
          sendString(errorMsg);
        }
      }
    } else {
      storedInputData[sysexBytesRead++] = inputData;
    }
  } else if ( (waitForData > 0) && (inputData < 128) ) {
    waitForData--;
    storedInputData[waitForData] = inputData;
    if ( (waitForData == 0) && executeMultiByteCommand ) { // got the whole message
      switch (executeMultiByteCommand) {
        case ANALOG_MESSAGE:
          if (currentAnalogCallback) {
            (*currentAnalogCallback)(multiByteChannel,
                                     (storedInputData[0] << 7)
                                     + storedInputData[1]);
          }
          break;
        case DIGITAL_MESSAGE:
          if (currentDigitalCallback) {
            (*currentDigitalCallback)(multiByteChannel,
                                      (storedInputData[0] << 7)
                                      + storedInputData[1]);
          }
          break;
        case REPORT_ANALOG:
          if (currentReportAnalogCallback)
            (*currentReportAnalogCallback)(multiByteChannel, storedInputData[0]);
          break;
        case REPORT_DIGITAL:
          if (currentReportDigitalCallback)
            (*currentReportDigitalCallback)(multiByteChannel, storedInputData[0]);
          break;
        case SET_PIN_MODE:
          setPinMode(storedInputData[1], storedInputData[0]);
          break;
      }
      executeMultiByteCommand = 0;
    }
  } else {
    // remove channel info from command byte if less than 0xF0
    if (inputData < 0xF0) {
      command = inputData & 0xF0;
      multiByteChannel = inputData & 0x0F;
    } else {
      command = inputData;
    }
    switch (command) {
      case ANALOG_MESSAGE:
      case DIGITAL_MESSAGE:
      case SET_PIN_MODE:
        waitForData = 2; // two data bytes needed
        executeMultiByteCommand = command;
        break;
      case REPORT_ANALOG:
      case REPORT_DIGITAL:
        waitForData = 1; // one data byte needed
        executeMultiByteCommand = command;
        break;
      case START_SYSEX:
        parsingSysex = true;
        sysexBytesRead = 0;
        break;
      case SYSTEM_RESET:
        reset();
        break;
      case REPORT_VERSION:
        printVersion();
        break;
    }
  }
}

boolean MessageIOStream::isParsingMessage(void)
{
  return (waitForData > 0 || parsingSysex);
}

boolean MessageIOStream::isResetting(void)
{
  return resetting;
}

