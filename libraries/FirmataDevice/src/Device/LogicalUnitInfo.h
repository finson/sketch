#ifndef LogicalUnitInfo_h
#define LogicalUnitInfo_h

#include <string.h>

#define MAX_LU_NAME_LENGTH 31

class LogicalUnitInfo {

public:

  LogicalUnitInfo(const char *name);
  LogicalUnitInfo();

  void setOpen(bool stateOfOpen);
  bool isOpen();
  void setLogicalUnitName(const char *name);
  const char *getLogicalUnitName();

private:
  char luName[MAX_LU_NAME_LENGTH+1];
  bool luIsOpen = false;

};

#endif
