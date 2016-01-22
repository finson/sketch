#ifndef Logger_h
#define Logger_h

#include <string.h>
#include <stdlib.h>

class Logger {
public:
  Logger(char *name);
  ~Logger();

  void debug(char *msg);
  void error(char *msg);
  void info(char *msg);
  void trace(char *msg);
  void warn(char *msg);

  char *getName();

private:
  char *loggerName;
};

#endif
