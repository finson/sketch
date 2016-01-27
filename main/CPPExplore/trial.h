#include <avr/pgmspace.h>

//const PROGMEM uint8_t MCP9808Driver::driverSemVer[] = {6, 0, 1, 0, 0, 0, 0};
//const PROGMEM char MCP9808Driver::driverName[] = {"MCP9808Driver"};

#define DECLARE_SEMVER static const PROGMEM char moduleName[]; static const PROGMEM int8_t moduleSemVer[];

#define DEFINE_SEMVER(theName, major, minor, patch) \
const PROGMEM char theName::moduleName[] = {#theName}; \
const PROGMEM int8_t theName::moduleSemVer[] = {3, major, minor, patch};

class DriverExample {
public:
private:
DECLARE_SEMVER
};

DEFINE_SEMVER(DriverExample,0,1,0)
