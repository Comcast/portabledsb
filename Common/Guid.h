#pragma once

#include <string>
#include <uuid/uuid.h>

namespace Common
{
  class Guid
  {
  public:
    static Guid Create()
    {
      uuid_t uuid;
      uuid_generate_random(uuid);
      
      #ifdef __linux__
      char* s;
      #else
      uuid_string_t s;
      #endif

      uuid_unparse(uuid, s);

      return Guid(std::string(s));
    }

    static Guid FromString(char const* s)
    {
      return Guid(s);
    }

    std::string ToString() const
      { return m_uuid; }

  private:
    Guid(std::string const& uuid) : m_uuid(uuid) { }

  private:
    std::string m_uuid;
  };
}

