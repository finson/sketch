//#include <FirmataFeatureLibrary.h>
#include <FirmataDeviceLibrary.h>

// Firmata Features

// #include <GPIO/DigitalInputFeature.h>
// DigitalInputFeature digitalInput;

// #include <GPIO/DigitalOutputFeature.h>
// DigitalOutputFeature digitalOutput;

// #include <GPIO/AnalogInputFeature.h>
// AnalogInputFeature analogInput;

// #include <GPIO/AnalogOutputFeature.h>
// AnalogOutputFeature analogOutput;

//#include <Device/DeviceFeature.h>
//DeviceFeature dfDevice("DF",1);

// #include <I2C/I2CFeature.h>
// I2CFeature pI2C;

// #include <Stepper/CommonStepper.h>
// //StepperFeature ffStepper;

// FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&deviceManager,0};
//FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,&ffStepper,&deviceManager,0};

// Device Drivers

#include <Hello/HelloDriver.h>
HelloDriver ddHello("HW");

#include <Peek/PeekDriver.h>
PeekDriver ddPeek("PK");

 #include <MCP9808/MCP9808Driver.h>
 MCP9808Driver ddMCP9808("TC",1);

DeviceDriver *selectedDevices[] = {&ddHello,&ddPeek,&ddMCP9808,0};
// DeviceDriver *selectedDevices[] = {&ddMCP9808,0};
//DeviceDriver *selectedDevices[] = {&ddHello,&ddPeek,&ddMCP9808,0};
