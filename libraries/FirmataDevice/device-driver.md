##Firmata Feature: Device Driver

Proposed for addition in Firmata 2.6.

The purpose of this Feature is to facilitate arbitrary additions to Firmata capabilities without requiring a central registration or causing frequent command code conflicts.  The Feature is implemented with a pair of new Sysex commands and the concept of a DeviceDriver abstract class with well defined method signatures.  In effect, this Feature uses Firmata as a remote procedure call mechanism.

##Introduction

Device Drivers are designed to control devices from a client-side application running remotely from a Firmata server to which the devices are connected.
###Device Driver API

Note that the general Device Driver API is intended to be implemented on both the client side (application host) and on the server side (Firmata micro).  Client-side applications and server-side device drivers always use this API and never compose Firmata messages themselves, instead they rely on Firmata to do that.

If the implementer of a device driver has chosen to implement the main control code on the client, then the client device driver uses  Firmata Features and commands as necessary to control the remote component(s) directly and according to the data sheet.  In this scenario, the server side Firmata responds to standard Firmata commands as received.  

If the implementer has chosen to implement the main control code on the server, then the client device driver acts as a proxy for the actual device driver and uses the Device Driver Sysex messages DEVICE\_QUERY and DEVICE\_RESPONSE to control the server side device driver, which in turn controls the component(s) using local capabilities.  In this scenario, the server side device driver receives the same calls and parameters as were provided to the proxy on the client.

###Firmata Messages

Two new Sysex sub-commands are added by this Feature: `DEVICE_QUERY` and `DEVICE_RESPONSE`.

There is a small set of action codes that specify what the driver is to do when it decodes the message.  

The first action is always `OPEN`.  The caller supplies a device name that can be recognized by the device driver, and when the device is opened, a handle is returned to the caller for use in future calls.

The following actions can include **...more later**

The final action is `CLOSE`.

###Method Prototypes

The method prototypes shown for each message are expected to be the primary interface to each Device Driver, on both the server and the client.  The type identifier `int` is used to indicate a signed integer value of at least 16 bits.  Only the low order 16 bits (the two low order bytes) are transmitted for these values by Firmata.


###Encoding

####Header

The data bytes in DEVICE\_QUERY and DEVICE\_RESPONSE Sysex message headers (bytes 0 to 4) are not encoded.

	0 is always the START_SYSEX byte (0xF0).
	1 is the Sysex command byte, either DEVICE_QUERY or DEVICE_RESPONSE.
	2 is the Device Action byte, with values as described below.
	3 is the least significant byte of the 2-byte flags or handle value.  The highest order bit must have a 0 value.
	4 is the most significant byte of the 2-byte flags or handle value.  The highest order bit must have a 0 value.

Note that this scheme requires that the high order bit in the two flag bytes and the high order bit in the two handle bytes must always be 0.  

#####Action Types

These are 7-bit values.  Stored in the Firmata messages at offset 2, the action type value is **_not_** included in the bulk encoding of  the message body.

	OPEN    (0x00)
	STATUS  (0x01)
	CONTROL (0x02)
	READ    (0x03)
	WRITE   (0x04)
	CLOSE   (0x05)

#####Flags or Handle

These are 7-bit values.  Stored in the Firmata messages at offsets 3 and 4, they are **_not_** included in the bulk encoding of  the message body.  For convenience the two bytes are stored together on the client in a single, wider integer variable (`int16_t`, `int32_t`, etc) but is important to remember that any operations on the value must preserve bits 7 and 15 set to 0 so that when the bytes are separated again they are still transmittable without further encoding.

	3 flags (LSB, bit 7 = 0)
	4 flags (MSB, bit 15 = 0)

or

	3 handle (minor ID) (LSB, bit 7 = 0)
	4 handle (major ID) (MSB, bit 15 = 0)


####Body
The body of the message (all bytes after offset 4 except the final END\_SYSEX) are transmitted using an 8-bit to 7-bit encoder.  The standard encoder is Base-64.  This encode/decode is handled entirely by the Firmata libraries right before and after transmission of the Sysex messages and should not ordinarily be visible to the client application.

Character strings are stored on the server in UTF-8.  All eight bits in a UTF-8 byte are significant.

A '0' in the high order bit indicates a character in the first group of 127 characters (the ASCII character set).  A '1' in the high order bit indicates that the byte is part of a multi-byte sequence. Unfortunately, it might also indicate a Firmata control byte.  Encoding in Base-64 avoids this problem.

In the following message tables, the message contents are all shown one byte per row.  Remember that all bytes starting at offset 5 are encoded for transmission. The values shown in the tables below starting at offset 5 are **the 8-bit values before or after encoding / decoding**, they are **_not_** the 7-bit quantities that are actually transmitted.

##General Device Driver Message Formats

