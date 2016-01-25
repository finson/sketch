#ifndef Logger_h
#define Logger_h

#include <arduino.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "LogLevel.h"

class Logger {
public:
  Logger(char *name);
  ~Logger();

  template <typename... ItemType>
  void fatal(char *msg, ItemType ...valPack);

  template <typename... ItemType>
  void error(char *msg, ItemType ...valPack);

  template <typename... ItemType>
  void warn(char *msg, ItemType ...valPack);

  template <typename... ItemType>
  void info(char *msg, ItemType ...valPack);

  template <typename... ItemType>
  void debug(char *msg, ItemType ...valPack);

  template <typename... ItemType>
  void trace(char *msg, ItemType ...valPack);

  char *getLoggerName();
  int getCurrentLogLevel();
  void setCurrentLogLevel(int newLevel);
  bool isEnabled(int lvl);

private:
  char *loggerName;
  int currentLogLevel;


template <typename... ItemType>
void recordLogEvent(int theLvl, char *theMsg, ItemType ...valPack);

template <typename First, typename... Rest>
void valuePrinter(First valFirst, Rest ...valPack);

template <typename Only>
void valuePrinter(Only value);

void valuePrinter();


};

#include "LoggerTemplates.h"

#endif
