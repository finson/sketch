#ifndef StepperDriverAsync_h
#define StepperDriverAsync_h

#include <Device/DeviceDriver.h>
#include <Stepper/StepperDriver.h>
#include <AsyncStepper.h>

const PROGMEM uint8_t driverSemVer[] = {6,0,1,0,0,0,0};
const PROGMEM char driverName[] = {"StepperDriverAsync"};

const PROGMEM uint8_t librarySemVer[] = {6,0,0,0,0,0,0};
const PROGMEM char libraryName[] = {"AsyncStepper"};

/**
 * This class defines a stepper motor device driver using the asynchronous
 * FirmataStepper library that is standard with Firmata.  The library was
 * renamed AsyncStepper for this use but otherwise it is the exact same code
 * as comes standard with Firmata.
 */
class StepperDriverAsync: public DeviceDriver {

public:

  StepperDriverAsync(const char *dName, int addrCount);

  virtual int open(const char *name, int flags = 0);
  virtual int status(int handle, int reg, int count, byte *buf);
  virtual int control(int handle, int reg, int count, byte *buf);
  virtual int read(int handle, int count, byte *buf);
  virtual int write(int handle, int count, byte *buf);
  virtual int close(int handle);

  int millisecondTimeBase();
  int microsecondTimeBase();

private:

  int controlCDR_Configure(int handle, int reg, int count, byte *buf);
  int controlSTP_MoveR(int handle, int reg, int count, byte *buf);
  int controlSTP_RPMSpeed(int handle, int reg, int count, byte *buf);

  int statusCDR_DriverVersion(int handle, int reg, int count, byte *buf);
  int statusCDR_LibraryVersion(int handle, int reg, int count, byte *buf);

class StepperDriverAsyncLUI: public LogicalUnitInfo {

    public:

        StepperDriverAsyncLUI() : LogicalUnitInfo(),
        theSpeed(50), theAcceleration(0), theDeceleration(0) { }

        void setRPMSpeed(int s) { theSpeed = s; }
        void setRPMAccelerations(int a, int d) {
          theAcceleration = a;
          theDeceleration = d;
        }

        int getRPMSpeed() { return theSpeed; }
        int getRPMAcceleration() { return theAcceleration; }
        int getRPMDeceleration() { return theDeceleration; }

    private:
        int theSpeed;
        int theAcceleration;
        int theDeceleration;
    };
};

#endif
