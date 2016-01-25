#ifndef test_assertions_h
#define test_assertions_h

#include <Arduino.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Logger.h"

class Tester {
public:

  Tester();
  ~Tester();

  void beforeGroup(char *groupName);
  void beforeTest(char *testName);
  void afterTest();
  void afterGroup();

  void assertTrue(char *msg, bool condition);
  void assertFalse(char *msg, bool condition);

  template <typename T0, typename T1>
  void assertEquals(char *msg, T0 expected, T1 actual);

  int getTestFailureCount();
  int getGroupFailureCount();

private:

  Logger *logger;

  char *theTestName;
  char *theGroupName;

  int testFailureCount;
  int groupFailureCount;
};

#include "TesterTemplates.h"

#endif
