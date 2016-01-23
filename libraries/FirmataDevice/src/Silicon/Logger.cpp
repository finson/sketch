#include "Logger.h"

const char *LogLevel::levelName[8] = {
  "OFF","FATAL","ERROR","WARN ",
  "INFO ","DEBUG","TRACE","ALL  "
};

Logger::Logger(char *name) {
  loggerName = strdup(name);
  currentLogLevel = LogLevel::OFF;
}

Logger::~Logger() {
  if (loggerName != 0) {
    free(loggerName);
  }
}

void Logger::error(char *msg, int val) {
  recordLogEvent(LogLevel::ERROR, msg, val);
}

void Logger::warn(char *msg, int val) {
  recordLogEvent(LogLevel::WARN, msg, val);
}

void Logger::info(char *msg, int val) {
  recordLogEvent(LogLevel::INFO, msg, val);
}

void Logger::debug(char *msg, int val) {
  recordLogEvent(LogLevel::DEBUG, msg, val);
}

void Logger::trace(char *msg, int val) {
  recordLogEvent(LogLevel::TRACE, msg, val);
}

void Logger::recordLogEvent(int lvl, char *msg, int val) {
  if (isEnabled(currentLogLevel)) {
    int levelIndex = constrain(lvl / 100, 0, LogLevel::COUNT);
    Serial.print(LogLevel::levelName[levelIndex]);
    Serial.print(" : ");
    Serial.print(msg);
    Serial.print(" (");
    Serial.print(val, DEC);
    Serial.println(")");
  }
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
