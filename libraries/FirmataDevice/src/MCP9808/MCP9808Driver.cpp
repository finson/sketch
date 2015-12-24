#include "MCP9808Driver.h"
#include <Mode/I2CMode.h>

enum class MCP9808Register {
  RESERVED,
  CONFIG,
  UPPER_TEMP,
  LOWER_TEMP,
  CRIT_TEMP,
  AMBIENT_TEMP,
  MANUF_ID,
  DEVICE_ID,
  RESOLUTION
};

//---------------------------------------------------------------------------

MCP9808Driver::MCP9808Driver(const char *dName, int baseAddr, int addrCount) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  logicalUnitCount = min(MAX_MCP9808_LU_COUNT, addrCount);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setDeviceAddress(baseAddr + idx);
    logicalUnits[idx].setOpen(false);
  }
}

MCP9808Driver::MCP9808Driver(const char *dName, int deviceAddresses[], int addrCount) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  logicalUnitCount = min(MAX_MCP9808_LU_COUNT, addrCount);
  for (int idx = 0; idx < logicalUnitCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    logicalUnits[idx].setLogicalUnitName(buf);
    logicalUnits[idx].setDeviceAddress(deviceAddresses[idx]);
    logicalUnits[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int MCP9808Driver::open(int *handle, const char *name, int flags) {
  uint8_t theRegister;
  int lun;
  for (lun = 0; lun < logicalUnitCount; lun++) {
    if (strcmp(logicalUnits[lun].getLogicalUnitName(), name) == 0) {
      break;
    }
  }
  if (lun == logicalUnitCount) {
    return ENXIO;
  }

  MCP9808LUI *currentDevice = &logicalUnits[lun];

  if ((flags & DDO_FORCE_OPEN) != 0) {
    currentDevice->setOpen(false);
  }

  if (currentDevice->isOpen()) {
    return EADDRINUSE;
  }

  I2CMode.enableI2CPins();

  int address = currentDevice->getDeviceAddress();
  theRegister = static_cast<uint8_t>(MCP9808Register::MANUF_ID);
  if (I2CMode.read16(address, theRegister) != 0x0054) {
    I2CMode.disableI2CPins();
    return ECONNREFUSED;
  }

  theRegister = static_cast<uint8_t>(MCP9808Register::DEVICE_ID);
  if (I2CMode.read16(address, theRegister) != 0x0400) {
    I2CMode.disableI2CPins();
    return ECONNREFUSED;
  }

  currentDevice->setOpen(true);
  *handle = lun;
  return ESUCCESS;
}
/**
 * Read a status register on the device. For the MCP9808, there are nine
 * registers accessible.
 */
int MCP9808Driver::status(int handle, int reg, int count, byte *buf) {
  MCP9808LUI *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }
  if (static_cast<MCP9808Register>(reg) == MCP9808Register::RESOLUTION) {
    if (count != 1) {
      return EMSGSIZE;
    }
  } else if (count != 2) {
    return EMSGSIZE;
  }

  int address = currentDevice->getDeviceAddress();
  if (count == 1) {
    buf[0] = I2CMode.read8(address, reg);
  } else {
    int v = I2CMode.read16(address, reg);
    buf[0] = (v >> 8) & 0xFF;
    buf[1] = v & 0xFF;
  }
  return ESUCCESS;
}

int MCP9808Driver::control(int handle, int reg, int count, byte *buf) {
  MCP9808LUI *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }
  if (static_cast<MCP9808Register>(reg) == MCP9808Register::RESOLUTION) {
    if (count != 1) {
      return EMSGSIZE;
    }
  } else if (count != 2) {
    return EMSGSIZE;
  }

  int address = currentDevice->getDeviceAddress();
  if (count == 1) {
    I2CMode.write8(address, reg,buf[0]);
  } else {
    I2CMode.write16(address, reg, (((buf[0] & 0xFF) << 8) | (buf[1] & 0xFF)));
  }
  return ESUCCESS;
}

int MCP9808Driver::read(int handle, int count, byte *buf) {
  MCP9808LUI *currentDevice = &logicalUnits[handle & 0x7F];
  if (!currentDevice->isOpen()) {
    return ENOTCONN;
  }

  if (count != 2) {
    return EMSGSIZE;
  }

  int address = currentDevice->getDeviceAddress();
  int reg = static_cast<uint8_t>(MCP9808Register::AMBIENT_TEMP);
  int v = I2CMode.read16(address, reg);
  buf[0] = (v >> 8) & 0xFF;
  buf[1] = v & 0xFF;

  return ESUCCESS;
}

int MCP9808Driver::write(int handle, int count, byte *buf) {
  return ENOSYS;
}

int MCP9808Driver::close(int lun) {
  MCP9808LUI *currentDevice = &logicalUnits[lun & 0x7F];
  if (currentDevice->isOpen()) {
    currentDevice->setOpen(false);
    I2CMode.disableI2CPins();
  }
  return ESUCCESS;
}

