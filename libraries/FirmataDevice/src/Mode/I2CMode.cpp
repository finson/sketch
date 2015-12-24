#include "I2CMode.h"

I2CModeClass::I2CModeClass() {
  enabled = 0;
}

//---------------------------------------------------------------------------

void I2CModeClass::handleGetCapability(byte pin)
{
  if (IS_PIN_I2C(pin)) {
    Firmata.write(I2C);
    Firmata.write(1);  // to do: determine appropriate value
  }
}

boolean I2CModeClass::handleSetPinMode(byte pin, int mode)
{
  if (IS_PIN_I2C(pin)) {
    if (mode == I2C) {
      // the user must call I2C_CONFIG to enable I2C for a device
      return true;
    } else if (isEnabled()) {
      // disable i2c so pins can be used for other functions
      // the following if statements should reconfigure the pins properly
      if (Firmata.getPinMode(pin) == I2C) {
        disableI2CPins();
      }
    }
  }
  return false;
}

void I2CModeClass::reset()
{
  // queryIndex = -1;
  // i2cReadDelayTime = 0;
  I2CMode.disableI2CPins();

  // if (isI2CEnabled) {
  //   disableI2CPins();
  // }
}

//---------------------------------------------------------------------------

bool I2CModeClass::isEnabled() {
  return (enabled > 0);
}

void I2CModeClass::enableI2CPins() {
  if (!isEnabled()) {
    byte i;
    for (i = 0; i < TOTAL_PINS; i++) {
      if (IS_PIN_I2C(i)) {
        if (Firmata.getPinMode(i) != IGNORE) {
          // mark pins as i2c so they are ignore in non i2c data requests
          Firmata.setPinMode(i, I2C);
          pinMode(i, I2C);
        }
      }
    }
    Wire.begin();
  }
  enabled += 1;
}

/* disable the i2c pins so they can be used for other functions */
void I2CModeClass::disableI2CPins()
{
  if (isEnabled()) {
#if (defined WIRE_HAS_END) && (WIRE_HAS_END == 1)
    Wire.end();
#endif
    enabled -= 1;
  }
}

void I2CModeClass::write8(int addr, uint8_t reg, uint8_t val) {
  if (!isEnabled()) return;

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)val);
  Wire.endTransmission();
}

uint8_t I2CModeClass::read8(int addr, uint8_t reg) {
  uint16_t val;
  if (!isEnabled()) return 0;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)1);
  val = Wire.read();
  return val;
}

void I2CModeClass::write16(int addr, uint8_t reg, uint16_t val) {
  if (!isEnabled()) return;

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write(val >> 8);
  Wire.write(val & 0xFF);
  Wire.endTransmission();
}

uint16_t I2CModeClass::read16(int addr, uint8_t reg) {
  char errorMsg[100] ;
  uint16_t val;
  int numBytes = 2;
  if (!isEnabled()) return 0;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)numBytes);

    // check to be sure correct number of bytes were returned by slave
  if (numBytes < Wire.available()) {
    sprintf(errorMsg, "I2C read16: Too many bytes received from device.  Exp: %1d, Act: %1d",numBytes,Wire.available());
    Firmata.sendString(errorMsg);
  } else if (numBytes > Wire.available()) {
    sprintf(errorMsg, "I2C read16: Too few bytes received from device.  Exp: %1d, Act: %1d",numBytes,Wire.available());
    Firmata.sendString(errorMsg);
  }

  val = Wire.read();
  val <<= 8;
  val |= Wire.read();
  return val;
}

// make one instance to coordinate I2C enable/disable and IO across the app.

I2CModeClass I2CMode;
