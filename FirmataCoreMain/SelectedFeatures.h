#include <DigitalInputFeature.h>
#include <DigitalOutputFeature.h>
#include <AnalogInputFeature.h>
#include <AnalogOutputFeature.h>
////#include <I2CProtocol.h>
//
DigitalInputFeature digitalInput;
DigitalOutputFeature digitalOutput;
AnalogInputFeature analogInput;
AnalogOutputFeature analogOutput;
////I2CProtocol pI2C;

FirmataFeature *selectedFeatures[] = {&digitalInput,&digitalOutput,&analogInput,&analogOutput,0};
//{&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,0};

