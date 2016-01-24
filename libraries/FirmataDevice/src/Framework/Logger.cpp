#include "Logger.h"

Logger::Logger(char *name) {
  loggerName = strdup(name);
  currentLogLevel = LogLevel::OFF;
}

Logger::~Logger() {
  if (loggerName != 0) {
    free(loggerName);
  }
}

template <typename Msg, typename... Items>
void Logger::fatal(Msg msg,Items ...valPack) {
  recordLogEvent(LogLevel::FATAL, msg, valPack...);
}

template <typename Msg, typename... Items>
void Logger::error(Msg msg,Items ...valPack) {
  recordLogEvent(LogLevel::ERROR, msg, valPack...);
}

template <typename Msg, typename... Items>
void Logger::warn(Msg msg,Items ...valPack) {
  recordLogEvent(LogLevel::WARN, msg, valPack...);
}

template <typename Msg, typename... Items>
void Logger::info(Msg msg,Items ...valPack) {
  recordLogEvent(LogLevel::INFO, msg, valPack...);
}

template <typename Msg, typename... Items>
void Logger::debug(Msg msg,Items ...valPack) {
  recordLogEvent(LogLevel::DEBUG, msg, valPack...);
}

template <typename Msg, typename... Items>
void Logger::trace(Msg msg,Items ...valPack) {
  recordLogEvent(LogLevel::TRACE, msg, valPack...);
}

char *Logger::getLoggerName(){
  return loggerName;
  }

int Logger::getCurrentLogLevel() {
  return currentLogLevel;
}

void Logger::setCurrentLogLevel(int newLevel) {
  currentLogLevel = abs(newLevel);
}

bool Logger::isEnabled(int lvl) {
  return (lvl >= currentLogLevel);
}

template <typename MsgLevel, typename Msg, typename... Items>
void Logger::recordLogEvent(MsgLevel theLvl, Msg theMsg, Items ...valPack) {

  if (isEnabled(theLvl)) {

    int levelIndex = constrain(theLvl / 100, 0, LogLevel::COUNT);
    Serial.print(LogLevel::levelName[levelIndex]);
    Serial.print(" : ");
    Serial.print(theMsg);
    Serial.print(" [");
    valuePrinter(valPack...);
    Serial.println("]");
  }
}

template <typename First, typename... Rest>
void Logger::valuePrinter(First valFirst, Rest ...valPack) {
Serial.print(" ");
Serial.print(valFirst);
valuePrinter(valPack...);
}

template <typename Only>
void Logger::valuePrinter(Only value) {
Serial.print(" ");
Serial.print(value);
}

void Logger::valuePrinter() {}
