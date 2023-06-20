#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
// #include <concurrent_queue.h>
#include <mutex>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

/// A line log info, usage is same as 'std::cout'.
// Log format in memory.
//  +-------+------+-----------+------+------------+------+------+
//  | level | time | thread id | file | (function) | line | logs |
//  +-------+------+-----------+------+------------+------+------+

namespace simplelogger {

class LogEntry;
class Logger;

enum LoggerType: uint8_t {
    kSimpleLogger = 0,
    kSyncLogger = 1
};

enum LogLevel: uint8_t {
    kDebug = 0,
    kInfo = 1,
    KWarn = 2,
    kError = 3,
    kFatal = 4,
};

// Stringify log level with width of 5.
static const char *stringifyLogLevel(LogLevel level) {
  const char *levelName[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL", "TRACE"};
  return levelName[level];
}

static unsigned long long get_thread_id() {
#ifdef _WIN32
    return GetCurrentThreadId();
#else
    return (unsigned long long)getpid();
#endif
}

class Logger {
public:
  Logger(): log_level_(LogLevel::kInfo) {}
  virtual void set_log_level(LogLevel log_level) {
    log_level_ = log_level;
  }
  virtual LogLevel log_level() {
    return log_level_;
  }
  
  void log(const std::string& msg) {
    std::cout << msg;
  }
  std::string get_color_code(LogLevel level) {
    std::string color_code;
    switch (level) {
        case LogLevel::kDebug:
            color_code = "\033[1;34m"; // blue
            break;
        case LogLevel::kInfo:
            color_code = "\033[1;32m"; // green
            break;
        case LogLevel::KWarn:
            color_code = "\033[1;33m"; // yellow
            break;
        case LogLevel::kError:
            color_code = "\033[1;31m"; // red
            break;
        case LogLevel::kFatal:
            color_code = "\033[1;35m"; // magenta
            break;
        default:
            break;
    }
    return color_code;
  }
  ~Logger() {
    // delete th;
  }
private:
  LogLevel log_level_;
  // ConcurentQueue<std::string> q_;
  // std::thread* th;
};

// Singleton pointer.
static Logger* singleton() {
  static Logger logger;
  return &logger;
}

class LogEntry {
public:
    explicit LogEntry(Logger* logger) : logger_(logger) {}
    ~LogEntry() {
        ss_ << "\n";
        logger_->log(ss_.str());
    }

    template<typename T>
    LogEntry& operator<<(const T& value) {
        ss_ << value;
        return *this;
    }

    LogEntry& operator()() {
      return *this;
    }
private:
    std::stringstream ss_;
    Logger* logger_;
};

static std::string get_time() {
  std::ostringstream ss;
  auto now = std::chrono::system_clock::now();
    // 将时间点转换为时间结构体
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  // 将时间结构体转换为本地时间
  std::tm* local_time = std::localtime(&time);

  ss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S.") \
    << std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000 \
    << std::setfill('0') << std::setw(3) \
    << std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 1000;
  
  return ss.str();
}

}  //namespace simplelogger

#define LOG(level) \
    if (level >= simplelogger::singleton()->log_level()) \
        simplelogger::LogEntry(simplelogger::singleton()) \
        << simplelogger::singleton()->get_color_code(level) \
        << "[" << stringifyLogLevel(level) << "] " \
        << simplelogger::get_time() << " " << simplelogger::get_thread_id() << " " \
        << __FILE__ << ":"  << __FUNCTION__ << ":" << __LINE__ << " "

#define LOG_DEBUG LOG(simplelogger::LogLevel::kDebug)
#define LOG_INFO LOG(simplelogger::LogLevel::kInfo)
#define LOG_WARN LOG(simplelogger::LogLevel::KWarn)
#define LOG_ERROR LOG(simplelogger::LogLevel::kError)
#define LOG_FATAL LOG(simplelogger::LogLevel::kFatal)
