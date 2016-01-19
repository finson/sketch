#ifndef Port_h
#define Port_h

#include <inttypes.h>
#include "ByteOrder.h"

/**
 * This abstract class defines some basic methods available for
 * reading and writing using various board level interfaces.  It
 * is intended as a very simple wrapper around the various
 * libraries available so that the underlying libraries can
 * change and simple applications won't be affected.
 *
 * Direct use of calls to the underlying library are always available
 * too if needed for more esoteric applications.
 *
 * The LE (LittleEndian) and BE (BigEndian) suffixes refer to the order in
 * which the bytes of a multi-byte quantity go to or from the attached device.
 * It's very possible, for example, to have devices that expect BE byte order
 * attached to LE hosts, and so these methods allow the user to easily match
 * the device's needs.  The byte order of the host is determined at compile
 * time and the code is adjusted to select the order in which the bytes are
 * addressed locally.
 */
class Port {

public:

  Port();
  ~Port();

  virtual void enable();
  virtual void disable();
  virtual bool isEnabled();

  virtual void writeUInt8(int addr, uint8_t reg, uint8_t value) = 0;
  virtual void writeUInt16LE(int addr, uint8_t reg, uint16_t value) = 0;
  virtual void writeUInt16BE(int addr, uint8_t reg, uint16_t value) = 0;
  virtual void writeUInt32LE(int addr, uint8_t reg, uint32_t value) = 0;
  virtual void writeUInt32BE(int addr, uint8_t reg, uint32_t value) = 0;

  virtual uint8_t readUInt8(int addr, uint8_t reg) = 0;
  virtual uint16_t readUInt16LE(int addr, uint8_t reg) = 0;
  virtual uint16_t readUInt16BE(int addr, uint8_t reg) = 0;
  virtual uint32_t readUInt32LE(int addr, uint8_t reg) = 0;
  virtual uint32_t readUInt32BE(int addr, uint8_t reg) = 0;
};

#endif
