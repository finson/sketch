#include <FirmataDeviceLibrary.h>

/**
   Acquire a bunch of samples at a given rate, then spew them out the serial line as CSV
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

  channelInterval = max(channelInterval, MIN_CHANNEL_INTERVAL);
  scanInterval = max(scanInterval, MIN_SCAN_INTERVAL);

  while (digitalPins[digitalPinCount] >= 0) {
    digitalPinCount++;
  }

  while (analogPins[analogPinCount] >= 0) {
    analogPinCount++;
  }
  channelCount = digitalPinCount + analogPinCount;

  snprintf(msgBuffer, MSG_BUFFER_SIZE, "digital pins: (%1d): ", digitalPinCount);
  Serial.print(msgBuffer);
  for (int idx = 0; idx < digitalPinCount; idx++) {
    itoa(digitalPins[idx], msgBuffer, 10);
    Serial.print(msgBuffer);
    Serial.print(" ");
  }
  Serial.println();


  snprintf(msgBuffer, MSG_BUFFER_SIZE, "analog pins:  (%1d): ", analogPinCount);
  Serial.print(msgBuffer);
  for (int idx = 0; idx < analogPinCount; idx++) {
    itoa(analogPins[idx], msgBuffer, 10);
    Serial.print(msgBuffer);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println();

  snprintf(msgBuffer, MSG_BUFFER_SIZE, "scan count: %1d, channels per scan: %1d", scanCount, channelCount);
  Serial.println(msgBuffer);
  snprintf(msgBuffer, MSG_BUFFER_SIZE, "channel interval (microseconds): %1d", channelInterval);
  Serial.println(msgBuffer);
  snprintf(msgBuffer, MSG_BUFFER_SIZE, "scan interval (milliseconds): %1d", scanInterval);
  Serial.println(msgBuffer);

  int runDuration = (((digitalPinCount + analogPinCount) * channelInterval) / 1000 + scanInterval) * scanCount;
  snprintf(msgBuffer, MSG_BUFFER_SIZE, "expected run duration: %1d milliseconds", runDuration);
  Serial.println(msgBuffer);

  Serial.println();
  Serial.println("------------");

  currentScanIndex = 0;
  currentChannelIndex = 0;
  dataBufferSize = channelCount * scanCount;
  dataBuffer = new int[dataBufferSize];

  if (dataBuffer == 0) {
    Serial.print("ERROR: Not enough memory for data buffer allocation.");
  }

}

void loop() {
  if (dataBuffer == 0) return;
  int currentStoragePointer = currentScanIndex * channelCount;

  if (currentScanIndex == 0) {
    Serial.println("-- start of run");
    Serial.println("-- start of data acquisition");
  }

  for (int idx = 0; idx < digitalPinCount; idx++) {
    dataBuffer[currentStoragePointer++] = digitalRead(digitalPins[idx]);
    delayMicroseconds(channelInterval);
  }

  for (int idx = 0; idx < analogPinCount; idx++) {
    dataBuffer[currentStoragePointer++] = analogRead(analogPins[idx]);
    delayMicroseconds(channelInterval);
  }

  currentScanIndex += 1;

  if (currentScanIndex >= scanCount) {
    Serial.println("-- end of data acquisition");
  
    int currentStorageIndex = 0;

    for (int scanNumber = 0; scanNumber < scanCount; scanNumber++) {
      Serial.print("Scan ");
      Serial.print(scanNumber);
      for (int channelIndex = 0; channelIndex < channelCount; channelIndex++) {
        snprintf(msgBuffer, MSG_BUFFER_SIZE, ",%1d", dataBuffer[currentStorageIndex++]);
        Serial.print(msgBuffer);
      }
      Serial.println();
    }

    Serial.println("--end of run\n");

    currentScanIndex = 0;
    currentChannelIndex = 0;

    delay(5000);
  }
}
