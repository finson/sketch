#include "Logger.h"

  Logger::Logger(char *name) {
    loggerName = strdup(name);
  }

  Logger::~Logger() {
    if (loggerName != 0) {
      free(loggerName);
    }
  }
  void Logger::debug(char *msg){}
  void Logger::error(char *msg){}
  void Logger::info(char *msg){}
  void Logger::trace(char *msg){}
  void Logger::warn(char *msg){}

  char *Logger::getName(){}
