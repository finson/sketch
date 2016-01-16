#include "MCP9808Driver.h"
#include <Wire.h>

DEFINE_SEMVER(MCP9808Driver, "MCP9808Driver", 0, 1, 0)

//---------------------------------------------------------------------------

MCP9808Driver::MCP9808Driver(const char *dName, int count) :
  DeviceDriver(dName, count) {}

//---------------------------------------------------------------------------

int MCP9808Driver::open(const char *name, int flags) {
  int lun = DeviceDriver::open(name, flags);
  if (lun < 0) return lun;

  logicalUnits[lun] = new MCP9808LUI();
  return lun;
}

int MCP9808Driver::control(int handle, int reg, int count, byte *buf) {
  MCP9808LUI *currentUnit = static_cast<MCP9808LUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  default:
    return ENOTSUP;
  }
}

//   if (static_cast<MCP9808Register>(reg) == MCP9808Register::RESOLUTION) {
//     if (count != 1) {
//       return EMSGSIZE;
//     }
//   } else if (count != 2) {
//     return EMSGSIZE;
//   }

//   int address = currentDevice->getDeviceAddress();
//   if (count == 1) {
//     I2CMode.write8(address, reg,buf[0]);
//   } else {
//     I2CMode.write16(address, reg, (((buf[0] & 0xFF) << 8) | (buf[1] & 0xFF)));
//   }
//   return count;
// }

int MCP9808Driver::controlCDR_Configure(int handle, int reg, int count, byte *buf) {

//   I2CMode.enableI2CPins();

//   int address = currentDevice->getDeviceAddress();
//   theRegister = static_cast<uint8_t>(MCP9808Register::MANUF_ID);
//   if (I2CMode.read16(address, theRegister) != 0x0054) {
//     I2CMode.disableI2CPins();
//     return ECONNREFUSED;
//   }

//   theRegister = static_cast<uint8_t>(MCP9808Register::DEVICE_ID);
//   if (I2CMode.read16(address, theRegister) != 0x0400) {
//     I2CMode.disableI2CPins();
//     return ECONNREFUSED;
//   }

}

/**
 * Read a status register on the device. For the MCP9808, there are nine
 * registers accessible.
 */
int MCP9808Driver::status(int handle, int reg, int count, byte *buf) {
  MCP9808LUI *currentUnit = static_cast<MCP9808LUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  switch (reg) {
  case static_cast<int>(CDR::DriverVersion):
    return DeviceDriver::buildVersionResponse(MCP9808Driver::driverSemVer, MCP9808Driver::driverName, count, buf);
  default:
    return ENOTSUP;
  }
}


//   MCP9808LUI *currentDevice = &logicalUnits[handle & 0x7F];
//   if (!currentDevice->isOpen()) {
//     return ENOTCONN;
//   }
//   if (static_cast<MCP9808Register>(reg) == MCP9808Register::RESOLUTION) {
//     if (count != 1) {
//       return EMSGSIZE;
//     }
//   } else if (count != 2) {
//     return EMSGSIZE;
//   }

//   int address = currentDevice->getDeviceAddress();
//   if (count == 1) {
//     buf[0] = I2CMode.read8(address, reg);
//   } else {
//     int v = I2CMode.read16(address, reg);
//     buf[0] = (v >> 8) & 0xFF;
//     buf[1] = v & 0xFF;
//   }
//   return count;
// }

int MCP9808Driver::read(int handle, int count, byte * buf) {
  MCP9808LUI *currentUnit = static_cast<MCP9808LUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) return ENOTCONN;

  if (count != 2) {
    return EMSGSIZE;
  }

  // int address = currentDevice->getDeviceAddress();
  // int reg = static_cast<uint8_t>(MCP9808Register::AMBIENT_TEMP);
  // int v = I2CMode.read16(address, reg);
  // buf[0] = (v >> 8) & 0xFF;
  // buf[1] = v & 0xFF;

  // return count;

  return ENOTSUP;
}

int MCP9808Driver::write(int handle, int count, byte * buf) {
  return ENOSYS;
}

int MCP9808Driver::close(int handle) {
  return DeviceDriver::close(handle);
}
