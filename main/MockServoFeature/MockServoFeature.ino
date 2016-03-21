#include <Servo.h>

/**
   Use the underlying library of a FirmataFeature to make sure I know how it should work.
*/

#define MSG_BUFFER_SIZE 256
#define MIN_CHANNEL_INTERVAL 500
#define MIN_SCAN_INTERVAL 1

int digitalPinCount = 0;
int analogPinCount = 0;
int channelCount = 0;

int scanCount = 240;
int channelInterval = 500;          // microseconds
int scanInterval = 10;             // milliseconds

int digitalPins[] = { -1};
int analogPins[] = {A6, A7, -1};
char msgBuffer[MSG_BUFFER_SIZE];

int currentScanIndex;
int currentChannelIndex;

int dataBufferSize;
int *dataBuffer;

int servoPin = 11;
Servo *theServo;
int angles[] = {0, 90, 180, 90, 30, 15, 7, 3, 1, 0};
int numberOfAngles = 10;
int angleIndex = 0;

void setup() {
  Serial.begin(115200);
  delay(3000);
  for (int idx = 5; idx > 0; idx--) {
    Serial.print(idx);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();
  Serial.println("------------\n");

  // servo config

  byte pin = servoPin;

  int minPulse = 544;
  int maxPulse = 2400;

  theServo = new Servo();
  int index = theServo->attach(pin, minPulse, maxPulse);
  Serial.print("servo index: ");
  Serial.println(index);

}

void loop() {
  snprintf(msgBuffer,MSG_BUFFER_SIZE,"Angle %1d is %1d.", angleIndex, angles[angleIndex]);
  Serial.println(msgBuffer);
  theServo->write(angles[angleIndex]);
  angleIndex = (angleIndex+1) % numberOfAngles;
  delay(1000);
}

