// Firmata Features

#include <DigitalInputFeature.h>
#include <DigitalOutputFeature.h>
#include <AnalogInputFeature.h>
#include <AnalogOutputFeature.h>

// Device Protocols

////#include <I2CProtocol.h>

// Command sources

//#include <SerialMessageStream.h>

// Associated data structures

DigitalInputFeature digitalInput;
DigitalOutputFeature digitalOutput;
AnalogInputFeature analogInput;
AnalogOutputFeature analogOutput;

//SerialMessageSource serialSource1(1);
//SerialMessageSource serialSource2(2);

//I2CProtocol pI2C;

//MessageSource *selectedMessageSources[] = {&serialSource1, &serialSource2};

FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,0};
//{&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,0};

