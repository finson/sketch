#include <FirmataDeviceLibrary.h>
#include <Silicon/ByteOrder.h>

uint8_t one[] = { 0,1,2,3};

void setup() {
  
  Serial.begin(115200);
  delay(3000);
  for (int idx = 5; idx > 0; idx--) {
    Serial.println(idx);
    delay(1000);
  }

  uint16_t resultWord;
  resultWord = from16LEToHost(one);
  Serial.print(resultWord,HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
