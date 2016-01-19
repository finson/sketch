#include <FirmataDeviceLibrary.h>
#include <Silicon/ByteSwap.h>

uint8_t one[] = { 0,1,2,3};

void setup() {
  byte resultWord;
  fromHostTo16LE((uint8_t *)&one,(uint8_t *)&resultWord);
  Serial.print(resultWord,HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
