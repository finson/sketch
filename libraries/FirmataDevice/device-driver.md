##Firmata Feature: Device Driver

Proposed for addition in Firmata 2.6.

The purpose of this Feature is to facilitate arbitrary additions to Firmata capabilities without requiring a central registration or causing frequent command code conflicts.  The Feature is implemented with a pair of new Sysex commands and the concept of a DeviceDriver abstract class with well defined method signatures.  In effect, this Feature uses Firmata as a remote procedure call mechanism.

##Introduction

Device Drivers are designed to allow device control by a client-side application running remotely from a Firmata server to which the devices are connected.

###Device Driver API

Note that the general Device Driver API is intended to be implemented on both the client side (application host) and on the server side (Firmata micro).  Client-side applications and server-side device drivers always use this API and never compose Firmata messages themselves, instead they rely on Firmata to do that.

If the implementer of a device driver has chosen to implement the main control code on the server, then the client device driver acts as a proxy for the actual device driver and uses the Device Driver Sysex messages DEVICE\_QUERY and DEVICE\_RESPONSE to control the server side device driver, which in turn controls the component(s) using local capabilities.  In this scenario, the server side device driver receives the same calls and parameters as were provided to the proxy on the client.

If the implementer has chosen to implement the main control code on the client, then the client device driver uses existing Firmata Features and commands as necessary to control the remote component(s) directly and according to the data sheet.  In this scenario, the server side Firmata responds to standard Firmata commands as received.  

###Terminology

Some terms with specific meanings for this topic are *device*, *logical unit* and *handle*.

- Device.  A device driver can control one or more instances of a specific *device* type.  The capabilities of the driver are determined by the device type, and are equally available to all the instances of the device. Each driver is given a simple name to identify the device type.  For example, MCP9808 is the name of a device driver for the MCP9808 temperature sensor.  Also, Hello is the name of a virtual device driver that deals with "Hello World" type messaging.
- Logical Unit.  Each instance of a device is a *logical unit*.  A device driver can handle one or more logical units, depending on device capabilities and device driver implementation. Each logical unit is given a name based on the device name.  For example, if there are two MCP9808 boards connected to a single server, then they are referred to as MCP9808:0 and MCP9808:1.
- Handle.  When a logical unit is first opened, a *handle* that uniquely identifies the device and logical unit of interest is returned to the caller for use in future operations.  The handle is valid until the logical unit is closed.

###Firmata Messages

Two new Sysex sub-commands are added by this Feature: `DEVICE_QUERY` and `DEVICE_RESPONSE`.

There is a small set of action codes that specify what the driver is to do after it receives the message.  

