#ifndef DeviceDriver_h
#define DeviceDriver_h

#include <arduino.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "LogicalUnitInfo.h"
#include "DeviceError.h"

// Put call to DECLARE_SEMVER in the declaration file xx.h in the private
// section of the device driver class declaration.  For example:
//
// class TrialDriver {
// public:
//   int yada();
// private:
//   DECLARE_SEMVER
// };

#define DECLARE_SEMVER static const PROGMEM char driverName[]; static const PROGMEM byte driverSemVer[];

// Put call to DEFINE_SEMVER in the definition file xx.cpp. For example:
//
// DEFINE_SEMVER(TrialDriver,"TrialDriver",1,0,3)

#define DEFINE_SEMVER(theScope, theName, major, minor, patch) \
const PROGMEM char theScope::driverName[] = {theName}; \
const PROGMEM byte theScope::driverSemVer[] = {3, major, minor, patch};

// These are the Common Device Register codes used by the DeviceDrivers in
// their status() and control() methods.  Register names specific to a
// particular device type are defined by the individual DeviceDrivers.

enum class CDR : int {
    DriverVersion   = -1,   /* Get driver name and version */
    LibraryVersion  = -2,   /* Get library name and version */
    Reset           = -3,   /* Reset all state in the device driver */
    Configure       = -4,   /* Configure a logical unit number instance */
    Debug           = -256  /* Do something helpful for debugging ... */
};

#define MAX_ROOT_NAME_LENGTH 32

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

class DeviceDriver {

public:

    DeviceDriver(const char *nameRoot, const int count);

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

    int buildVersionResponse(const byte *semver, const char *name, int count, byte *buf);

    const char *rootName;
    int logicalUnitCount;
    LogicalUnitInfo **logicalUnits;
};

#endif
