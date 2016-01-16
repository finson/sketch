#include <avr/pgmspace.h>

//const PROGMEM uint8_t MCP9808Driver::driverSemVer[] = {6, 0, 1, 0, 0, 0, 0};
//const PROGMEM char MCP9808Driver::driverName[] = {"MCP9808Driver"};

#define DEFINE_SEMVER(theScope, theName, major, minor, patch) \
const PROGMEM char theScope::moduleName[] = {theName}; \
const PROGMEM int8_t theScope::moduleSemVer[] = {3, major, minor, patch};

#define DECLARE_SEMVER static const PROGMEM char moduleName[]; static const PROGMEM int8_t moduleSemVer[];

class DriverExample {
  public:
private:
  DECLARE_SEMVER
};

