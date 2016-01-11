#include <FirmataDeviceLibrary.h>
#include <Stepper/StepperDriverBasic.h>

/**
   Test the operation of the motor control software.
   The StepperDriverBasic class is one implementation of the
   StepperDriver abstract class, using the standard Stepper 
   library that comes with Arduino.
*/

StepperDriverBasic dd("STD",6);

void setup() {
  Serial.begin(115200);
  delay(3000);
  for (int idx=5; idx>0; idx--) {
  Serial.println(idx);
  delay(1000);
  }
  Serial.print("Open. ");
  int status = dd.open("STD:0");
  Serial.println(status);
}

void loop() {

  Serial.print("a");
//  Serial.print(motor->getCurrentPosition());
//  motor->runToNewPosition(10);
  delay(1000);

  Serial.print("b");
//  Serial.println(motor->getCurrentPosition());
//  motor->runToNewPosition(-10);
  delay(1000);

}
