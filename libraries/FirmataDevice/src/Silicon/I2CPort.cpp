#include "I2CPort.h"

I2CPort::I2CPort() {
  Wire.begin();
}

I2CPort::~I2CPort() {
  Wire.end();
}

void I2CPort::enable() {}
void I2CPort::disable() {}
bool I2CPort::isEnabled() {return true;}

void I2CPort::writeUInt8(int addr, uint8_t reg, uint8_t *valueAddress) {
  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)(*valueAddress));
  Wire.endTransmission();
}

void I2CPort::writeUInt16LE(int addr, uint8_t reg, uint8_t *valueAddress) {
  uint8_t bytesToWrite[2];
  fromHostTo16LE(valueAddress,bytesToWrite);

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write(bytesToWrite[0]);
  Wire.write(bytesToWrite[1]);
  Wire.endTransmission();
}

void I2CPort::writeUInt16BE(int addr, uint8_t reg, uint8_t *valueAddress) {
  uint8_t bytesToWrite[2];
  fromHostTo16BE(valueAddress,bytesToWrite);

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write(bytesToWrite[0]);
  Wire.write(bytesToWrite[1]);
  Wire.endTransmission();
}

void I2CPort::writeUInt32LE(int addr, uint8_t reg, uint8_t *valueAddress) {
  uint8_t bytesToWrite[4];
  fromHostTo32LE(valueAddress,bytesToWrite);

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write(bytesToWrite[0]);
  Wire.write(bytesToWrite[1]);
  Wire.endTransmission();
}

void I2CPort::writeUInt32BE(int addr, uint8_t reg, uint8_t *valueAddress) {
  uint8_t bytesToWrite[4];
  fromHostTo32BE(valueAddress,bytesToWrite);

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.write(bytesToWrite[0]);
  Wire.write(bytesToWrite[1]);
  Wire.endTransmission();
}

uint8_t I2CPort::readUInt8(int addr, uint8_t reg) {

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)1);
  return (uint8_t)Wire.read();
}

uint16_t I2CPort::readUInt16LE(int addr, uint8_t reg) {
  uint8_t bytesRead[2];
  uint16_t value;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)2);

  bytesRead[0] = Wire.read();
  bytesRead[1] = Wire.read();

  from16LEToHost(bytesRead,&value);
  return value;
}

uint16_t I2CPort::readUInt16BE(int addr, uint8_t reg) {
  uint8_t bytesRead[2];
  uint16_t value;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)2);

  bytesRead[0] = Wire.read();
  bytesRead[1] = Wire.read();

  from16BEToHost(bytesRead,&value);
  return value;
}

uint32_t I2CPort::readUInt32LE(int addr, uint8_t reg) {
  uint8_t bytesRead[4];
  uint32_t value;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)4);

  bytesRead[0] = Wire.read();
  bytesRead[1] = Wire.read();
  bytesRead[2] = Wire.read();
  bytesRead[3] = Wire.read();

  from32LEToHost(bytesRead,&value);
  return value;
}

uint32_t I2CPort::readUInt32BE(int addr, uint8_t reg) {
  uint8_t bytesRead[4];
  uint32_t value;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)addr, (uint8_t)4);

  bytesRead[0] = Wire.read();
  bytesRead[1] = Wire.read();
  bytesRead[2] = Wire.read();
  bytesRead[3] = Wire.read();

  from32BEToHost(bytesRead,&value);
  return value;
}