//     /**
//      * @throws IOException
//      * @see ws.tuxi.lib.firmata.device.DeviceDriver#control(int, int, int, byte[])
//      */
//     @Override
//     public int control(int handle, int reg, int count, byte[] buf) throws IOException {
//         I2CDeviceInfo currentDevice;
//         FirmataMessage request;

//         currentDevice = (I2CDeviceInfo) devices.get(handle);

//         if (!currentDevice.isOpen()) {
//             throw new DeviceException("Device must be open to control it: " + currentDevice.getDeviceName());
//         }
//         if (buf.length < count) {
//             throw new DeviceException(
//                     "Buffer length '" + buf.length + "' too small for requested count '" + count + "'.");
//         }

//         if (reg != MCP9808Register.RESOLUTION.ordinal() && count != 2) {
//             throw new ProtocolException("MCP9808 register " + reg + " control message is 2 bytes long, not " + count
//                     + " as requested in this call.");
//         }

//         if (reg == MCP9808Register.RESOLUTION.ordinal() && count != 1) {
//             throw new ProtocolException(
//                     "MCP9808 resolution control message is 1 byte long, not " + count + " as requested in this call.");
//         }

//         // Note that, at least for this device, the internal register address is sent as the first
//         // byte of the data being written.

//         // Also, according to the data sheet, "This device does not support sequential register
//         // read/write. Each register needs to be addressed using the register pointer." I think
//         // that means that each write needs to start with the 8-bit register number, followed by
//         // two data bytes (or one for the resolution register).

//         // So before sending the user's data, we prepend it with the requested register number.

//         int[] dataToWrite = new int[buf.length + 1];
//         dataToWrite[0] = reg & 0xFF;
//         int dataIndex = 1;
//         for (byte b : buf) {
//             dataToWrite[dataIndex++] = Byte.toUnsignedInt(b);
//         }

//         request = new I2CRequestWrite(currentDevice.getDeviceAddress(), dataToWrite.length, dataToWrite);
//         logger.trace("Control request: {}", request.toString());
//         qOut.writeMessage(request);

//         return count;
//     }

//     /**
//      * The read method on this device always reads the two ambient temperature bytes.
//      *
//      * @throws IOException
//      *
//      * @see ws.tuxi.lib.firmata.device.DeviceDriver#read(int)
//      */
//     @Override
//     public int read(int handle, int count, byte[] buf) throws IOException {
//         FirmataMessage query;
//         FirmataMessage response;
//         I2CDeviceInfo currentDevice = (I2CDeviceInfo) devices.get(handle);

//         if (!currentDevice.isOpen()) {
//             throw new DeviceException("Device must be open to control it: " + currentDevice.getDeviceName());
//         }

//         if (count != 2) {
//             throw new DeviceException(
//                     "MCP9808 read result is always 2 bytes long, not " + count + " as requested in this call.");
//         }
//         if (buf.length < count) {
//             throw new DeviceException(
//                     "Buffer length '" + buf.length + "' too small for requested count '" + count + "'.");
//         }

//         query = new I2CRequestRead(currentDevice.getDeviceAddress(), MCP9808Register.AMBIENT_TEMP.ordinal(), 2);
//         qOut.writeMessage(query);

//         do {
//             response = qIn.readMessage();
//         } while (!I2CReply.class.isAssignableFrom(response.getClass()));

//         I2CReply msg = (I2CReply) response;
//         if (msg.getRegister() != MCP9808Register.AMBIENT_TEMP.ordinal()) {
//             throw new ProtocolException("MCP9808 response register '" + msg.getRegister()
//                     + "' does not match request register '" + MCP9808Register.AMBIENT_TEMP.ordinal() + "'.");
//         }
//         buf[0] = (byte) msg.getData(0);
//         buf[1] = (byte) msg.getData(1);
//         logger.debug("Read {} bytes: {}, {}", 2, Integer.toHexString(Byte.toUnsignedInt(buf[0])),
//                 Integer.toHexString(Byte.toUnsignedInt(buf[1])));
//         return 2;
//     }

//     /**
//      * @see ws.tuxi.lib.firmata.device.DeviceDriver#write(int, int, byte[])
//      */
//     @Override
//     public int write(int handle, int count, byte[] buf) {
//         throw new UnsupportedOperationException("Device '" + this.toString(handle) + "' does not support write().");
//     }

//     @Override
//     public int close(int handle) throws IOException {
//         String name = devices.get(handle).getDeviceName();
//         if (devices.get(handle).isOpen()) {
//             devices.get(handle).setOpen(false);
//         } else {
//             throw new DeviceException("Could not close '" + name + "', " + DeviceStatus.DEVICE_ALREADY_CLOSED);
//         }
//         logger.info("Device {} closed successfully.  Handle was {}.", name, handle);
//         return 0;
//     }
// }
