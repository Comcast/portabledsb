
#include "Common/Log.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>

#include <mutex>

#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#endif

#include <map>

namespace
{
  common::LogLevel const kDefaultLoggingLevel = common::LogLevel::Info;

  FILE* gLog = stdout;

  std::mutex gLogMutex;
  common::LogLevel gDefaultLogLevel = common::LogLevel::Info;

  typedef std::map< std::string, std::shared_ptr<common::Logger> > LogMap;
  LogMap gLogMap;

  char const* LevelToString(common::LogLevel l)
  {
    char const* s = "unknown";
    switch (l)
    {
      case common::LogLevel::Off: s = "OFF"; break;
      case common::LogLevel::Debug: s = "DEBUG"; break;
      case common::LogLevel::Info: s = "INFO"; break;
      case common::LogLevel::Warn: s = "WARN"; break;
      case common::LogLevel::Error: s = "ERROR"; break;
      case common::LogLevel::Fatal: s = "FATAL"; break;
      default:
        break;
    }
    return s;
  }

  #if defined(__APPLE__)
  #define ThreadId_FMT "%p"
  pthread_t GetCurrentThreadId() { return pthread_self(); }
  #elif defined(__linux__)
  #define ThreadId_FMT "%ld"
  long GetCurrentThreadId() { return syscall(__NR_gettid); }
  #else
  #error ThreadId_FMT and GetCurrentThreadId() are not defined for your platform
  #endif
}

common::Logger::Logger(std::string const& name)
  : m_name(name)
{
}


void
common::Logger::SetDefaultLevel(LogLevel level)
{
  std::lock_guard<std::mutex> lock(gLogMutex);
  gDefaultLogLevel = level;
}

void
common::Logger::Write(LogLevel level, char const* file, int line, char const* format, ...)
{ 
  va_list args;
  va_start(args, format);
  VaLog(level, file, line, format, args);
  va_end(args);
}

void
common::Logger::SetLevel(LogLevel level)
{
  std::lock_guard<std::mutex> lock(gLogMutex);
  m_level = level;
}

bool
common::Logger::IsLevelEnabled(std::string const& module, LogLevel level)
{
  std::lock_guard<std::mutex> lock(gLogMutex);

  common::LogLevel minLevel = gDefaultLogLevel;
  LogMap::const_iterator itr = gLogMap.find(module);
  if (itr != gLogMap.end())
    minLevel = itr->second->m_level;

  return level >= minLevel;
}

void
common::Logger::VaLog(LogLevel level, const char* file, int line, const char* format, va_list args)
{
  struct tm result;
  time_t const clock = time(0);
  localtime_r(&clock, &result);

  char buff[64];
  size_t n = strftime(buff, sizeof(buff), "%Y-%m-%dT%T", &result);
  if (n > 0)
    buff[n] = '\0';

  // The logger is passed around as a shared_ptr<> to the adapter. There's no telling
  // if multiple threads will log to this thing or not. synchronize to be safe
  std::lock_guard<std::mutex> lock(gLogMutex);
  fprintf(gLog, "%s (%5s) Thread-" ThreadId_FMT " [%s:%s:%d] - ", buff, LevelToString(level),
    GetCurrentThreadId(), m_name.c_str(), file, line);
  vfprintf(gLog, format, args);
  fprintf(gLog, "\n");
}

std::shared_ptr<common::Logger>
common::Logger::GetLogger(std::string const& name)
{
  std::shared_ptr<Logger> logger;

  std::lock_guard<std::mutex> lock(gLogMutex);
  LogMap::const_iterator itr = gLogMap.find(name);
  if (itr == gLogMap.end())
  {
    logger.reset(new Logger(name));
    logger->m_level = gDefaultLogLevel;
    gLogMap.insert(LogMap::value_type(name, logger));
  }
  else
  {
    logger = itr->second;
  }

  assert(logger.get() != nullptr);
  return logger;
}
