/*
  MessageSource.h
  An abstract Arduino Stream whose subclasses wrap an actual Stream
  instance to provide parsing capabilities for Firmata command messages.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
 */

#ifndef MessageSource_h
#define MessageSource_h

#include <FirmataCore.h>

class MessageSource : public Stream
{
  public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;

     virtual size_t write(uint8_t) = 0;
};

#endif
