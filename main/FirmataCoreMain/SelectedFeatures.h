
// Firmata Features

#include <GPIO/DigitalInputFirmata.h>
DigitalInputFirmata digitalInput;

#include <GPIO/DigitalOutputFirmata.h>
DigitalOutputFirmata digitalOutput;

#include <GPIO/AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <GPIO/AnalogOutputFirmata.h>
AnalogOutputFirmata analogOutput;

#include <I2C/I2CFirmata.h>
I2CFirmata ffI2C;

#include <Stepper/StepperFirmata.h>
StepperFirmata ffStepper;

//#include <Device/DeviceFirmata.h>
//DeviceFirmata ffDevice;

FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&ffStepper,&ffI2C,0};
//FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&ffStepper,&ffI2C,&ffDevice,0};

// Device Drivers

// #include <Hello/HelloDriver.h>
// HelloDriver ddHello("Hello",2);

// #include <MCP9808/MCP9808Driver.h>
// MCP9808Driver ddMCP9808("MCP9808",0x18,1);

// DeviceDriver *selectedDevices[] = {0};
// DeviceDriver *selectedDevices[] = {&ddHello,0};
// DeviceDriver *selectedDevices[] = {&ddMCP9808,0};
// DeviceDriver *selectedDevices[] = {&ddHello,&ddMCP9808,0};
