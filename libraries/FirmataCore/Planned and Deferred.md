##Current Plan

* **Done**.  Use raw I2C to control device from client.
* **Done**.  Write device driver to control device from client (MCP9808).
* **Done**.  Make provisions for multiple command input streams (Firmata and direct call)
* **Done**. Implement board level libraries as Features of type Mode.  I2C.  **No**, remove Mode altogether and fold back in to Features.
* **Done** v0.3.  Write device driver to control device from server (MCP9808).
* **Done** Write server device driver to control virtual device (Hello).
* **Done** Add status register code "get device driver version"
* Add status register code "get installed device drivers"
* Write Stepper device controllers (raw, client, server)
* Client - handle async message arrival
* Wire, SPI, OneWire Mode Ports
* more device drivers
* spec - add discussion of the use of report() and update()
* spec - add discussion of the interface between Features and DeviceDrivers.
* pick up - add delegating dispatchTimers() and addSelectedFeatures() to Firmata.
* pick up - take dispatch timers code out of DeviceTable
* Add separate DeviceTableDriver object to the driver list during DeviceTable initialization and use it for things like "status() - list of installed drivers"

##Deferred Features and changes.

* **Done** Error value return facility.
* **Done**. Get rid of the double initializaton of cmd in Java message classes.
* Logging facility as a FirmataFeature.
* Task scheduler as FirmataFeature.
* Restore FirmataFeature method names as in ConfigurableFirmata.
* Debug info defined by LUN, retrieved with read (?)
* Put tools ram method in Peek
