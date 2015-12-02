#include "I2CReadWrite.h"
#include "I2CBeginEnd.h"

I2CReadWrite::I2CReadWrite(int addr) : i2cEnabler() {
}

void I2CReadWrite::setDeviceAddress(int addr) {
  theDeviceAddress = addr;
}

int I2CReadWrite::getDeviceAddress() {
  return theDeviceAddress;
}

void I2CReadWrite::write8(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(theDeviceAddress);
  Wire.write((uint8_t)reg);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t I2CReadWrite::read8(uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(theDeviceAddress);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)theDeviceAddress, (uint8_t)1);
  val = Wire.read();
  return val;
}

void I2CReadWrite::write16(uint8_t reg, uint16_t val) {
  Wire.beginTransmission(theDeviceAddress);
  Wire.write((uint8_t)reg);
  Wire.write(val >> 8);
  Wire.write(val & 0xFF);
  Wire.endTransmission();
}

uint16_t I2CReadWrite::read16(uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(theDeviceAddress);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)theDeviceAddress, (uint8_t)2);
  val = Wire.read();
  val <<= 8;
  val |= Wire.read();
  return val;
}
