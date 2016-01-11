#ifndef StepperDriverBasic_h
#define StepperDriverBasic_h

#include "StepperDriver.h"
#include <Stepper.h>

/**
 * This class defines a stepper motor device driver, using
 * the basic blocking Stepper library that is standard with Arduino.
 */
class StepperDriverBasic: public StepperDriver {

public:

  StepperDriverBasic(const char *dName, int addrCount);

  // virtual int open(const char *name, int flags = 0);
  virtual int status(int handle, int reg, int count, byte *buf);
  virtual int control(int handle, int reg, int count, byte *buf);
  virtual int read(int handle, int count, byte *buf);
  virtual int write(int handle, int count, byte *buf);
  virtual int close(int handle);


private:
  int executeCCR_Configure(int handle, int reg, int count, byte *buf);

};

#endif
