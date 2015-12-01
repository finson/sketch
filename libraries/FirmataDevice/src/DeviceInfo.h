#ifndef DeviceInfo_h
#define DeviceInfo_h

class DeviceInfo {

public:

  DeviceInfo(char *name);

  void setOpen(bool stateOfOpen);
  bool isOpen();
  void setDeviceName(char *name);
  char *getDeviceName();

private:
  char *deviceName = "";
  bool deviceIsOpen = false;

};

#endif