###Device Driver Query Message

From Firmata client to Firmata on the micro (server).

	0  START_SYSEX (0xF0)  
	1  DEVICE_QUERY (0x30)  
	2  action  
	3  flag/handle (LSB)  
	4  flag/handle (MSB)  
	5  body 0  
	6  body 1  
	...  
	n  END_SYSEX (0XF7)  

###Device Driver Response Message

From Firmata on the micro (server) to Firmata client.


	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  action
	3  handle (LSB)
	4  handle (MSB)
	5  body 0
	6  body 1
	...
	n  END_SYSEX (0XF7)



##Detailed Device Driver Message Formats

###Device Driver Query - Open
	int open(char* name)
	int open(char* name, int flags)

**param** `name` Name of the device to open.  UTF-8 encoded, does not include a null byte terminator.  
**param** `flags` Flags associated with the open.  Currently, no flag bits are defined but the parameter is transmitted anyway.  

**return** On success, the value that is used in future calls to indicate the device driver and specific device being addressed.  On error, throws an exception or returns -1.  Note that Firmata actually sends 0x7F7F if the remote implementation supplies -1 as the return value, and so the client library `open()` method must note the distinguished value and return -1 to its caller instead.

####Query

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x00 (OPEN)
	3  flags (LSB)
	4  flags (MSB)
	5  string byte 0 
	6  string byte 1
	...
	n  END_SYSEX (0XF7)


####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x00 (OPEN)
	3  handle (LSB)
	4  handle (MSB) 
	5  END_SYSEX (0XF7)

---
###Device Driver Query - Status
	int status(int handle, int reg, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `reg` The register address at which to start reading.  
**param** `count` The number of bytes to read.  
**param** `buf` Pointer to the buffer to receive the data read.  Must be large enough to hold `count` bytes.  

**return** The number of bytes just read, or -1 if there was an error (device was not open, the handle is invalid, etc.)

####Query

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x01 (STATUS)
	3  handle (LSB)
	4  handle (MSB)
	5  count (LSB)
	6  count (MSB) 
	7  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x01 (STATUS)
	3  handle (LSB)
	4  handle (MSB) 
	5  count (LSB)
	6  count (MSB) 
	7  status byte 0
	8  status byte 1
	...
	n  END_SYSEX (0XF7)

---
###Device Driver Query - Control
	int control(int handle, int reg, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `reg` The register address at which to start writing.  
**param** `count` The number of bytes to write.  
**param** `buf` Pointer to the buffer containing the data to write.  

**return** The number of bytes actually written, or -1 if there was an error (device was not open, the handle is invalid, etc.)

####Query

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x02 (CONTROL)
	3  handle (LSB)
	4  handle (MSB) 
	5  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x02 (CONTROL)
	3  count (LSB)
	4  count (MSB) 
	5  END_SYSEX (0XF7)

---
###Device Driver Query - Read
	int read(int handle, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous
call.  
**param** `count` The number of bytes to read.  
**param** `buf` Pointer to the buffer to receive the data read.  Must be large enough to hold `count` bytes.  

**return** The number of bytes just read, or -1 if there was an error (device was 
not open, the handle is invalid, etc.)

####Query

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x03 (READ)
	3  handle (LSB)
	4  handle (MSB)
	5  count (LSB)
	6  count (MSB) 
	7  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x03 (READ)
	3  handle (LSB)
	4  handle (MSB) 
	5  count (LSB)
	6  count (MSB) 
	7  input byte 0
	8  input byte 1
	...
	n  END_SYSEX (0XF7)




---
###Device Driver Query - Write
	int write(int handle, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous
call.  
**param** `count` The number of bytes to write.  
**param** `buf` Pointer to the buffer containing the data to write.  Must contain at least `count` bytes.  

**return** The number of bytes actually written, or -1 if there was an error (device was not open, the handle is invalid, etc.)

####Query

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x04 (WRITE)
	3  handle (LSB)
	4  handle (MSB) 
	5  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x04 (WRITE)
	3  count (LSB)
	4  count (MSB) 
	5  END_SYSEX (0XF7)

---
###Device Driver Query - Close
	int close(int handle)

**param** `handle` The device driver selector value returned by Open in a previous
call.  The selected device driver is responsible for deciding what actions if any are needed to "close" the connection.  After a close, the only valid action on the device is another open.

**return** On success, returns 0. On error, throws an exception or returns -1.  Note that Firmata actually sends 0x7F7F if the remote implementation supplies -1 as the status value, and so the client library `close()` method must note the distinguished value and return -1 to its caller instead.

####Query

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  CLOSE (0x05)
	3  handle (LSB)
	4  handle (MSB) 
	5  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  CLOSE (0x05)
	3  status (LSB)
	4  status (MSB) 
	5  END_SYSEX (0XF7)

---







