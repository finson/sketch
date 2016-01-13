#ifndef DeviceRegister_h
#define DeviceRegister_h

// These are the Common Device Register codes used by the DeviceDrivers in
// their status() and control() methods.  Register names specific to a
// particular device type are defined by the individual DeviceDrivers.

#define CDR_DriverVersion  (-1)  /* Get driver name and version */
#define CDR_LibraryVersion (-2)  /* Get library name and version */
#define CDR_Reset          (-3)  /* Reset all state in the device driver */
#define CDR_Configure      (-4)  /* Configure a logical unit number instance */
#define CDR_Debug          (-256)
#endif
