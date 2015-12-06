#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(char *name) {
    deviceName = name;
    deviceIsOpen = false;
}

void DeviceInfo::setOpen(bool stateOfOpen) {
    deviceIsOpen = stateOfOpen;
}

bool DeviceInfo::isOpen() {
    return deviceIsOpen;
}

void DeviceInfo::setDeviceName(char *name) {
    deviceName = name;
}

char *DeviceInfo::getDeviceName() {
    return deviceName;
}
