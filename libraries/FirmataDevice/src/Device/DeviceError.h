#ifndef DeviceError_h
#define DeviceError_h

#define ESUCCESS     0  /* Success, no error */
#define ENXIO        6  /* No such device or address */
#define ENODEV      19  /* No such device */
#define ENOSYS      38  /* Function not implemented */
#define EMSGSIZE    90  /* Message too long */
#define EADDRINUSE  98  /* Address already in use */
#define EISCONN     106 /* Transport endpoint is already connected */
#define ENOTCONN    107 /* Transport endpoint is not connected */
#define ECONNREFUSED    111 /* Connection refused */
#define EREMOTEIO   121 /* Remote I/O error */

#endif
