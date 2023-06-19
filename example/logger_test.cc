#include <string>
#include "simplelogger.h"
int main() {
    simplelogger::singleton()->set_log_level(simplelogger::LogLevel::kDebug);
    std::string str("std::string");
    LOG_ERROR << 'c';
    LOG_WARN << "a";
    LOG_INFO << "c";
    LOG_DEBUG << "c";
    LOG_FATAL << "c";
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}