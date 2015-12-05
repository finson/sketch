
// Firmata Features

#include <DigitalInputFeature.h>
DigitalInputFeature digitalInput;

#include <DigitalOutputFeature.h>
DigitalOutputFeature digitalOutput;

#include <AnalogInputFeature.h>
AnalogInputFeature analogInput;

#include <AnalogOutputFeature.h>
AnalogOutputFeature analogOutput;

#include <DeviceFeature.h>
DeviceFeature deviceManager;

// Firmata Features (Communications)

#include <I2C/I2CFeature.h>
I2CFeature pI2C;

// FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,0};
FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,&deviceManager,0};

// Device Drivers

#include <MCP9808/MCP9808Driver.h>
MCP9808Driver ddMCP9808("MCP9808",0x18,1);

// DeviceDriver *selectedDevices[] = {0};
DeviceDriver *selectedDevices[] = {&ddMCP9808,0};
