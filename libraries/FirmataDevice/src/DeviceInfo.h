#ifndef DeviceInfo_h
#define DeviceInfo_h

#include <string.h>

class DeviceInfo {

public:

  DeviceInfo();

  void setOpen(bool stateOfOpen);
  bool isOpen();
  void setDeviceName(char *name);
  char *getDeviceName();

private:
  char *deviceName = "";
  bool deviceIsOpen = false;

};

#endif
