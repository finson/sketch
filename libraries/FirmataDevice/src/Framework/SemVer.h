#ifndef SemVer_h
#define SemVer_h

// Put call to DECLARE_SEMVER in the declaration file xx.h in the private
// section of the device driver class declaration.  For example:
//
// class TrialDriver {
// public:
//   int yada();
// private:
//   DECLARE_SEMVER
// };

#define DECLARE_SEMVER \
static const PROGMEM uint8_t releaseVersion[];\
static const PROGMEM char scopeName[];

// Put call to DEFINE_SEMVER in the definition file xx.cpp, but outside
// of any constructor or method.  For example:
//
// DEFINE_SEMVER(TrialDriver,1,0,3)

#define DEFINE_SEMVER(theScopeName, major, minor, patch) \
const PROGMEM uint8_t theScopeName::releaseVersion[] = {3, major, minor, patch};\
const PROGMEM char theScopeName::scopeName[] = {#theScopeName}; \

#endif
