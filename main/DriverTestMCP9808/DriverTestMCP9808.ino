#include <FirmataDeviceLibrary.h>
#include <Device/DeviceTable.h>
#include <Silicon/Logger.h>
#include <Silicon/Tester.h>

#include "SelectedDeviceDrivers.h"

/**
   Test the operation of the MCP9808 Temperature Sensor.
*/

#define BUF_SIZE 256
byte buf[BUF_SIZE];
char names[BUF_SIZE];
int vReg[] = {static_cast<int>(CDR::DriverVersion), static_cast<int>(CDR::LibraryVersion)};

void setup() {
  Logger *logger = new Logger("DriverTestMCP9808");

  // Countdown before running to give time to open the monitor window.

  Serial.begin(115200);
  delay(3000);
  for (int idx = 5; idx > 0; idx--) {
    Serial.println(idx);
    delay(1000);
  }

  //  Create the DeviceTable object and use it to open the device under test.

  DeviceTable *dt = new DeviceTable(selectedDevices);
  Tester *tst = new Tester("MCP9808");

  // --------------------------------------------------------

  tst->before("MCP9808OpenClose");
  int status = dt->open("TempSensor:0", 0);
  tst->assertTrue("Open error.", (status >= 0));
  if (status >= 0) {
    int handle = status;
    status = dt->close(handle);
    tst->assertTrue("Close error.", (status >= 0));
  }
  tst->after();

  // --------------------------------------------------------

  tst->before("MCP9808ReadMethod");
  byte buf[2];
  status = dt->open( "TempSensor:0");
  tst->assertTrue("Open error.", (status >= 0));

  if (status >= 0) {
    int handle = status;
    status = dt->read(handle, 2, buf);
    tst->assertEquals("Read error:", (uint16_t)2, (uint16_t)status);

//    logger.info("Read 2 bytes from {}: 0x{} 0x{}", device.toString(handle),
//                Integer.toHexString(Byte.toUnsignedInt(buf[0])),
//                Integer.toHexString(Byte.toUnsignedInt(buf[1])));
//
    status = dt->close(handle);
    tst->assertTrue("Close error.", (status >= 0));
  }
  tst->after();

// --------------------------------------------------------

// @Test
//     public void testMCP9808StatusMethod() throws IOException, ReflectiveOperationException {
//         byte[] buf = new byte[18];
//         int status = device.open("MCP9808:0",
//                 DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
//         assertTrue("Open error.", (status>=0));
//         int handle = status;

//         for (Register r : Register.values()) {
//             int count = (r == Register.RESOLUTION) ? 1 : 2;

//             status = device.status(handle, r.bits(), count, buf);
//             assertEquals("Status error:", count, status);

//             if (count == 1) {
//                 logger.info("Read {} status byte from reg {}: 0x{}", count, r.toString(),
//                         Integer.toHexString(Byte.toUnsignedInt(buf[0])));
//             } else {
//                 logger.info("Read {} status bytes from reg {}: 0x{} 0x{}", count, r.toString(),
//                         Integer.toHexString(Byte.toUnsignedInt(buf[0])),
//                         Integer.toHexString(Byte.toUnsignedInt(buf[1])));
//             }

//             switch (r) {
//             case RESERVED:
//                 hopeEquals("MCP9808 Reg " + Register.RESERVED + " (MSB)", 0,
//                         Byte.toUnsignedInt(buf[0]));
//                 hopeEquals("MCP9808 Reg " + Register.RESERVED + " (LSB)", 0x1d,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case CONFIG:
//                 hopeEquals("MCP9808 Reg " + Register.CONFIG + " (MSB)", 0,
//                         Byte.toUnsignedInt(buf[0]));
//                 hopeEquals("MCP9808 Reg " + Register.CONFIG + " (LSB)", 0,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case UPPER_TEMP:
//                 assertEquals("MCP9808 Reg " + Register.UPPER_TEMP + " (MSB)", 0,
//                         Byte.toUnsignedInt(buf[0]));
//                 assertEquals("MCP9808 Reg " + Register.UPPER_TEMP + " (LSB)", 0,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case LOWER_TEMP:
//                 assertEquals("MCP9808 Reg " + Register.LOWER_TEMP + " (MSB)", 0,
//                         Byte.toUnsignedInt(buf[0]));
//                 assertEquals("MCP9808 Reg " + Register.LOWER_TEMP + " (LSB)", 0,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case CRIT_TEMP:
//                 assertEquals("MCP9808 Reg " + Register.CRIT_TEMP + " (MSB)", 0,
//                         Byte.toUnsignedInt(buf[0]));
//                 assertEquals("MCP9808 Reg " + Register.CRIT_TEMP + " (LSB)", 0,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case AMBIENT_TEMP:
//                 assertEquals("MCP9808 Reg " + Register.AMBIENT_TEMP + " (MSB)", 0xc1,
//                         Byte.toUnsignedInt(buf[0]));
//                 // assertEquals("MCP9808 Reg " + Register.AMBIENT_TEMP + " (LSB)", 0,
//                 // Byte.toUnsignedInt(buf[1])); // room
//                 break;
//             case MANUF_ID:
//                 assertEquals("MCP9808 Reg " + Register.MANUF_ID + " (MSB)", 0,
//                         Byte.toUnsignedInt(buf[0]));
//                 assertEquals("MCP9808 Reg " + Register.MANUF_ID + " (LSB)", 0x54,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case DEVICE_ID:
//                 assertEquals("MCP9808 Reg " + Register.DEVICE_ID + " (MSB)", 0x4,
//                         Byte.toUnsignedInt(buf[0]));
//                 assertEquals("MCP9808 Reg " + Register.DEVICE_ID + " (LSB)", 0,
//                         Byte.toUnsignedInt(buf[1]));
//                 break;
//             case RESOLUTION:
//                 hopeEquals("MCP9808 Reg " + Register.RESOLUTION + " (MSB)", 0x0,
//                         Byte.toUnsignedInt(buf[0]));
//                 break;
//             default:
//                 fail("Unexpected register identifier " + r);
//             }
//         }
//         status = device.close(handle);
//         assertTrue("Close error.", (status>=0));
//     }

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
//             Serial.print('-');
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
}

void loop() {
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
}