The first action is always `OPEN`.  The caller supplies a logical unit name that can be recognized by a device driver, and upon success, a handle is returned for use in future calls. After the handle has been received, the caller can read status (`STATUS`), write control (`CONTROL`), read data stream (`READ`), and write data stream (`WRITE`).  Once the caller has completed its operations on a device, it can use `CLOSE' to make the logical unit available for another client.

###Method Prototypes

The method prototypes shown for each message are expected to be the primary interface to each Device Driver, on both the server and the client.  The type identifier `int` is used to indicate a signed integer value of at least 16 bits.  Only the low order 16 bits (the two low order bytes) are transmitted for these values by Firmata.


###Encoding

####Header

The DEVICE\_QUERY and DEVICE\_RESPONSE message headers are Sysex bytes 0 to 5.

DEVICE\_QUERY

	0 is always the START_SYSEX byte (0xF0).
	1 is the Sysex command byte DEVICE_QUERY.
	2 is the Device Action byte, with values as described below.  The highest order bit must be 0.
	3 is Reserved for future use and is set to 0.
	4 is the least significant byte of the 2-byte flags or handle value.  The highest order bit must be 0.
	5 is the most significant byte of the 2-byte flags or handle value.  The highest order bit must be 0.

DEVICE\_RESPONSE

	0 is always the START_SYSEX byte (0xF0).
	1 is the Sysex command byte DEVICE_RESPONSE.
	2 is the Device Action byte that was provided in the associated DEVICE_QUERY.  The highest order bit must be 0.
	3 is the status return value: 0 for success, or a non-zero error code for failure.
	4 is the least significant byte of the 2-byte handle value.  The highest order bit must be 0.
	5 is the most significant byte of the 2-byte handle value.  The highest order bit must be 0.

#####Device Action Types

These are 7-bit values, stored in Firmata DEVICE\_QUERY and DEVICE\_RESPONSE messages at offset 2.

	OPEN    (0x00)
	STATUS  (0x01)
	CONTROL (0x02)
	READ    (0x03)
	WRITE   (0x04)
	CLOSE   (0x05)

#####Flags or Handle

These are 7-bit values, stored in the Firmata DEVICE\_QUERY and DEVICE\_RESPONSE messages at offsets 4 and 5.  For convenience the two bytes can be stored together on the client in a single, wider integer variable (`int16_t`, `int32_t`, etc) but is important to remember that any operations on the value must preserve bits 7 and 15 set to 0 so that when the bytes are separated again they are still transmittable without further encoding.

	3 flags (LSB, bit 7 = 0)
	4 flags (MSB, bit 15 = 0)

or

	3 handle (minor) (LSB, bit 7 = 0)
	4 handle (major) (MSB, bit 15 = 0)


####Body
The body of the message (all bytes after offset 5 except the final END\_SYSEX) is encoded before transmission using an 8-bit to 7-bit encoder.  The standard encoder is Base-64.  This encode/decode is handled entirely by the Firmata libraries right before and after transmission of the Sysex messages and should not ordinarily be visible to the client application.

Character strings are stored on the server in UTF-8.  All eight bits in a UTF-8 byte are significant.  A '0' in the high order bit indicates a character in the first group of 127 characters (the ASCII character set).  A '1' in the high order bit indicates that the byte is part of a multi-byte sequence. Unfortunately, it might also indicate a Firmata control byte.  Encoding in Base-64 avoids this problem.

In the following message tables, the message contents are all shown one byte per row.  Remember that all bytes starting at offset 6 are encoded prior to transmission. The values shown in the tables below starting at offset 6 are **the 8-bit values before or after encoding / decoding**, they are **_not_** the 7-bit quantities that are actually transmitted.

##Detailed Device Driver Message Formats

###Device Driver Query - Open
	int open(char* name)
	int open(char* name, int flags)

**param** `name` Name of the logical unit to open.  UTF-8 encoded.  

**param** `flags` Flags associated with the open.  

**return** On success, the handle value that is used in future calls to indicate the device driver and specific device being addressed.

####Query
**Message Header** (Plain text)

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x00 (OPEN)
	3  0 (Reserved)
	4  flags (LSB)
	5  flags (MSB)

**Parameter Block** (Before encoding)

> 		0 name byte 0
> 		1 name byte 1
 		...

**Message End** (Plain text)

 	n  END_SYSEX (0XF7)


# **--------v---v--v---------Restart editing here** #
####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x00 (OPEN)
	3  success/error
	4  handle (LSB)
	5  handle (MSB)
	6  END_SYSEX (0XF7)

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
	5  register (LSB)
	6  register (MSB)
	7  count (LSB)
	8  count (MSB) 
	9  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x01 (STATUS)
	3  handle (LSB)
	4  handle (MSB)
	5  success/error code
	6  status byte 0
	7  status byte 1
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
	5  register (LSB)
	6  register (MSB)
	...
	n  END_SYSEX (0XF7)

####Response

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x02 (CONTROL)
	3  count/error (LSB)
	4  count/error (MSB) 
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
	3  count/error (LSB)
	4  count/error (MSB) 
	5  input byte 0
	6  input byte 1
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







