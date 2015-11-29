
// Device Communication Channels

#include <I2CChannel.h>
I2CChannel pI2C;

// Firmata Features

#include <DigitalInputFeature.h>
DigitalInputFeature digitalInput;

#include <DigitalOutputFeature.h>
DigitalOutputFeature digitalOutput;

#include <AnalogInputFeature.h>
AnalogInputFeature analogInput;

#include <AnalogOutputFeature.h>
AnalogOutputFeature analogOutput;

#include <device/DeviceManager.h>
DeviceManager deviceManager;

FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,&deviceManager,0};

// Device Drivers

#include <device/MCP9808Driver.h>
MCP9808Driver ddMCP9808("TD",0x18,1);

DeviceDriver *selectedDevices[] = {&ddMCP9808,0};
