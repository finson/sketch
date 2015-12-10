##Hello Device Driver

Implemented as part of the Device Driver Feature proposed for Firmata 2.6

##Introduction

This Hello Device Driver provides a simple example of controlling functions  on a remote Firmata server from a client-side application.

###Device Driver API

Note that the general Device Driver API is intended to be implemented on both the client side (application host) and on the server side (Firmata micro).  Client-side applications and server-side device drivers always use this API and never compose Firmata messages themselves, instead they rely on Firmata to do that.

If the implementer of a device driver has chosen to implement the main control code on the client, then the client device driver uses  Firmata Features and commands as necessary to control the remote component(s) directly and according to the data sheet.  In this scenario, the server side Firmata responds to standard Firmata commands as received.  

If the implementer has chosen to implement the main control code on the server, then the client device driver acts as a proxy for the actual device driver and uses the Device Driver Sysex messages DEVICE\_QUERY and DEVICE\_RESPONSE to control the server side device driver, which in turn controls the component(s) using local capabilities.  In this scenario, the server side device driver receives the same calls and parameters as were provided to the proxy on the client.

###Method Prototypes

The method prototypes shown for each message are expected to be the primary interface for all Device Drivers.  The type identifier `int` is used to indicate a signed integer value of at least 16 bits.  Only the low order 16 bits (the two low order bytes) are transmitted for these values by Firmata.

See device-driver.md for more discussion of how the queries and responses are encoded by Firmata and where the parameter values are placed in each message type.

##Hello Device Driver Methods

###Hello - Open

By default, this device driver recognizes names beginning with "Hello", but other prefixes can be specified when the device driver is initialized.

    int open(char* name)
    int open(char* name, int flags)

**param** `name` Name of the device to open.  UTF-8 encoded.  
**param** `flags` Flags associated with the open, if any.

**return** The value that is used in future calls to indicate the device driver and specific device being addressed.  On error, returns -1.

---
###Hello - Status
	int status(int handle, int reg, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `reg` The status register address at which to start reading.  This value does not necessarily name a real register, but is used by the device driver to identify the desired status information.  
**param** `count` The number of bytes to read.  
**param** `buf` Pointer to the buffer to receive the data read.  Must be large enough to hold `count` bytes.  

**return** The number of bytes just read, or -1 if there was an error (device was 
not open, the handle is invalid, etc.)

---
###Hello - Control
	int control(int handle, int reg, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `reg` The control register address at which to start writing.   This value does not necessarily name a real register, but is used by the device driver to identify the desired control function.  
**param** `count` The number of bytes to write.  
**param** `buf` Pointer to the buffer containing the data to write.  

**return** The number of bytes actually written, or -1 if there was an error (device was not open, the handle is invalid, etc.)


---
###Hello - Read
	int read(int handle, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `count` The number of bytes to read.  
**param** `buf` Pointer to the buffer to receive the data read.  Must be large enough to hold `count` bytes.  

**return** The number of bytes just read, or -1 if there was an error (device was 
not open, the handle is invalid, etc.)



---
###Hello - Write
	int write(int handle, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous
call.  
**param** `count` The number of bytes to write.  
**param** `buf` Pointer to the buffer containing the data to write.  Must contain at least `count` bytes.  

**return** The number of bytes actually written, or -1 if there was an error (device was not open, the handle is invalid, etc.)


---
###Hello - Close
	int close(int handle)

**param** `handle` The device driver selector value returned by Open in a previous
call.  The selected device driver is responsible for deciding what actions if any are needed to "close" the connection.  After a close, the only valid action on the device is another open.

**return** On success, returns zero. On error, -1 is returned.

##Hello Library Methods

###Library - readTempC
    double readTempC(int handle)

**param** `handle` The device driver selector value returned by Open in a previous
call.  

**return** Reads the 16-bit ambient temperature register and returns the Centigrade temperature as a floating point value.

**throws** Throws a `DeviceException` in the event of an error while reading the device.

---
