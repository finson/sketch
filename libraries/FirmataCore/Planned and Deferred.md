##Current Plan

* **Done**.  Use raw I2C to control device from client.
* **Done**.  Write device driver to control device from client (MCP9808).
* **Done**.  Make provisions for multiple command input streams (Firmata and direct call)
* *Revert*. Implement board level libraries as Features of type Mode.  I2C.  **No**, remove Mode altogether and fold back in to Features.
* **Done** v0.3.  Write device driver to control device from server (MCP9808).
* Write server device driver to control virtual device (Hello).
* Client - handle async message arrival
* Write Stepper device controllers (raw, client, server)
* Wire, SPI, OneWire Mode Features
* other device drivers
* spec - add discussion of the use of report() and update()
* spec - add discussion of the interface between Features and DeviceDrivers.
* **Done** Add status register code "get device driver version"
* Debug info defined by LUN, retrieved with read
* Put tools ram method in Peek and assign it a LUN


##Deferred Features and changes.

* Logging facility as a FirmataFeature.
* **Done** Error value return facility (as a FirmataFeature?).
* Task scheduler as FirmataFeature.
* Restore FirmataFeature method names as in ConfigurableFirmata.
* **Done**. Give a little more thought on how to get rid of the double initializaton of cmd in Java message classes.
* Add status register code "get installed device driver count"
