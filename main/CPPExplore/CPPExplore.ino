//#include <LuniLib.h>
//
//#include <Framework/Tester.h>
//#include <Framework/Logger.h>
//#include <Framework/Boards.h>

#include "trial.h"
//
//Logger logger("Explorer");
//Tester tst;

//--------------------------------------------------------------------
  
void setup() {
  char buf[256];
  char *myName;

  // Countdown before running to give time to open the monitor window.

  Serial.begin(115200);

  delay(3000);
  for (int idx = 0; idx < 5; idx++) {
    Serial.print(5 - idx);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();
//
//  //  Create the objects needed for the testing framework.
//
//  logger.setCurrentLogLevel(LogLevel::DEBUG);
//  logger.info("CPPExplore");

//-----------------------------

  JAMIT(__func__)
  Serial.println(myName);

  A *obj = new A();

//logger.info("Boards.h");
//logger.info("TOTAL_ANALOG_PINS",TOTAL_ANALOG_PINS);
//logger.info("TOTAL_PINS",TOTAL_PINS);
//logger.info("VERSION_BLINK_PIN",VERSION_BLINK_PIN);
//logger.info("PIN_SERIAL1_RX",PIN_SERIAL1_RX);
//logger.info("PIN_SERIAL1_TX",PIN_SERIAL1_TX);

//logger.info("pins_arduino.h");
//logger.info("NUM_ANALOG_INPUTS",NUM_ANALOG_INPUTS);
//logger.info("NUM_DIGITAL_PINS",NUM_DIGITAL_PINS);
//logger.info("LED_BUILTIN",LED_BUILTIN);
//logger.info("A0",A0);
//logger.info("SDA",SDA);
//
//for (int idx=0; idx<NUM_DIGITAL_PINS; idx++) {
//  Serial.print(idx);
//  if (IS_PIN_DIGITAL(idx)) Serial.print(" Digital");
//  if (IS_PIN_ANALOG(idx)) {
//    Serial.print(" Analog");
//    Serial.print(" ");
//    itoa(PIN_TO_ANALOG(idx),buf,10);
//    Serial.print(buf);
//  }
//  if (IS_PIN_PWM(idx)) Serial.print(" PWM");
////  if (IS_PIN_SERVO(idx)) Serial.print(" Servo");
//  if (IS_PIN_I2C(idx)) Serial.print(" I2C");
//  if (IS_PIN_SPI(idx)) Serial.print(" SPI");
//  if (IS_PIN_INTERRUPT(idx)) Serial.print(" Interrupt");
//  if (IS_PIN_SERIAL(idx)) Serial.print(" Serial");
//  Serial.println();
//}
}

//--------------------------------------------------------------------
  
void loop() {}


