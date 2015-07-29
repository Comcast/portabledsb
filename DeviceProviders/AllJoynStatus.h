#pragma once

#include <string>
#include <alljoyn/Status.h>

class AllJoynStatus
{
public:
  AllJoynStatus(QStatus statusCode)
    : m_statusCode(statusCode)
  { }

  inline QStatus GetCode() const
    { return m_statusCode; }

  inline std::string GetText() const
    { return QCC_StatusText(m_statusCode); }

  inline bool IsSuccess() const
    { return m_statusCode == QStatus::ER_OK; }

  inline bool IsFailure() const
    { return m_statusCode != QStatus::ER_OK; }

private:
  QStatus m_statusCode;
};
