
// Features

#include <GPIO/DigitalInputFeature.h>
DigitalInputFeature digitalInput;

#include <GPIO/DigitalOutputFeature.h>
DigitalOutputFeature digitalOutput;

#include <GPIO/AnalogInputFeature.h>
AnalogInputFeature analogInput;

#include <GPIO/AnalogOutputFeature.h>
AnalogOutputFeature analogOutput;

#include <Device/DeviceFeature.h>
DeviceFeature deviceManager("DeviceFeature",1);

#include <I2C/I2CFeature.h>
I2CFeature pI2C;

//FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&deviceManager,0};
FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,&deviceManager,0};

// Device Drivers

//#include <Hello/HelloDriver.h>
//HelloDriver ddHello("Hello",2);

 #include <MCP9808/MCP9808Driver.h>
 MCP9808Driver ddMCP9808("MCP9808",0x18,1);

DeviceDriver *selectedDevices[] = {&ddMCP9808,0};
// DeviceDriver *selectedDevices[] = {&ddHello,&ddMCP9808,0};
