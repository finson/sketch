##Current Plan

* **Done**.  Use raw I2C to control device from client.
* **Done**.  Write simple device driver to control device from client (java).
* **Done**.  Make provisions for multiple command input streams (Firmata and direct call)
* Write simple device driver to control virtual device on the server (Hello).
* Write simple device driver to control device from server (MCP9808).
* Implement board level libraries as Features of type Port.  I2C, Stepper
* Write Stepper device driver
* Wire, SPI, OneWire Port Features
* other device drivers




##Deferred Features and changes.

* Logging facility as a FirmataFeature.
* Error value return facility (as a FirmataFeature?).
* Task scheduler as FirmataFeature.
* Restore FirmataFeature method names as in ConfigurableFirmata.
* **Done**. Give a little more thought on how to get rid of the double initializaton of cmd in Java message classes.
