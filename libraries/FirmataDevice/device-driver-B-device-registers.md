
----------
*tbd* 2016 V 0.2.0 beta.  Doug Johnson (finson@whidbey.com) 

----------

## DeviceFeature and Device Drivers - Appendix B: Device Registers and Parameter Block Layout

The document device-driver.md describes the overall device feature proposal and most parts of the detailed design.  Appendix A lists the standard virtual registers that all devices can implement.

This Appendix B defines the registers that are actually implemented by several known device drivers and the detailed format of the parameter block bodies they expect.

Register names starting with CCR_ refer to **C**ommon **C**ontrol **R**egisters.  Names starting with STP_ refer to **ST**e**P**per registers.

##StepperDriverBasic

###Control Register `CCR_Configure`

####2-wire (unipolar) motor

     0  steps per revolution (LSB)
     1  steps per revolution (MSB)
     2  pin1 (LSB)
     3  pin1 (MSB)
     4  pin2 (LSB)
     5  pin2 (MSB)

####4-wire (bipolar) motor

     0  steps per revolution (LSB)
     1  steps per revolution (MSB)
     2  pin1 (LSB)
     3  pin1 (MSB)
     4  pin2 (LSB)
     5  pin2 (MSB)
     6  pin3 (LSB)
     7  pin3 (MSB)
     8  pin4 (LSB)
     9  pin4 (MSB)

---
###Control Register `STP_MoveR`

####Move to new relative position

     0  delta position (steps) (LSB)
     1  delta position (steps) (MSB)
     2  block? (0 or 1)

---
###Control Register `STP_RPMSpeed`

####Set stepping speed using Revolutions Per Minute

     0  RPM (LSB)
     1  RPM (MSB)

---