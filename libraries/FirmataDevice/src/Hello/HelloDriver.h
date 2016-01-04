#ifndef HelloDriver_h
#define HelloDriver_h

#include <FirmataCore.h>
#include <Device/DeviceDriver.h>

#define MAX_HELLO_LU_COUNT 2
#define MAX_HELLO_TEXT_LENGTH 31
#define SAMPLE_COUNT 16

enum class HelloRegister;

class HelloDriver: public DeviceDriver
{
public:

    HelloDriver(const char *dName = "Hello", int count = 1);

    int open(const char *name, int flags = 0);
    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);
    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);
    int close(int handle);

    int millisecondTimeBase();

    class HelloLUI: public LogicalUnitInfo {

    public:
        HelloLUI(const char *interjection) : LogicalUnitInfo(interjection) {
            strlcpy(who,"World",MAX_HELLO_TEXT_LENGTH+1);
        }

        HelloLUI() : LogicalUnitInfo() {
            strlcpy(who,"World",MAX_HELLO_TEXT_LENGTH+1);
        }

        void setWho(const char *newWho) {
            strlcpy(who,newWho,MAX_HELLO_TEXT_LENGTH+1);
        }

        const char *getWho() {
            return who;
        }

    private:
        char who[MAX_HELLO_TEXT_LENGTH];
    };

private:

    unsigned long previousTime[2];   // the time the last interval expired
    unsigned long currentTime[2];    // the current values from millis() and micros()
    unsigned long samples[SAMPLE_COUNT+1];
    int sampleIndex;
    boolean isSampleBufferFull;
    unsigned long calculateAverageInterval();

    HelloLUI logicalUnits[MAX_HELLO_LU_COUNT];
};

#endif
