#include "CommonStepper.h"

/**
 * This abstract class defines the methods that are available to any
 * caller that wishes to control stepper motors.  The concrete subclasses
 * implement the methods as well as they can, depending on what underlying
 * library they use.  Not all libraries can support all the defined methods
 * fully, but the subclasses provide the best implementation they can, given
 * the constraints of their library.
 */

// FirmataExt calls these two methods on short, regular intervals.
// Subclasses should override if they want the chance to update motor state,
// change motor control parameters, report to the calling method, or other
// repetitive tasks.

  void CommonStepper::update(unsigned long deltaMicros) {}
  void CommonStepper::report(unsigned long deltaMillis) {}

  void CommonStepper::stop() {}
  void CommonStepper::reset() {}

  void CommonStepper::setMinPulseWidth(unsigned int minWidth) {}
