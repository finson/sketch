##Firmata Feature: Device Driver

Proposed for addition in Firmata 2.6.

##Introduction

The purpose of this Feature is to facilitate arbitrary additions to Firmata capabilities without requiring a central registration or causing frequent command code conflicts.

Two new Sysex sub-commands are added by this Feature: `DEVICE_QUERY` and `DEVICE_RESPONSE`.

There is a small set of action codes that specify what the driver is to do when it decodes the message.  

The first action is always `OPEN`.  The caller supplies a device name that can be recognized by the device driver, and when the device is opened, a handle is returned to the caller for use in future calls.

The following actions can include **...more later**

The final action is `CLOSE`.

Character strings are encoded in UTF-8.  All eight bits in a UTF-8 byte are
significant.  A 0 in the high order bit indicates a character in the first group of 127 characters (the ASCII character set).  A 1 in the high order bit
indicates that the byte is part of a multi-byte sequence. Unfortunately, it
might also indicate a Firmata control byte.  Consequently, all characters
strings (which are all UTF-8 in the Arduino) are all transmitted using two bytes
per source byte.

In the following message tables, the message contents are all shown one byte per row.

##General Device Driver Message Formats


###Device Driver Request Message

From Firmata client to Firmata on the micro-host.

	0  START_SYSEX (0xF0)  
	1  DEVICE_QUERY (0x30)  
	2  action (LSB)  
	3  action (MSB)  
	4  data 0  
	5  data 1  
	6  data 2  
	7  data 3  
	...  
	n  END_SYSEX (0XF7)  

###Device Driver Reply Message

From Firmata on the micro-host to Firmata client.


	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  action (LSB)
	3  action (MSB)
	4  data 0
	5  data 1
	6  data 2
	7  data 3
	...
	n  END_SYSEX (0XF7)


###Action Types

	OPEN    (0x0000)
	STATUS  (0x0001)
	CONTROL (0x0002)
	READ    (0x0003)
	WRITE   (0x0004)
	CLOSE   (0x0005)


##Detailed Device Driver Message Formats

###Device Driver Request - Open
`uint16_t open(char* name)`

param `name` Name of the device to open.  UTF-8 encoded, does not include a null byte terminator.  As described above, Firmata uses two bytes to transmit each 
byte of the original UTF-8 string.

returns `handle` A 14-bit value that is used to indicate the device driver being
addressed in future calls.  Returns handle of 0x3FFF if none of the device drivers claimed the named device.

####Request

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x00 (OPEN Lo7)
	3  0x00 (OPEN Hi7)
	4  string byte 0 (Lo7)
	5  string byte 0 (Hi1) 
	6  string byte 1 (Lo7)
	7  string byte 1 (Hi1)
	...
	n  END_SYSEX (0XF7)


####Reply

	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x00 (OPEN Lo7)
	3  0x00 (OPEN Hi7)
	4  handle (Lo7)
	5  handle (Hi7) 
	6  END_SYSEX (0XF7)

---
###Device Driver Request - Close
`uint16_t close(uint16_t handle)`

param `handle` The device driver selector value returned by Open in a previous
call.  The selected device driver is responsible for deciding what actions if any are needed to "close" the connection.  After a close, the only valid action on the device is another open.

returns `handl`e The handle that was just closed, or 0x3FFF if the device was 
not open or the handle is invalid.

####Request

	0  START_SYSEX (0xF0)
	1  DEVICE_QUERY (0x30)
	2  0x05 (CLOSE Lo7)
	3  0x00 (CLOSE Hi7)
	4  handle (Lo7)
	5  handle (Hi7) 
	6  END_SYSEX (0XF7)

####Reply


	0  START_SYSEX (0xF0)
	1  DEVICE_RESPONSE (0x31)
	2  0x05 (CLOSE Lo7)
	3  0x00 (CLOSE Hi7)
	4  handle (Lo7)
	5  handle (Hi7) 
	6  END_SYSEX (0XF7)

---







