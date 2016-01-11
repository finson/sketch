#ifndef DeviceRegister_h
#define DeviceRegister_h

// These are the common register codes used by the DeviceDrivers.  Registers
// specific to each device are defined by the individual DeviceDrivers.

// Common Status Registers

#define CSR_Version     (-1)
#define CSR_DeviceName  (-2)
#define CSR_UnitName    (-3)
#define CSR_UnitCount   (-4)
#define CSR_Debug       (-5)

// Common Control Registers

#define CCR_Reset       (-256)  /* Reset all state in the device driver */
#define CCR_Configure   (-257)  /* Configure a logical unit number instance */
#endif
