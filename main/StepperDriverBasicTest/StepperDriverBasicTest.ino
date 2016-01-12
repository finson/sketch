#include <FirmataDeviceLibrary.h>
#include <Stepper/StepperDriverBasic.h>

/**
   Test the operation of the motor control software.
   The StepperDriverBasic class is one implementation of the
   StepperDriver abstract class, using the standard Stepper
   library that comes with Arduino.
*/

#define StepsPerRevolution 686
#define RevolutionsPerMinute 10

StepperDriverBasic dd("STP", 8);
int pin[] = {4, 5, 8, 9};
byte buf[16];
int handle;

void setup() {
  Serial.begin(115200);
  delay(3000);
  for (int idx = 5; idx > 0; idx--) {
    Serial.println(idx);
    delay(1000);
  }

  Serial.print("Open: ");
  int status = dd.open("STP:0");
  Serial.println(status);

  Serial.print("Config: ");
  handle = status;
  buf[0] = getLSBInt16(StepsPerRevolution);
  buf[1] = getMSBInt16(StepsPerRevolution);
  for (int idx = 0; idx < 4; idx++) {
    buf[(2 * idx) + 2] = getLSBInt16(pin[idx]);
    buf[(2 * idx) + 2 + 1] = getMSBInt16(pin[idx]);
  }
  status = dd.control(handle, CCR_Configure, 10, buf);
  Serial.println(status);

  Serial.print("Speed: ");
  buf[0] = getLSBInt16(RevolutionsPerMinute);
  buf[1] = getMSBInt16(RevolutionsPerMinute);
  status = dd.control(handle, STP_RPMSpeed, 2, buf);
  Serial.println(status);
}

void loop() {

  Serial.print("Move+: ");
  buf[0] = getLSBInt16(15);
  buf[1] = getMSBInt16(15);
  buf[2] = 0;
  int status = dd.control(handle, STP_MoveR, 3, buf);
  Serial.println(status);

  //delay(1000);


  Serial.print("Move-: ");
  buf[0] = getLSBInt16(-15);
  buf[1] = getMSBInt16(-15);
  buf[2] = 0;
  status = dd.control(handle, STP_MoveR, 3, buf);
  Serial.println(status);

  //delay(1000);

}
