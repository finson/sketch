#include "Tester.h"

Tester::Tester() {
  theTestName = "Unnamed";
  testFailureCount = 0;
  groupFailureCount = 0;
}

Tester::~Tester() {
  if (theGroupName != 0) {
    free(theGroupName);
  }
  if (theTestName !=0) {
    free(theTestName);
  }
}

void Tester::beforeGroup(char *groupName) {
  theGroupName = strdup(groupName);
  Serial.print("Begin group ");
  Serial.println(theGroupName);
  groupFailureCount = 0;
  testFailureCount = 0;

};

void Tester::beforeTest(char *testName) {
  theTestName = strdup(testName);
  testFailureCount = 0;
  Serial.print(" Begin test ");
  Serial.println(theTestName);
}

void Tester::afterTest() {
  Serial.print(" After test ");
  Serial.print(theTestName);
  Serial.print(". ");
  Serial.print(testFailureCount);
  Serial.println(" assertion failures.");
  groupFailureCount += testFailureCount;
  testFailureCount = 0;
}

void Tester::afterGroup() {
  Serial.print("After group ");
  Serial.print(theGroupName);
  Serial.print(". ");
  Serial.print(groupFailureCount);
  Serial.println(" cumulative assertion failures.\n");
  groupFailureCount = 0;
  testFailureCount = 0;
};

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

uint32_t Tester::getGroupFailureCount() {
  return groupFailureCount;
}

bool Tester::assertConditionIsTrue(char *msg, bool condition) {
  if (condition) {
    return true;
  } else {
    handleFailure(msg);
  }
  return false;
}

