#ifndef LoggerTemplates_h
#define LoggerTemplates_h

template <typename... ItemType>
void Logger::fatal(char *msg,ItemType ... valPack) {
  recordLogEvent(LogLevel::FATAL, msg, valPack...);
}

template <typename... ItemType>
void Logger::error(char *msg,ItemType ...valPack) {
  recordLogEvent(LogLevel::ERROR, msg, valPack...);
}

template <typename... ItemType>
void Logger::warn(char *msg,ItemType ...valPack) {
  recordLogEvent(LogLevel::WARN, msg, valPack...);
}

template <typename... ItemType>
void Logger::info(char *msg,ItemType ...valPack) {
  recordLogEvent(LogLevel::INFO, msg, valPack...);
}

template <typename... ItemType>
void Logger::debug(char *msg,ItemType ...valPack) {
  recordLogEvent(LogLevel::DEBUG, msg, valPack...);
}

template <typename... ItemType>
void Logger::trace(char *msg,ItemType ...valPack) {
  recordLogEvent(LogLevel::TRACE, msg, valPack...);
}

template <typename... ItemType>
void Logger::recordLogEvent(int theLvl, char *theMsg, ItemType ...valPack) {
  if (isEnabled(theLvl)) {
    int levelIndex = constrain(theLvl / 100, 0, LogLevel::COUNT);
    Serial.print('~');
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

#endif
