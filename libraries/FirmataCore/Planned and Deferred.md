##Current Plan

* Use raw I2C to control device from client.
* Make provisions for multiple command input streams.
* Write simple device driver to control device from server (arduino).
* Write fancier device driver to control display from server
* Implement OneWirePort, other Ports



##Deferred Features and changes.

* Logging facility as a FirmataFeature.
* Restore FirmataFeature method names as in ConfigurableFirmata.
* Give a little more thought on how to get rid of the double initializaton of cmd in Java message classes.
