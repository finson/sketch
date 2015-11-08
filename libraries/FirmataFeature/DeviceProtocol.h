#ifndef DeviceProtocol_h
#define DeviceProtocol_h

#include <FirmataFeature.h>

// pin modes
//
//#define INPUT                 0x00 // defined in wiring.h
//#define OUTPUT                0x01 // defined in wiring.h
//#define ANALOG                  0x02 // analog pin in analogInput mode
//#define PWM                     0x03 // digital pin in PWM output mode
#define SERVO                   0x04 // digital pin in Servo output mode
#define SHIFT                   0x05 // shiftIn/shiftOut mode
#define I2C                     0x06 // pin included in I2C setup
#define ONEWIRE                 0x07 // pin configured for 1-wire
#define STEPPER                 0x08 // pin configured for stepper motor
#define ENCODER                 0x09 // pin configured for encoders
// #define IGNORE                  0x7F // pin configured to be ignored by digitalWrite and capabilityResponse
// #define TOTAL_PIN_MODES         11

// Sysex commands that have an associated pin mode and implement or extend the mode

#define ENCODER_DATA            0x61 // reply with encoders current positions (FirmataFeature ??)
#define SERVO_CONFIG            0x70 // set max angle, minPulse, maxPulse, freq (FirmataFeature ServoFirmata)
#define STEPPER_DATA            0x72 // control a stepper motor (FirmataFeature StepperFirmata)
#define ONEWIRE_DATA            0x73 // send an OneWire read/write/reset/select/skip/search request (FirmataFeature OneWireFirmata)
#define SHIFT_DATA              0x75 // a bitstream to/from a shift register (FirmataFeature ??)
#define I2C_REQUEST             0x76 // send an I2C read/write request (FirmataFeature I2CFirmata)
#define I2C_REPLY               0x77 // a reply to an I2C read request (FirmataFeature I2CFirmata)
#define I2C_CONFIG              0x78 // config I2C settings such as delay times and power pins (FirmataFeature I2CFirmata)

class DeviceProtocol: public FirmataFeature
{
  public:
};

#endif
