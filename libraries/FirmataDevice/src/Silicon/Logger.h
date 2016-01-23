#ifndef Logger_h
#define Logger_h

#include <arduino.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

namespace LogLevel {
  const int OFF = 0;
  const int FATAL = 100;
  const int ERROR = 200;
  const int WARN = 300;
  const int INFO = 400;
  const int DEBUG = 500;
  const int TRACE = 600;
  const int ALL = INT_MAX;

  const int COUNT = 8;
  extern const char *levelName[COUNT];
}

class Logger {
public:
  Logger(char *name);
  ~Logger();

  void error(char *msg, int val);
  void warn(char *msg, int val);
  void info(char *msg, int val);
  void debug(char *msg, int val);
  void trace(char *msg, int val);

  char *getLoggerName();
  int getCurrentLogLevel();
  void setCurrentLogLevel(int newLevel);
  bool isEnabled(int lvl);

private:
  char *loggerName;
  int currentLogLevel;
  void recordLogEvent(int lvl, char *msg, int val);
};

#endif
