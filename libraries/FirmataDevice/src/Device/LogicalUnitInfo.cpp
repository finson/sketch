#include "LogicalUnitInfo.h"

LogicalUnitInfo::LogicalUnitInfo(const char *name) {
  strlcpy(luName, name, (MAX_LU_NAME_LENGTH + 1));
  luIsOpen = false;
}

LogicalUnitInfo::LogicalUnitInfo() {
  strlcpy(luName, "LUN", (MAX_LU_NAME_LENGTH + 1));
  luIsOpen = false;
}

void LogicalUnitInfo::setOpen(bool stateOfOpen) {
  luIsOpen = stateOfOpen;
}

bool LogicalUnitInfo::isOpen() {
  return luIsOpen;
}

void LogicalUnitInfo::setLogicalUnitName(const char *name) {
  strlcpy(luName, name, (MAX_LU_NAME_LENGTH + 1));
}

const char *LogicalUnitInfo::getLogicalUnitName() {
  return luName;
}

void LogicalUnitInfo::setDeviceObject(void *obj) {
  theDeviceObject = obj;
}

void  *LogicalUnitInfo::getDeviceObject() {
  return theDeviceObject;
}

