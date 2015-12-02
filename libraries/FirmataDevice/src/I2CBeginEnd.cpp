#include "I2CBeginEnd.h"

bool I2CBeginEnd::isI2CEnabled = false;

I2CBeginEnd::I2CBeginEnd() {
  enableI2CPins();
}

bool I2CBeginEnd::enableI2CPins() {
  if (!I2CBeginEnd::isI2CEnabled) {
    byte i;
    // is there a faster way to do this? would probaby require importing
    // Arduino.h to get SCL and SDA pins
    for (i = 0; i < TOTAL_PINS; i++) {
      if (IS_PIN_I2C(i)) {
        if (Firmata.getPinMode(i) == IGNORE) {
          return false;
        }
        // mark pins as i2c so they are ignore in non i2c data requests
        Firmata.setPinMode(i, I2C);
        pinMode(i, I2C);
      }
    }
    Wire.begin();
    I2CBeginEnd::isI2CEnabled = true;
  }
}

/* disable the i2c pins so they can be used for other functions */
void I2CBeginEnd::disableI2CPins()
{
  if (I2CBeginEnd::isI2CEnabled) {
    Wire.end();
    I2CBeginEnd::isI2CEnabled = false;
  }
}
