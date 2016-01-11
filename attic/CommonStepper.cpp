#include "CommonStepper.h"

/**
 * This abstract class defines the methods that are available to any
 * caller that wishes to control stepper motors.  The concrete subclasses
 * implement the methods as well as they can, depending on what underlying
 * library they use.  Not all libraries can support all the defined methods
 * fully, but the subclasses provide the best implementation they can, given
 * the constraints of their library.
 */

// If a concrete subclass needs to update motor state, change motor
// control parameters, report to the calling method, or other repetitive
// tasks it should override one or both of the following two methods and
// then ensure that they are called on short, regular intervals as needed.

  void CommonStepper::microsecondUpdate(unsigned long deltaMicros) {}
  void CommonStepper::millisecondReport(unsigned long deltaMillis) {}

  void CommonStepper::stop() {}
  void CommonStepper::reset() {}

  void CommonStepper::setMinPulseWidth(unsigned int minWidth) {}
