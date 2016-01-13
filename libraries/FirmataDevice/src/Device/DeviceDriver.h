#ifndef DeviceDriver_h
#define DeviceDriver_h

#include <arduino.h>
#include <avr/pgmspace.h>

#include "LogicalUnitInfo.h"
#include "DeviceError.h"
#include "DeviceRegister.h"

#define MAX_DEVICE_NAME_LENGTH (MAX_LU_NAME_LENGTH-3)

#define  DD_OPEN    0x00
#define  DD_STATUS  0x01
#define  DD_CONTROL 0x02
#define  DD_READ    0x03
#define  DD_WRITE   0x04
#define  DD_CLOSE   0x05

#define DDO_FORCE_OPEN 0x01

#define DDC_INIT 0

#define getInt8LE(addr) ((*(addr))&0xFF)
#define getInt16LE(addr) ((((*((addr)+1))&0xFF)<<8)  |  ((*((addr)+0))&0xFF))
#define getInt32LE(addr) ((((*((addr)+3))&0xFF)<<24) | (((*((addr)+2))&0xFF)<<16) | (((*((addr)+1))&0xFF)<<8) | ((*((addr)+0))&0xFF))

// class DeviceFeature;
// extern DeviceFeature deviceManager;

class DeviceDriver {

public:
    DeviceDriver(const char *nameRoot);

    virtual int open(const char *name, int flags) = 0;
    virtual int status(int handle, int reg, int count, byte *buf) = 0;
    virtual int control(int handle, int reg, int count, byte *buf) = 0;
    virtual int read(int handle, int count, byte *buf) = 0;
    virtual int write(int handle, int count, byte *buf) = 0;
    virtual int close(int handle) = 0;


    /**
     * Called repeatedly by the application program at the expiration of a
     * microsecond based interval to perform real time updates of position and
     * control.
     * @param deltaMicros Length, in microseconds, of the interval since the last call to this
     * method.
     * @return status code
     */
    virtual int microsecondUpdate(unsigned long deltaMicros);

    /**
     * Called repeatedly by the application program at the expiration of a
     * millisecond based interval to provide an opportunity for reporting and
     * other lower frequency tasks.
     * @param deltaMillis Length, in milliseconds, of the interval since the last call to this method.
     * @return status code
     */
    virtual int millisecondReport(unsigned long deltaMillis);

protected:
    char deviceName[MAX_DEVICE_NAME_LENGTH + 1];
    int logicalUnitCount;
};

#endif
