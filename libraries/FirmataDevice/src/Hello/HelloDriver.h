#ifndef HelloDriver_h
#define HelloDriver_h

#include <FirmataCore.h>
#include <DeviceDriver.h>

#define MAX_HELLO_MINOR_HANDLE_COUNT 8

enum class HelloRegister;

class HelloDriver: public DeviceDriver
{
public:
    HelloDriver(char *dNameRoot = "Hello", int count = 5);

    int open(char *name, int flags = 0);

    int status(int handle, int reg, int count, byte *buf);
    int control(int handle, int reg, int count, byte *buf);

    int read(int handle, int count, byte *buf);
    int write(int handle, int count, byte *buf);

    int close(int handle);

protected:
    class HelloDeviceInfo: public DeviceInfo {
    public:
        HelloDeviceInfo(char *interjection = "Hello", char *firstWho = "World") :
            DeviceInfo(interjection),
            who(firstWho) {}
        void setWho(char *newWho) {
            who = newWho;
        }
        char *getWho() {
            return who;
        }

    private:
        char *who;
    };

    HelloDeviceInfo devices[MAX_HELLO_MINOR_HANDLE_COUNT];
    int actualMinorHandleCount = 0;

};

#endif
