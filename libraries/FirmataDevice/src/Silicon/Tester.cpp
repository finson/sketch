#include "Tester.h"

Tester::Tester(char *suiteName) {
  theSuiteName = strdup(suiteName);
  theTestName = "Unnamed";
  suiteFailureCount = 0;
  testFailureCount = 0;
  Serial.print("Begin suite ");
  Serial.println(theSuiteName);
}

Tester::~Tester() {
  if (theSuiteName != 0) {
    free(theSuiteName);
  }
  if (theTestName !=0) {
    free(theTestName);
  }
}

void Tester::before(char *testName) {
  theTestName = strdup(testName);
  testFailureCount = 0;
  Serial.print("Begin test ");
  Serial.println(theTestName);
}

void Tester::after() {
  Serial.print("After test ");
  Serial.print(theTestName);
  Serial.print(". Found ");
  Serial.print(testFailureCount);
  Serial.println(" assertion failures.");
  suiteFailureCount += testFailureCount;
  testFailureCount = 0;
}

bool Tester::assertTrue(char *msg, bool condition) {
  return assertConditionIsTrue(msg, condition);
}

bool Tester::assertFalse(char *msg, bool condition) {
  return assertConditionIsTrue(msg, !condition);
}

bool Tester::assertEquals(char *msg, uint8_t expected, uint8_t actual) {
  return assertConditionIsTrue(msg, (expected == actual));
}

bool Tester::assertEquals(char *msg, uint16_t expected, uint16_t actual) {
  return assertConditionIsTrue(msg, (expected == actual));
}

bool Tester::assertEquals(char *msg, uint32_t expected, uint32_t actual) {
  return assertConditionIsTrue(msg, (expected == actual));
}


void Tester::handleFailure(char *msg) {
  testFailureCount += 1;
}

uint32_t Tester::getTestFailureCount() {
  return testFailureCount;
}

uint32_t Tester::getSuiteFailureCount() {
  return suiteFailureCount;
}

bool Tester::assertConditionIsTrue(char *msg, bool condition) {
  if (condition) {
    return true;
  } else {
    handleFailure(msg);
  }
  return false;
}

