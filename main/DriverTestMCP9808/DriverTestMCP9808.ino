#include <FirmataDeviceLibrary.h>
#include <Device/DeviceTable.h>
#include <Framework/Logger.h>
#include <Framework/Tester.h>
#include <Framework/ByteOrder.h>
#include "SelectedDeviceDrivers.h"

/**
   Test the operation of the MCP9808 Temperature Sensor.
*/

#define BUF_SIZE 256
byte buf[BUF_SIZE];

DeviceTable *dt;
Tester *tst;
Logger *logger;

// --------------------------------------------------------

void setup() {

  // Countdown before running to give time to open the monitor window.

  Serial.begin(115200);

  delay(3000);
  for (int idx = 0; idx < 5; idx++) {
    Serial.print(5 - idx);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();

  // Load the device driver table

  dt = new DeviceTable(selectedDevices);

  //  Create the objects needed for the testing framework.

  tst = new Tester();
  logger = new Logger("DriverTestMCP9808");
  logger->setCurrentLogLevel(LogLevel::DEBUG);

  // --------------------------------------------------------

  // Begin testing

  tst->beforeGroup("MCP9808");

  // --------------------------------------------------------

  tst->beforeTest("MCP9808OpenClose");
  int status = dt->open("TempSensor:0", DDO_FORCE_OPEN);
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
    tst->assertEquals("read() count assertEquals:", 2, status);
    logger->debug("Read 2 bytes:", buf[0], buf[1]);
    status = dt->close(handle);
    tst->assertTrue("Close error.", (status >= 0));
  }
  tst->afterTest();

  // --------------------------------------------------------

  tst->beforeTest("MCP9808CDRDriverVersion");
  status = dt->open("TempSensor:0");
  tst->assertTrue("Open error.", (status >= 0));

  int reg = static_cast<int>(CDR::DriverVersion);

  if (status >= 0) {
    int handle = status;
    status = dt->status(handle, reg, BUF_SIZE, buf);
    tst->assertTrue("Device status() Driver Version assertTrue:", (status >= 0));
    if (status >= 0) {
      logger->debug("Version data is n bytes. ",status);
//      char *svString = SemVer::toString(buf);
//      logger->debug("  Read n version bytes ", status,svString);
//      free(svString);
    }
    status = dt->close(handle);
    tst->assertTrue("Close error.", (status >= 0));

  }
  tst->afterTest();

  // --------------------------------------------------------

  tst->beforeTest("MCP9808StatusMethod");
  status = dt->open("TempSensor:0");
  tst->assertTrue("Open error.", (status >= 0));

  if (status >= 0) {
    int handle = status;

    for (int r = 1; r < 9; r++) {
      int count = (r == 8) ? 1 : 2;

      status = dt->status(handle, r, count, buf);
      tst->assertEquals("Device status() count assertEquals:", count, status);
      if (status < 0) continue;

      if (count == 1) {
        logger->debug("  Read 1 status byte from reg ", r, buf[0]);
      } else {
        logger->debug("  Read 2 status bytes from reg ", r, buf[0], buf[1]);
      }

      switch (r) {
        case static_cast<int>(MCP9808Register::MANUF_ID):
          logger->info("Manufacturer ID: ", from16BEToHost(buf));
          tst->assertEquals("MCP9808 MANUF_ID assertEquals", 0x54, from16BEToHost(buf));
          break;
        case static_cast<int>(MCP9808Register::DEVICE_ID):
          logger->info("Device ID: ", from16BEToHost(buf));
          tst->assertEquals("MCP9808 DEVICE_ID assertEquals", 0x400, from16BEToHost(buf));
          break;
      }
    }
    status = dt->close(handle);
    tst->assertTrue("Close error.", (status >= 0));
  }
  tst->afterTest();

  // --------------------------------------------------------

  tst->afterGroup();

}


void loop() {}


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


