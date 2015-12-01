#include "MCP9808Driver.h"

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

MCP9808Driver::MCP9808Driver(char *dNameRoot, int baseAddr, int addrCount) :
  DeviceDriver(dNameRoot) {
  char buf[32];
  actualMinorHandleCount = min(MAX_MCP9808_MINOR_HANDLE_COUNT, addrCount);
  for (int idx = 0; idx < actualMinorHandleCount; idx++) {
    sprintf(buf, "%s:%1d", dNameRoot, idx);
    devices[idx].setDeviceName(buf);
    devices[idx].setDeviceAddress(baseAddr + idx);
    devices[idx].setOpen(false);
  }
}

MCP9808Driver::MCP9808Driver(char *dNameRoot, int deviceAddresses[], int addrCount) :
  DeviceDriver(dNameRoot) {
  char buf[32];
  actualMinorHandleCount = min(MAX_MCP9808_MINOR_HANDLE_COUNT, addrCount);
  for (int idx = 0; idx < actualMinorHandleCount; idx++) {
    sprintf(buf, "%s:%1d", dNameRoot, idx);
    devices[idx].setDeviceName(buf);
    devices[idx].setDeviceAddress(deviceAddresses[idx]);
    devices[idx].setOpen(false);
  }
}

//---------------------------------------------------------------------------

int MCP9808Driver::open(char *name, int flags) {
  byte i2cRxData[32];
  int handle;
  for (handle = 0; handle < actualMinorHandleCount; handle++) {
    if (strcmp(devices[handle].getDeviceName(), name) == 0) {
      break;
    }
  }
  if (handle == actualMinorHandleCount) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.NO_SUCH_DEVICE);
    return -1;
  }

  I2CDeviceInfo currentDevice = devices[handle];
  if (currentDevice.isOpen()) {
    // throw new DeviceException(
    //         "Could not open '" + name + "', " + DeviceStatus.DEVICE_ALREADY_OPEN);
    return -1;
  }


  int mfgr = -1;
  int devid = -1;

//         request = new I2CRequestRead(currentDevice.getDeviceAddress(), MCP9808Register.MANUF_ID.ordinal(), 2);
//         logger.trace("Request mfgr: {}", request.toString());
//         qOut.writeMessage(request);

  int theAddress = currentDevice.getDeviceAddress();
  int theRegister = static_cast<int>(MCP9808Register::MANUF_ID);
  int numBytes = 2;

//        readAndReportData(theAddress, theRegister, numBytes);

  Wire.beginTransmission(theAddress);
#if ARDUINO >= 100
  Wire.write((byte)theRegister);
#else
  Wire.send((byte)theRegister);
#endif
  Wire.endTransmission();

  // do not set a value of 0
  // if (i2cReadDelayTime > 0) {
  //   // delay is necessary for some devices such as WiiNunchuck
  //   delayMicroseconds(i2cReadDelayTime);
  // }

  Wire.requestFrom(theAddress, numBytes);  // all bytes are returned in requestFrom

  // check to be sure correct number of bytes were returned by slave
  if (numBytes < Wire.available()) {
//    Firmata.sendString("I2C: Too many bytes received");
    return -1;
  } else if (numBytes > Wire.available()) {
    // Firmata.sendString("I2C: Too few bytes received");
    return -1;
  }

  i2cRxData[0] = theAddress;
  i2cRxData[1] = theRegister;

  for (int i = 0; i < numBytes && Wire.available(); i++) {
#if ARDUINO >= 100
    i2cRxData[2 + i] = Wire.read();
#else
    i2cRxData[2 + i] = Wire.receive();
#endif
  }

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
  return -1;
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
