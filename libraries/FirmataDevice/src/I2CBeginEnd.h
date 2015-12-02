#ifndef I2CBeginEnd_h
#define I2CBeginEnd_h

#include <Wire.h>
#include <FirmataFeature.h>

class I2CBeginEnd {

public:

  I2CBeginEnd();
  bool enableI2CPins();
  void disableI2CPins();

  static bool isI2CEnabled;

private:

};

#endif
