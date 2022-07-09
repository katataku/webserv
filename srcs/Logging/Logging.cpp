#include "Logging.hpp"

Logging::Logging() : caller_name("") {}

Logging::Logging(std::string str) : caller_name(str) {}

void Logging::PrintLog(std::string str, std::string error_level) {
    struct timespec ts;
    struct tm t;
    char buf[32];

    clock_gettime(CLOCK_REALTIME, &ts);
    localtime_r(&ts.tv_sec, &t);
    strftime(buf, 32, "%Y/%m/%d %H:%M:%S", &t);

    std::cout << std::string(buf) << " [" << error_level << "] "
              << this->caller_name << " " << str << std::endl;
}

void Logging::Fatal(std::string str) {
    if (DEBUG_LEVEL >= E_FATAL) this->PrintLog(str, "FATAL");
}
void Logging::Error(std::string str) {
    if (DEBUG_LEVEL >= E_ERROR) this->PrintLog(str, "ERROR");
}
void Logging::Warn(std::string str) {
    if (DEBUG_LEVEL >= E_WARN) this->PrintLog(str, "WARN");
}
void Logging::Info(std::string str) {
    if (DEBUG_LEVEL >= E_INFO) this->PrintLog(str, "INFO");
}
void Logging::Debug(std::string str) {
    if (DEBUG_LEVEL >= E_DEBUG) this->PrintLog(str, "DEBUG");
}
