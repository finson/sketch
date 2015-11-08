/*
  MessageIOStream.h
  An Arduino Stream class that wraps an IO stream of some sort to add parsing
  capabilities for Firmata command messages.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
 */

#ifndef MessageIOStream_h
#define MessageIOStream_h

  class MessageIOStream : public Stream
  {
  public:

//  Stream methods

    int available();  // yes
    int read();
    int peek();
    void flush();

    size_t write(uint8_t); //yes

//  Parse message stream

    void processInputStream(void);
    void parse(int value);
    boolean isParsingMessage(void);
    boolean isResetting(void);

  private:
    Stream *ios;
    boolean resetting;

    /* input message handling */

    byte waitForData; // this flag says the next serial input will be data
    byte executeMultiByteCommand; // execute this after getting multi-byte data
    byte multiByteChannel; // channel data for multiByteCommands
    byte storedInputData[MAX_DATA_BYTES]; // multi-byte data

    /* sysex */

    boolean parsingSysex;
    int sysexBytesRead;

  };

#endif
