#pragma once

namespace logger
{

enum class LogLevel : int
{
    Trace = 0,
    Debug,
    Info,
    Warning,
    Error,
    Fetal,
};

#define l_trace(fmt, ...) logger::detail::doLog(logger::LogLevel::Trace, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define l_debug(fmt, ...) logger::detail::doLog(logger::LogLevel::Debug, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define l_info(fmt, ...) logger::detail::doLog(logger::LogLevel::Info, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define l_warning(fmt, ...) logger::detail::doLog(logger::LogLevel::Warning, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define l_error(fmt, ...) logger::detail::doLog(logger::LogLevel::Error, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define l_fetal(fmt, ...) logger::detail::doLog(logger::LogLevel::Fetal, __FILE__, __LINE__, fmt, __VA_ARGS__)

void setMaxLevel(LogLevel level);
LogLevel getMaxLevel();

namespace detail
{
void doLog(LogLevel level, const char* file, int line, const char* fmt, ...);
}//detail

}//logger

