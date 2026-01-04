#include "bt/common/log.hpp"
#include <iostream>

namespace bt {

void log(LogLevel lvl, const std::string& msg) {
  const char* tag = "";
  switch (lvl) {
    case LogLevel::Debug: tag = "DEBUG"; break;
    case LogLevel::Info:  tag = "INFO";  break;
    case LogLevel::Warn:  tag = "WARN";  break;
    case LogLevel::Error: tag = "ERROR"; break;
  }
  std::cerr << "[" << tag << "] " << msg << "\n";
}

}