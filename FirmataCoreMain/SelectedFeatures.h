#include <FirmataFeature.h>
#include <DigitalInputFeature.h>
#include <DigitalOutputFeature.h>
#include <AnalogInputFeature.h>
#include <AnalogOutputFeature.h>
#include <I2CProtocol.h>

DigitalInputFeature digitalInput;
DigitalOutputFeature digitalOutput;
AnalogInputFeature analogInput;
AnalogOutputFeature analogOutput;
I2CProtocol pI2C;

FirmataFeature *selectedFeatures[MAX_FEATURES] =
{&digitalInput,&digitalOutput,&analogInput,&analogOutput,&pI2C,0};
