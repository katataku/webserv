#ifndef SRCS_LOGGING_LOGGING_HPP_
#define SRCS_LOGGING_LOGGING_HPP_

#include <sys/time.h>

#include <iostream>
#include <string>

#ifdef DEBUG
#define DEBUG_LEVEL E_DEBUG
#endif

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL E_DEBUG
#endif

typedef enum { E_FATAL, E_ERROR, E_WARN, E_INFO, E_DEBUG } DEBUG_LEVEL_ENUM;

class Logging {
 public:
    Logging();
    explicit Logging(std::string);
    void Fatal(std::string);
    void Error(std::string);
    void Warn(std::string);
    void Info(std::string);
    void Debug(std::string);

 private:
    std::string caller_name;
    void PrintLog(std::string, std::string);
};

#endif  // SRCS_LOGGING_LOGGING_HPP_
