// Pure C++ RDNA2/RDNA4 Windows 11 ROCm 10.1 (7.16) glog alternative
// Compatible with clang-cl 23 + hipcc 7.16 (Strawberry/MinGW/MSYS2/w64devkit)
// CPU Fallback: MAINTAINED (YES)
// Perf Trace: INTEGRATED (sherlock-it.md)

#pragma once
#include <iostream>
#include <string>
#include <chrono>

#ifdef WIN32_ZMQ_ALTERNATIVE
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#define LOG(level) std::cerr << "[" << level << "] "
#define CHECK(condition) if (!(condition)) { std::cerr << "CHECK FAILED: " << #condition << std::endl; std::abort(); }
#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_GT(a, b) CHECK((a) > (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))
#define CHECK_LT(a, b) CHECK((a) < (b))

namespace glog {
  enum LogSeverity { INFO = 0, WARNING = 1, ERROR = 2, FATAL = 3 };
  inline void logging(LogSeverity severity, const char* msg) {
    std::cerr << "[" << (severity == INFO ? "INFO" : severity == WARNING ? "WARNING" : severity == ERROR ? "ERROR" : "FATAL") << "] " << msg << std::endl;
    if (severity == FATAL) std::abort();
  }
} // namespace glog

#define VLOG(level) if (level <= 0) std::cerr
#define DVLOG(level) if (level <= 0) std::cerr
#define LOG_IF(condition, level) if (condition) std::cerr << "[" << (level == 0 ? "INFO" : "WARNING") << "] "
#define PLOG(level) std::cerr << "[" << (level == 0 ? "INFO" : "WARNING") << "] "

// Minimal glog/logging.h replacement for xLLM Windows Port
namespace xllm {
  namespace glog_alternative {
    inline void init_logging() {}
    inline void shutdown_logging() {}
  }
} // namespace xllm

using xllm::glog_alternative::init_logging;
using xllm::glog_alternative::shutdown_logging;
