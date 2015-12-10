#include "MCP9808Driver.h"
#include <I2CPort.h>

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

MCP9808Driver::MCP9808Driver(char *dName, int baseAddr, int addrCount) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  minorDeviceCount = min(MAX_MCP9808_LU_COUNT, addrCount);
  for (int idx = 0; idx < minorDeviceCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    minorDevices[idx].setLogicalUnitName(buf);
    minorDevices[idx].setDeviceAddress(baseAddr + idx);
    minorDevices[idx].setOpen(false);
  }
}

MCP9808Driver::MCP9808Driver(char *dName, int deviceAddresses[], int addrCount) :
  DeviceDriver(dName) {
  char buf[MAX_LU_NAME_LENGTH+1];
  minorDeviceCount = min(MAX_MCP9808_LU_COUNT, addrCount);
  for (int idx = 0; idx < minorDeviceCount; idx++) {
    snprintf(buf, MAX_LU_NAME_LENGTH+1, "%s:%1d", dName, idx);
    minorDevices[idx].setLogicalUnitName(buf);
    minorDevices[idx].setDeviceAddress(deviceAddresses[idx]);
    minorDevices[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int MCP9808Driver::open(char *name, int flags) {
  uint8_t theRegister;

  int handle;
  for (handle = 0; handle < minorDeviceCount; handle++) {
    if (strcmp(minorDevices[handle].getLogicalUnitName(), name) == 0) {
      break;
    }
  }
  if (handle == minorDeviceCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  MCP9808LUI currentDevice = minorDevices[handle];
  if (currentDevice.isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }

  I2CPort.enableI2CPins();

  int address = currentDevice.getDeviceAddress();
  theRegister = static_cast<uint8_t>(MCP9808Register::MANUF_ID);
  if (I2CPort.read16(address, theRegister) != 0x0054) {
    I2CPort.disableI2CPins();
    return -1;
  }

  theRegister = static_cast<uint8_t>(MCP9808Register::DEVICE_ID);
  if (I2CPort.read16(address, theRegister) != 0x0400) {
    I2CPort.disableI2CPins();
    return -1;
  }

  currentDevice.setOpen(true);
  return handle;
}

int MCP9808Driver::status(int handle, int reg, int count, byte *buf) {
  return -1;
}

int MCP9808Driver::control(int handle, int reg, int count, byte *buf) {
  return -1;
}

int MCP9808Driver::read(int handle, int count, byte *buf) {
  return -1;
}
int MCP9808Driver::write(int handle, int count, byte *buf) {
  return -1;
}

int MCP9808Driver::close(int handle) {
  MCP9808LUI currentDevice = minorDevices[handle];
  if (currentDevice.isOpen()) {
    currentDevice.setOpen(false);
    I2CPort.disableI2CPins();
    return 0;
  } else {
    return -1;
  }
}

//     @Override
//     public int open(String name, int flags) throws IOException {
//         I2CDeviceInfo currentDevice;
//         FirmataMessage request;
//         FirmataMessage response;

//         int handle = super.open(name);
//         currentDevice = (I2CDeviceInfo) devices.get(handle);

//         int mfgr = -1;
//         int devid = -1;

//         request = new I2CRequestRead(currentDevice.getDeviceAddress(), MCP9808Register.MANUF_ID.ordinal(), 2);
//         logger.trace("Request mfgr: {}", request.toString());
//         qOut.writeMessage(request);

//         do {
//             response = qIn.readMessage();
//         } while (!I2CReply.class.isAssignableFrom(response.getClass()));

//         I2CReply msg = (I2CReply) response;
//         if (msg.getRegister() == MCP9808Register.MANUF_ID.ordinal()) {
//             mfgr = (msg.getData(0) << 8) | (msg.getData(1));
//         }

//         request = new I2CRequestRead(currentDevice.getDeviceAddress(), MCP9808Register.DEVICE_ID.ordinal(), 2);
//         qOut.writeMessage(request);

//         do {
//             response = qIn.readMessage();
//         } while (!I2CReply.class.isAssignableFrom(response.getClass()));

//         msg = (I2CReply) response;
//         if (msg.getRegister() == MCP9808Register.DEVICE_ID.ordinal()) {
//             devid = msg.getData(0);
//         }

//         logger.debug("Device manufacturer: expected: 0x0054, actual:{}, Device ID: expected: 0x04, actual: {}",
//                 Integer.toHexString(mfgr), Integer.toHexString(devid));

//         if ((mfgr != 0x0054) || (devid != 0x04)) {
//             currentDevice.setOpen(false);
//             throw new DeviceException("Could not open '" + name + "', " + DeviceStatus.UNKNOWN_DEVICE_TYPE);
//         } else {
//             currentDevice.setOpen(true);
//         }
//         logger.info("Device {} opened successfully.  Handle is {}.", name, handle);
//         return handle;
//     }

//     /**
//      * Read a status register on the device. For the MCP9808, there are nine registers accessible.
//      *
//      * @throws IOException
//      *
//      * @see ws.tuxi.lib.firmata.device.DeviceDriver#status(int, byte[], int)
//      */
//     @Override
//     public int status(int handle, int reg, int count, byte[] buf) throws IOException {
//         FirmataMessage query;
//         FirmataMessage response;

//         I2CDeviceInfo currentDevice = (I2CDeviceInfo) devices.get(handle);

//         if (!currentDevice.isOpen()) {
//             throw new DeviceException("Device must be open to read status: " + currentDevice.getDeviceName());
//         }
//         if (buf.length < count) {
//             throw new DeviceException(
//                     "Buffer length '" + buf.length + "' too small for requested count '" + count + "'.");
//         }

//         if (reg != MCP9808Register.RESOLUTION.ordinal() && count != 2) {
//             throw new ProtocolException("MCP9808 register " + reg + " status message is 2 bytes long, not " + count
//                     + " as requested in this call.");
//         }

//         if (reg == MCP9808Register.RESOLUTION.ordinal() && count != 1) {
//             throw new ProtocolException(
//                     "MCP9808 resolution status message is 1 byte long, not " + count + " as requested in this call.");
//         }

//         logger.trace("Status read. Handle: {}, reg: {}, count: {}, buf.length: {}", handle, reg, count, buf.length);

//         query = new I2CRequestRead(currentDevice.getDeviceAddress(), reg, count);
//         qOut.writeMessage(query);

//         do {
//             response = qIn.readMessage();
//         } while (!I2CReply.class.isAssignableFrom(response.getClass()));

//         I2CReply msg = (I2CReply) response;
//         if (msg.getRegister() == reg) {
//             if (count == 1) {
//                 buf[0] = (byte) msg.getData(0);
//                 logger.trace("Read 1 status byte: 0x{}", Integer.toHexString(msg.getData(0)));
//             } else {
//                 buf[0] = (byte) msg.getData(0);
//                 buf[1] = (byte) msg.getData(1);
//                 logger.trace("Read 2 status bytes: 0x{}, 0x{}", Integer.toHexString(msg.getData(0)),
//                         Integer.toHexString(msg.getData(1)));
//             }
//         } else {
//             throw new ProtocolException(
//                     "Register in request (" + reg + ") does not match register in reply (" + msg.getRegister() + ").");
//         }
//         return count;
//     }

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
