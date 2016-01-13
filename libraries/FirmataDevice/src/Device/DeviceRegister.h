#ifndef DeviceRegister_h
#define DeviceRegister_h

// These are the Common Device Register codes used by the DeviceDrivers in
// their status() and control() methods.  Register names specific to a
// particular device type are defined by the individual DeviceDrivers.

#define CDR_DriverVersion (-1)
#define CDR_DriverName    (-2)
#define CDR_UnitName      (-3)
#define CDR_UnitCount     (-4)
#define CDR_Reset         (-5)  /* Reset all state in the device driver */
#define CDR_Configure     (-6)  /* Configure a logical unit number instance */
#define CDR_Debug         (-256)
#endif
