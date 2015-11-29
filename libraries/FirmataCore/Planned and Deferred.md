##Current Plan

* Use raw I2C to control device from client.
* Make provisions for multiple command input streams (Firmata and direct call)
* Write simple device driver to control device from client (java).
* Write simple device driver to control device from server (arduino).
* Actual board level libraries.  Wire, SPI, OneWire
* Write fancier device driver to control display from server
* Implement OneWirePort, other Ports



##Deferred Features and changes.

* Logging facility as a FirmataFeature.
* Task scheduler as FirmataFeature.
* Restore FirmataFeature method names as in ConfigurableFirmata.
* _Give a little more thought on how to get rid of the double initializaton of cmd in Java message classes.__
