#pragma once

#include "Common/Variant.h"

#include <memory>
#include <vector>

namespace common
{
  class AdapterValue
  {
  public:
    typedef std::vector<AdapterValue> Vector;

    AdapterValue()
    {
    }

    AdapterValue(std::string const& name, Variant const& value)
      : m_name(name)
      , m_value(value) { }

    std::string const& GetName() const
      { return m_name; }

    Variant const& GetValue() const
      { return m_value; }

    void SetValue(Variant const& value)
      { m_value = value; }

    bool IsNull() const
      { return m_name.empty() && m_value.IsEmpty(); }

    static AdapterValue const& Null()
    {
      static AdapterValue kNullValue("", Variant());
      return kNullValue;
    }

  private:
    std::string   m_name;
    Variant       m_value;
  };
}
