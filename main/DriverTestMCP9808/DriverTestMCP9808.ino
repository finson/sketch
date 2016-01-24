#include <FirmataDeviceLibrary.h>
#include <Device/DeviceTable.h>
#include <Framework/Logger.h>
#include <Framework/Tester.h>
#include <Framework/ByteOrder.h>
#include <elapsedMillis.h>
#include "SelectedDeviceDrivers.h"

/**
   Test the operation of the MCP9808 Temperature Sensor.
*/

#define BUF_SIZE 256
byte buf[BUF_SIZE];
int vReg[] = {static_cast<int>(CDR::DriverVersion), static_cast<int>(CDR::LibraryVersion)};

bool waitingForInput = false;
elapsedMillis millisecondsSinceLastInputCheck;
unsigned int checkInterval = 500;

DeviceTable *dt;
Tester *tst;
Logger *logger;

// --------------------------------------------------------

void setup() {

  // Countdown before running to give time to open the monitor window.

  Serial.begin(115200);
  Serial.println("I'm starting ...");
 
  delay(3000); 
  for (int idx = 0; idx < 5; idx++) {
    Serial.println(5-idx);
    delay(1000);
  }

  //  Create the objects needed for the testing framework.

  logger = new Logger("DriverTestMCP9808");
  logger->setCurrentLogLevel(LogLevel::INFO);

  dt = new DeviceTable(selectedDevices);
  tst = new Tester();
}

// Run the test group once, then again each time the user presses the enter key

void loop() {
  if (!waitingForInput) {
    tst->beforeGroup("MCP9808");

    // --------------------------------------------------------

    tst->beforeTest("MCP9808OpenClose");
    int status = dt->open("TempSensor:0", 0);
    tst->assertTrue("Open error.", (status >= 0));
    if (status >= 0) {
      int handle = status;
      status = dt->close(handle);
      tst->assertTrue("Close error.", (status >= 0));
    }
    tst->afterTest();

    // --------------------------------------------------------

    tst->beforeTest("MCP9808Read");
    status = dt->open( "TempSensor:0");
    tst->assertTrue("Open error.", (status >= 0));

    if (status >= 0) {
      int handle = status;
      status = dt->read(handle, 2, buf);
      tst->assertEquals("Read error:", (uint16_t)2, (uint16_t)status);
      logger->info("Read 2 bytes:", from16BEToHost(buf));
      status = dt->close(handle);
      tst->assertTrue("Close error.", (status >= 0));
    }
    tst->afterTest();

    // --------------------------------------------------------

    tst->beforeTest("MCP9808Status");
    status = dt->open("TempSensor:0");
    tst->assertTrue("Open error.", (status >= 0));
    if (status >= 0) {
      int handle = status;

      for (int r = 1; r < 9; r++) {
        int count = (r == 8) ? 1 : 2;

        status = dt->status(handle, r, count, buf);
        tst->assertEquals("Status error:", (uint32_t)count, (uint32_t)status);

        if (count == 1) {
          logger->info("Read 1 status byte from reg ", r);
        } else {
          logger->info("Read 2 status bytes from reg ", r);
        }

        switch (r) {
          case static_cast<int>(MCP9808Register::MANUF_ID):
            logger->info("Manufacturer ID: ", from16BEToHost(buf));
            tst->assertEquals("Unexpected MCP9808 MANUF_ID", 0x54, from16BEToHost(buf));
            break;
          case static_cast<int>(MCP9808Register::DEVICE_ID):
            logger->info("Device ID: ", from16BEToHost(buf));
            tst->assertEquals("Unexpected MCP9808 DEVICE_ID", 0x400, from16BEToHost(buf));
            break;
        }
        status = dt->close(handle);
        tst->assertTrue("Close error.", (status >= 0));
      }
      tst->afterTest();

      // --------------------------------------------------------

      tst->afterGroup();

      // --------------------------------------------------------

      waitingForInput = true;
      millisecondsSinceLastInputCheck = 0;
      Serial.print("Press enter key to repeat test: ");
    }

    // Idle until we see an enter key press

    int incomingByte;
    if ((millisecondsSinceLastInputCheck > checkInterval) && Serial.available()) {
      incomingByte = Serial.read();
      Serial.println(incomingByte, HEX);
      if ((incomingByte & 0x00FF) == '\n') {
        waitingForInput = false;
      }
    }
  }
}


//   //  Serial.print("Config: ");
//   handle = status;
//   int bufIndex = 0;
//   //  buf[bufIndex++] = 2;  // 4-wire interface
//   //  buf[bufIndex++] = getLSBInt16(StepsPerRevolution);
//   //  buf[bufIndex++] = getMSBInt16(StepsPerRevolution);
//   //  for (int idx = 0; idx < 4; idx++) {
//   //    buf[bufIndex++] = getLSBInt16(pin[idx]);
//   //    buf[bufIndex++] = getMSBInt16(pin[idx]);
//   //  }
//   //  status = dd->control(handle, CDR_Configure, bufIndex, buf);
//   //  Serial.println(status);
//   //
//   //  Serial.print("Speed: ");
//   //  bufIndex = 0;
//   //  buf[bufIndex++] = getLSBInt16(RevolutionsPerMinute);
//   //  buf[bufIndex++] = getMSBInt16(RevolutionsPerMinute);
//   //  buf[bufIndex++] = 0;
//   //  buf[bufIndex++] = 0;
//   //  status = dd->control(handle, STP_RPMSpeed, bufIndex, buf);
//   //  Serial.println(status);

//   for (int regIndex = 0; regIndex < 2; regIndex++) {
//     Serial.print("Version: ");
//     status = dt->status(handle, vReg[regIndex], BUF_SIZE, buf);
//     Serial.print(status);
//     Serial.print(' ');
//     if (status >= 0) {
//       bufIndex = 0;
//       int packetSize = buf[bufIndex++];
//       for (int idx = 0; idx < packetSize; idx++) {
//         Serial.print(buf[bufIndex++]);
//         switch (idx) {
//           case 0:
//           case 1:
//             Serial.print('.');
//             break;
//           case 2:
//             Serial.print(' - ');
//             break;
//           case 3:
//           case 4:
//             Serial.print('.');
//             break;
//           case 5:
//             Serial.print(' ');
//             break;
//         }
//       }
//       Serial.write(reinterpret_cast<char *>(&buf[bufIndex]));
//     }
//     Serial.println();
//   }
//}

//
//  Serial.print("Move+: ");
//  int msgIndex = 0;
//  buf[msgIndex++] = getLSBInt16(15);
//  buf[msgIndex++] = getMSBInt16(15);
//  buf[msgIndex++] = 0;
//  buf[msgIndex++] = 0;
//  buf[msgIndex++] = 0;
//  int status = dd->control(handle, STP_MoveR, msgIndex, buf);
//  Serial.println(status);
//
//  //delay(1000);
//
//  Serial.print("Move-: ");
//  msgIndex = 0;
//  buf[msgIndex++] = getLSBInt16(-15);
//  buf[msgIndex++] = getMSBInt16(-15);
//  buf[msgIndex++] = -1;
//  buf[msgIndex++] = -1;
//  buf[msgIndex++] = 0;
//  status = dd->control(handle, STP_MoveR, msgIndex, buf);
//  Serial.println(status);
//
//  //delay(1000);


