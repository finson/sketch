##Firmata Feature: Device Driver for MCP9808 Temperature Sensor

Implemented as part of the Device Driver Feature proposed for Firmata 2.6

##Introduction

This Device Driver is designed to provide a means of controlling and reading the MCP9808 temperature sensor from a client-side application running remotely from a Firmata server to which a sensor is connected.

Note especially that the API defined here is to be implemented on both the client side (application host) and on the server side (Firmata micro).  The client-side application always uses this API and never composes the Firmata messages itself, instead it relies on the client side device driver to do that.

If the implementer of the device driver has chosen to implement the main control code on the client, then the client device driver uses the Firmata I2C Feature and its associated Sysex commands to control the sensor directly and according to the data sheet.  In this scenario, the server side Firmata responds to the I2C commands as received.  

If the implementer has chosen to implement the main control code on the server, then the client device driver acts as a proxy for the actual device driver and uses the Device Driver Sysex messages DEVICE\_QUERY and DEVICE\_RESPONSE to control the server side device driver, which in turn controls the sensor using local I2C capabilities.  In this scenario, the server side Firmata receives the same calls and parameters as were provided to the proxy on the client.

###Method Prototypes

The method prototypes shown for each message are expected to be the primary interface to each Device Driver, on both the server and the client.  The type identifier `int` is used to indicate a signed integer value of at least 16 bits.  Only the low order 16 bits (the two low order bytes) are transmitted for these values by Firmata.

See also device-driver.md for more discussion of how the queries and responses are encoded by Firmata and where the parameter values are placed in each message type.

##MCP9808 Device Driver Methods

###MCP9808 - Open

By default, this device driver recognizes names beginning with "MCP9808", but other prefixes can be specified when the MCP9808 device driver is initialized.

    int open(char* name)
    int open(char* name, int flags)

**param** `name` Name of the device to open.  UTF-8 encoded, does not include a null byte terminator.  
**param** `flags` Flags associated with the open.  Currently, no flag bits are defined and the parameter is ignored.

**return** The value that is used in future calls to indicate the device driver and specific device being addressed.  Returns -1 if none of the device drivers claimed the named device for whatever reason.

---
###MCP9808 - Status
	int status(int handle, int reg, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `reg` The register address at which to start reading.  
**param** `count` The number of bytes to read.  
**param** `buf` Pointer to the buffer to receive the data read.  Must be large enough to hold `count` bytes.  

**return** The number of bytes just read, or -1 if there was an error (device was 
not open, the handle is invalid, etc.)

---
###MCP9808 - Control
	int control(int handle, int reg, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `reg` The register address at which to start writing.  
**param** `count` The number of bytes to write.  
**param** `buf` Pointer to the buffer containing the data to write.  

**return** The number of bytes actually written, or -1 if there was an error (device was not open, the handle is invalid, etc.)


---
###MCP9808 - Read
	int read(int handle, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous call.  
**param** `count` The number of bytes to read.  
**param** `buf` Pointer to the buffer to receive the data read.  Must be large enough to hold `count` bytes.  

**return** The number of bytes just read, or -1 if there was an error (device was 
not open, the handle is invalid, etc.)



---
###MCP9808 - Write
	int write(int handle, int count, void *buf)

**param** `handle` The device driver selector value returned by Open in a previous
call.  
**param** `count` The number of bytes to write.  
**param** `buf` Pointer to the buffer containing the data to write.  Must contain at least `count` bytes.  

**return** The number of bytes actually written, or -1 if there was an error (device was not open, the handle is invalid, etc.)


---
###MCP9808 - Close
	int close(int handle)

**param** `handle` The device driver selector value returned by Open in a previous
call.  The selected device driver is responsible for deciding what actions if any are needed to "close" the connection.  After a close, the only valid action on the device is another open.

**return** On success, returns zero. On error, -1 is returned.

##MCP9808 Device Library Methods

###Library - readTempC
    double readTempC(int handle)

**param** `handle` The device driver selector value returned by Open in a previous
call.  

**return** Reads the 16-bit ambient temperature register and returns the Centigrade temperature as a floating point value.

**throws** Throws a `DeviceException` in the event of an error while reading the device.

---
