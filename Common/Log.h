#pragma once

#if defined(Q_CC_GNU)
#define PRINTF_FORMAT(IDX, FIRST) __attribute__ ((format (printf, IDX, FIRST)))
#else
#define PRINTF_FORMAT(IDX, FIRST)
#endif

#include <stdarg.h>
#include <memory>
#include <string>

namespace common
{
  enum class LogLevel
  {
    Off,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
  };

  class Logger
  {
  public:
    static std::shared_ptr<Logger> GetLogger(std::string const& name);

    Logger(std::string const& name);

    void Write(LogLevel level, const char* file, int line, const char* format, ...)
      PRINTF_FORMAT(5, 6);

    void VaLog(LogLevel level, const char* file, int line, const char* format, va_list args);

    bool IsLevelEnabled(std::string const& module, LogLevel level);

    std::string const& GetName() const
      { return m_name; }

    void SetLevel(LogLevel level);

    static void SetDefaultLevel(LogLevel level);
  private:
    LogLevel    m_level;
    std::string m_name;
  };
}

#define DSB_DECLARE_LOGNAME(LOGNAME) std::string const __dsb_logger_module_name__ = #LOGNAME

#define DSBLOG_WITH_LEVEL(LEVEL, FORMAT, ...) \
    do { std::shared_ptr<common::Logger> log = common::Logger::GetLogger(__dsb_logger_module_name__); \
      if (log->IsLevelEnabled(LEVEL)) { \
        log->Write(LEVEL, __FILE__, __LINE__, FORMAT, ##__VA_ARGS__); \
    } } while (0)

#define DSBLOG(LEVEL, NAME, FORMAT, ...) \
    do { std::shared_ptr<common::Logger> log = common::Logger::GetLogger(NAME); \
      if (log->IsLevelEnabled(NAME, common::LogLevel::LEVEL)) { \
        log->Write(common::LogLevel::LEVEL, __FILE__, __LINE__, FORMAT, ##__VA_ARGS__); \
    } } while (0)


#define DSBLOG_DEBUG(FORMAT, ...) DSBLOG(Debug, __dsb_logger_module_name__, FORMAT, ##__VA_ARGS__)
#define DSBLOG_INFO(FORMAT, ...) DSBLOG(Info, __dsb_logger_module_name__, FORMAT, ##__VA_ARGS__)
#define DSBLOG_WARN(FORMAT, ...) DSBLOG(Warn, __dsb_logger_module_name__, FORMAT, ##__VA_ARGS__)
#define DSBLOG_ERROR(FORMAT, ...) DSBLOG(Error, __dsb_logger_module_name__, FORMAT, ##__VA_ARGS__)

#define DSBLOG_NOT_IMPLEMENTED() DSBLOG_WARN("%s has not been implemented", __func__);

