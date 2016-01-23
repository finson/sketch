#ifndef test_assertions_h
#define test_assertions_h

#include <Arduino.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

class Tester {
public:

  Tester();
  ~Tester();

  void beforeGroup(char *groupName);
  void beforeTest(char *testName);
  void afterTest();
  void afterGroup();

  bool assertTrue(char *msg, bool condition);
  bool assertFalse(char *msg, bool condition);

  bool assertEquals(char *msg, uint8_t expected, uint8_t actual);
  bool assertEquals(char *msg, uint16_t expected, uint16_t actual);
  bool assertEquals(char *msg, uint32_t expected, uint32_t actual);

  uint32_t getTestFailureCount();
  uint32_t getGroupFailureCount();

private:

  char *theTestName;
  char *theGroupName;

  uint32_t testFailureCount;
  uint32_t groupFailureCount;

  void handleFailure(char *msg);
  bool assertConditionIsTrue(char *msg, bool condition);
};

#endif
