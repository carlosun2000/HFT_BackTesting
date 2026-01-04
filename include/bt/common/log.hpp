#pragma once
#include <string>

namespace bt{
    enum class LogLevel {Debug, Info, Warn,Error};
    void log(LogLevel lvl, const std::string& msg);
}