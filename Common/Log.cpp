
#include "Common/Log.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <map>

namespace
{
  FILE* gLog = stdout;

  struct StringCompare
  {
    bool operator()(char const* s, char const* t) const
      { return strcmp(s, t) < 0; }
  };

  char const* LevelToString(Common::Logger::Level l)
  {
    char const* s = "unknown";
    switch (l)
    {
      case Common::Logger::DSB_LOGLEVEL_OFF:
        s = "OFF";
        break;
      case Common::Logger::DSB_LOGLEVEL_DEBUG:
        s = "DEBUG";
        break;
      case Common::Logger::DSB_LOGLEVEL_INFO:
        s = "INFO";
        break;
      case Common::Logger::DSB_LOGLEVEL_WARN:
        s = "WARN";
        break;
      case Common::Logger::DSB_LOGLEVEL_ERROR:
        s = "ERROR";
        break;
      default:
        break;
    }
    return s;
  }

  typedef std::map<char const*, Common::Logger::Level, StringCompare> LevelMap;
  LevelMap gLevelMap;

  Common::Logger::Level sDefaultLoggingLevel = Common::Logger::DSB_LOGLEVEL_INFO;

  #ifdef __APPLE__
  #define ThreadId_FMT "%p"
  pthread_t GetCurrentThreadId() { return pthread_self(); }
  #else
  #define ThreadId_FMT "%d"
  int32_t GetCurrentThreadId() { return syscall(SYS_gettid); }
  #endif
}

void Common::Logger::Write(char const* module, Level level, char const* file,
        int line, char const* format, ...)
{ 
  va_list args;
  va_start(args, format);
  VaLog(module, level, file, line, format, args);
  va_end(args);
}

bool Common::Logger::IsLevelEnabled(char const* module, Level level)
{
  Common::Logger::Level minLevel = sDefaultLoggingLevel;
  LevelMap::const_iterator itr = gLevelMap.find(module);
  if (itr != gLevelMap.end())
    minLevel = itr->second;
  
  return level >= minLevel;
}

void Common::Logger::VaLog(const char* module, Level level, const char* /*file*/, int /*line*/,
  const char* format, va_list args)
{
  struct tm result;
  time_t const clock = time(0);
  localtime_r(&clock, &result);

  char buff[64];
  size_t n = strftime(buff, sizeof(buff) - 1, "%Y-%m-%dT%T", &result);
  if (n > 0)
    buff[n] = '\0';
  fprintf(gLog, "%s (%5s) Thread-" ThreadId_FMT " [%s] - ", buff, LevelToString(level), GetCurrentThreadId(), module);
  vfprintf(gLog, format, args);
  fprintf(gLog, "\n");
}

