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
#define BUF_SIZE 128
byte buf[BUF_SIZE];
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
  int bufIndex = 0;
  buf[bufIndex++] = 2;  // 4-wire interface
  buf[bufIndex++] = getLSBInt16(StepsPerRevolution);
  buf[bufIndex++] = getMSBInt16(StepsPerRevolution);
  for (int idx = 0; idx < 4; idx++) {
    buf[bufIndex++] = getLSBInt16(pin[idx]);
    buf[bufIndex++] = getMSBInt16(pin[idx]);
  }
  status = dd.control(handle, CDR_Configure, bufIndex, buf);
  Serial.println(status);

  Serial.print("Speed: ");
  bufIndex = 0;
  buf[bufIndex++] = getLSBInt16(RevolutionsPerMinute);
  buf[bufIndex++] = getMSBInt16(RevolutionsPerMinute);
  buf[bufIndex++] = 0;
  buf[bufIndex++] = 0;
  status = dd.control(handle, STP_RPMSpeed, bufIndex, buf);
  Serial.println(status);

  Serial.print("Versions: ");
  status = dd.status(handle, CDR_DriverVersion,BUF_SIZE,buf);
  Serial.print(status);
  if (status >= 0) {
    for (int idx=0; idx<status; idx++) {
      Serial.print(" ");
      Serial.print(buf[idx]);
    }
  }
  Serial.println();
}

void loop() {

  Serial.print("Move+: ");
  int msgIndex = 0;
  buf[msgIndex++] = getLSBInt16(15);
  buf[msgIndex++] = getMSBInt16(15);
  buf[msgIndex++] = 0;
  buf[msgIndex++] = 0;
  buf[msgIndex++] = 0;
  int status = dd.control(handle, STP_MoveR, msgIndex, buf);
  Serial.println(status);

  //delay(1000);

  Serial.print("Move-: ");
  msgIndex = 0;
  buf[msgIndex++] = getLSBInt16(-15);
  buf[msgIndex++] = getMSBInt16(-15);
  buf[msgIndex++] = -1;
  buf[msgIndex++] = -1;
  buf[msgIndex++] = 0;
  status = dd.control(handle, STP_MoveR, msgIndex, buf);
  Serial.println(status);

  //delay(1000);

}
