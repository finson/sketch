
// Selected device Ports

#include <I2CPort.h>

// Selected command sources

//#include <MessageIOStream.h>

// Firmata Features

#include <DigitalInputFeature.h>
#include <DigitalOutputFeature.h>
#include <AnalogInputFeature.h>
#include <AnalogOutputFeature.h>

// Associated data structures

I2CPort pI2C;

//MessageIOStream serialSource1(1);
//MessageIOStream serialSource2(2);

DigitalInputFeature digitalInput;
DigitalOutputFeature digitalOutput;
AnalogInputFeature analogInput;
AnalogOutputFeature analogOutput;

// DevicePort *selectedDevicePorts[] = {&pI2C};
// {&pI2C,0};

//MessageSource *selectedMessageSources[] = {&serialSource1, &serialSource2};

FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,0};
//{&digitalInput,&digitalOutput,&analogInput,&analogOutput,0};

