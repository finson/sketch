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

  template <typename Msg, typename... Items>
  void fatal(Msg msg,Items ...valPack);

  template <typename Msg, typename... Items>
  void error(Msg msg,Items ...valPack);

  template <typename Msg, typename... Items>
  void warn(Msg msg,Items ...valPack);

  template <typename Msg, typename... Items>
  void info(Msg msg,Items ...valPack);

  template <typename Msg, typename... Items>
  void debug(Msg msg,Items ...valPack);

  template <typename Msg, typename... Items>
  void trace(Msg msg,Items ...valPack);

  char *getLoggerName();
  int getCurrentLogLevel();
  void setCurrentLogLevel(int newLevel);
  bool isEnabled(int lvl);

private:
  char *loggerName;
  int currentLogLevel;


template <typename MsgLevel, typename Msg, typename... Items>
void recordLogEvent(MsgLevel theLvl, Msg theMsg, Items ...valPack);

template <typename First, typename... Rest>
void valuePrinter(First valFirst, Rest ...valPack);

template <typename Only>
void valuePrinter(Only value);

void valuePrinter();


};

#endif
