#include "Bridge/Log.h"
#include "Common/Log.h"

namespace
{
  common::Logger::Level toLogLevel(bridge::AdapterLogLevel level)
  {
    common::Logger::Level l = common::Logger::DSB_LOGLEVEL_OFF;
    switch (level)
    {
      case bridge::AdapterLogLevel::Off:    l = common::Logger::DSB_LOGLEVEL_OFF; break;
      case bridge::AdapterLogLevel::Debug:  l = common::Logger::DSB_LOGLEVEL_DEBUG; break;
      case bridge::AdapterLogLevel::Info:   l = common::Logger::DSB_LOGLEVEL_INFO; break;
      case bridge::AdapterLogLevel::Warn:   l = common::Logger::DSB_LOGLEVEL_WARN; break;
      case bridge::AdapterLogLevel::Error:  l = common::Logger::DSB_LOGLEVEL_ERROR; break;
      case bridge::AdapterLogLevel::Fatal:  l = common::Logger::DSB_LOGLEVEL_FATAL; break;
    }
    return l;
  }
}

bridge::AdapterLog::AdapterLog(std::string const& module)
  : m_module(module)
{
}

void
bridge::AdapterLog::Write(AdapterLogLevel level, char const* file, int line, char const* format, ...)
{
  va_list args;
  va_start(args, format);
  common::Logger::VaLog(m_module.c_str(), toLogLevel(level), file, line, format, args);
  va_end(args);
}

bool
bridge::AdapterLog::IsLevelEnabled(AdapterLogLevel level)
{
  common::Logger::Level l = toLogLevel(level);
  return common::Logger::IsLevelEnabled(m_module.c_str(), l);
}

