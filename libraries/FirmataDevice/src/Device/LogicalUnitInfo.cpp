#include "LogicalUnitInfo.h"

LogicalUnitInfo::LogicalUnitInfo() : theDeviceObject(0) {}

LogicalUnitInfo::~LogicalUnitInfo() {
  if (theDeviceObject != 0) {
    delete theDeviceObject;
  }
}

  void * LogicalUnitInfo::getDeviceObject() {
    return theDeviceObject;
  }

  void LogicalUnitInfo::setDeviceObject(void *obj) {
    theDeviceObject = obj;
  }

