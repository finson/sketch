#include "DeviceInfo.h"

DeviceInfo::DeviceInfo() {
    deviceName = "";
    deviceIsOpen = false;
}

void DeviceInfo::setOpen(bool stateOfOpen) {
    deviceIsOpen = stateOfOpen;
}

bool DeviceInfo::isOpen() {
    return deviceIsOpen;
}

void DeviceInfo::setDeviceName(char *name) {
    deviceName = strdup(name);
}

char *DeviceInfo::getDeviceName() {
    return strdup(deviceName);
}
