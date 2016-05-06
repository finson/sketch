#include <LuniLib.h>
#include "trial.h"

//#include <Arduino.h>
//#include "boards.h"

#include <Framework/Tester.h>
#include <Framework/Logger.h>
#include <Framework/Boards.h>

Logger logger("Explorer");
Tester tst;

//--------------------------------------------------------------------

void setup() {
  char buf[256];

  // Countdown before running to give time to open the monitor window.

  Serial.begin(115200);

  delay(3000);
  for (int idx = 0; idx < 5; idx++) {
    Serial.print(5 - idx);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();

  //  Create the objects needed for the testing framework.

  logger.setCurrentLogLevel(LogLevel::DEBUG);
  logger.info("CPPExplore");

  //-----------------------------

  //logger.info("Boards.h");
  //logger.info("TOTAL_PINS",TOTAL_PINS);
  //logger.info("TOTAL_ANALOG_PINS",TOTAL_ANALOG_PINS);

  //for (int idx=0; idx<128; idx++) {
  //  Serial.print(idx);
  //  if (IS_PIN_DIGITAL(idx)) {
  //    Serial.print(" Digital");
  //  }
  //  if (IS_PIN_ANALOG(idx)) {
  //    Serial.print(" Analog");
  //    Serial.print(" ");
  //    itoa(PIN_TO_ANALOG(idx),buf,10);
  //    Serial.print(buf);
  //  }
  //  if (IS_PIN_PWM(idx)) {
  //    Serial.print(" PWM");
  //  }
  ////  if (IS_PIN_SERVO(idx)) Serial.print(" Servo");
  //  if (IS_PIN_I2C(idx)) {
  //    Serial.print(" I2C");
  //  }
  //  if (IS_PIN_SPI(idx)) {
  //    Serial.print(" SPI");
  //  }
  //  if (IS_PIN_INTERRUPT(idx)) {
  //    Serial.print(" Interrupt");
  //  }
  //  if (IS_PIN_SERIAL(idx)) {
  //    Serial.print(" Serial");
  //  }
  //  Serial.println();
  //}

  logger.info("Arduino.h and pins_arduino.h");
  logger.info("NUM_DIGITAL_PINS", NUM_DIGITAL_PINS);
  logger.info("NUM_ANALOG_INPUTS", NUM_ANALOG_INPUTS);
  logger.info("LED_BUILTIN", LED_BUILTIN);
  logger.info("A0", A0);

  int analogPinNumber[128];
  for(int i = 0; i < 128; i++) analogPinNumber[i] = -1;
  
  for (int i = 0; i<128; i++) {
    int digitalPin = analogInputToDigitalPin(i);
    if (digitalPin != -1) {
      analogPinNumber[digitalPin] = i;
    }
  }

  for (int idx = 0; idx < 128; idx++) {
    Serial.print(idx);

    if (isPinDigital(idx)) {
      Serial.print(" Digital");
    }

    if (digitalPinHasPWM(idx)) {
      Serial.print(" PWM");
    }

    if (isPinI2C(idx)) {
      Serial.print(" I2C");
    }
    
    if (isPinSPI(idx)) {
      Serial.print(" SPI");
    }

    if (digitalPinToInterrupt(idx) != NOT_AN_INTERRUPT) {
      Serial.print(" Interrupt ");
      itoa(digitalPinToInterrupt(idx), buf, 10);
      Serial.print(buf);
    }

    if (analogPinNumber[idx] != -1) {
      Serial.print(" Analog ");
      itoa(analogPinNumber[idx], buf, 10);
      Serial.print(buf);
    }

    Serial.println();
  }
}



//--------------------------------------------------------------------

void loop() {}


