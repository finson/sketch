#include "MCP9808Driver.h"
#include <Wire.h>

DEFINE_SEMVER(MCP9808Driver, "MCP9808Driver", 0, 1, 0)

//---------------------------------------------------------------------------

MCP9808Driver::MCP9808Driver(const char *dName, int lunCount) :
  DeviceDriver(dName, lunCount) {}

//---------------------------------------------------------------------------

int MCP9808Driver::open(const char *name, int flags) {
  int lun = DeviceDriver::open(name, flags);
  if (lun < 0) return lun;
  logicalUnits[lun] = new MCP9808LUI();
  return lun;
}

// shouldn't the driver be the one that knows what the I2C address of each logical unit is?

int MCP9808Driver::control(int handle, int reg, int count, byte *buf) {
  MCP9808LUI *currentUnit = static_cast<MCP9808LUI *>(logicalUnits[handle & 0x7F]);
  if (currentUnit == 0) {
    return ENOTCONN;
  }

  int theI2CAddress = currentDevice->getI2CAddress();
  if (theI2CAddress < I2C_MIN_7BIT_ADDRESS || theI2CAddress > I2C_MAX_7BIT_ADDRESS) {
    return EBADFD;
  }

  switch (reg) {

  case static_cast<int>(CDR_Configure):
    return controlCDR_Configure(handle, reg, count, buf);

  case static_cast<int>(MCP9808Register::CONFIG):
    break;

  case static_cast<int>(MCP9808Register::UPPER_TEMP):
  case static_cast<int>(MCP9808Register::LOWER_TEMP):
  case static_cast<int>(MCP9808Register::CRIT_TEMP):
    if (count == 2) {
      I2CMode.write16(address, reg, (((buf[0] & 0xFF) << 8) | (buf[1] & 0xFF)));
      return count;
    } else {
      return EMSGSIZE;
    }
    break;

  case static_cast<int>(MCP9808Register::RESOLUTION):
    if (count == 1) {
      I2CMode.write8(address, reg, buf[0]);
      return count;
    } else {
      return EMSGSIZE;
    }
    break;

  default:
    return ENOTSUP;
  }
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
  case static_cast<int>(CDR::Debug):
    return statusCDR_Debug(handle, reg, count, buf);
  case static_cast<int>(MCP9808Register::MANUF_ID):
    break;
  case static_cast<int>(MCP9808Register::DEVICE_ID):
    break;
  case static_cast<int>(MCP9808Register::AMBIENT_TEMP):
    break;
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

//---------------------------------------------------------------------------

int MCP9808Driver::statusCDR_Debug(int handle, int reg, int count, byte *buf) {
  return ENOTSUP;
}


int MCP9808Driver::controlCDR_Configure(int handle, int reg, int count, byte *buf) {
//----------
// for (int idx = 0; idx < logicalUnitCount; idx++) {
//   MCP9808LUI *currentUnit = static_cast<MCP9808LUI *>(logicalUnits[idx]);
//   if (currentUnit == 0) continue;
//   if (theI2CAddress == currentUnit->getI2CAddress()) {
//     theI2CAddress = -1;
//     break;
//   }
// }
//----------

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
