#include <FirmataDeviceLibrary.h>
#include <Stepper/ArduinoStepper.h>

/**
   Test the operation of the ArduinoStepper motor control software.
   The ArduinoStepper class is an implementation of the FirmataDevice
   interface CommonStepper, using the standard Stepper library that
   comes with Arduino.
*/

ArduinoStepper *motor;

void setup() {
  Serial.begin(115200);
  motor = new ArduinoStepper(200, 4,5, 8,9);
  motor->setRPMSpeed(200);
  Serial.println("Ready.");
}

void loop() {

  Serial.print("a");
  Serial.print(motor->getCurrentPosition());
  motor->runToNewPosition(10);
  delay(1000);
  
  Serial.print("b");
  Serial.println(motor->getCurrentPosition());
  motor->runToNewPosition(-10);
  delay(1000);

}
