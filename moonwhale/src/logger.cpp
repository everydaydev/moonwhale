#include "logger.h"
#include <cstdarg>
#include <cstdio>

static const char* g_logLevelPrefixes[] = {
    "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FETAL"
};

namespace logger
{

static struct
{
    LogLevel maxLevel;
} g_logConfig;

void setMaxLevel(LogLevel level)
{
    g_logConfig.maxLevel = level;
}

LogLevel getMaxLevel()
{
    return g_logConfig.maxLevel;
}

namespace detail
{
void doLog(LogLevel level, const char* file, int line, const char* fmt, ...)
{
    if (level > getMaxLevel())
    {
        return;
    }

    printf("[%s][%s:%d]: ", g_logLevelPrefixes[static_cast<int>(level)], file, line);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
}//detail

}//logger