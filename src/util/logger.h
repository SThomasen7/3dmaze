#ifndef _LOGGER_H_
#define _LOGGER_H_ 1

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

enum LogLevel{
  Verbose,
  Info,
  Warn,
  Error,
  Disable
};

typedef LogLevel LL;

class Logger{

  LogLevel severity;

  template <typename T>
  void streamAppend(std::ostringstream& oss, const T& arg){
    oss << arg;
  }

public:

  Logger(){ severity = LogLevel::Info; }

  inline void setLogLevel(LogLevel severity){
    this->severity = severity;
  }

  template<typename... Args>
  void log(LogLevel severity, const char* file=__FILE__, int line=__LINE__, Args... args){

    // skip if severity is below threshold
    if(severity < this->severity || severity == LL::Disable){
      return;
    }

    std::ostringstream oss;
    // output the file name and line number this was called from
    oss << "[" << (strrchr(file, '/') ? strrchr(file, '/') + 1 : file);
    oss << ":" << line << "] ";

    switch(severity){
      case LogLevel::Verbose:
        oss << "Debug: ";
        break;
      case LogLevel::Info:
        oss << "Info : ";
        break;
      case LogLevel::Warn:
        oss << "Warn : ";
        break;
      case LogLevel::Error:
        oss << "ERROR: ";
        break;
    }

    // output the log level
    (streamAppend(oss, args), ...); // fold expression
    std::cout << oss.str() << std::endl;
  }

};

extern Logger LOGGER;
#define LOG(log_level, ...) LOGGER.log(log_level, __FILE__, __LINE__, __VA_ARGS__)

#endif
