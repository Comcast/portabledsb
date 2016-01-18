#pragma once

#include "Common/AdapterObject.h"
#include "Common/AdapterValue.h"

#include <string>
#include <vector>

namespace common
{
  class AdapterInterface;
  
  class AdapterSignal : public AdapterObject
  {
    friend class AdapterInterface;

  public:
    typedef std::vector<AdapterSignal> Vector;

    AdapterSignal(std::string const& name)
      : AdapterObject(name)
    {
    }

    std::string const& GetInterface() const
      { return m_interface; }

    AdapterValue::Vector const& GetParameters() const
      { return m_params; }

    void AddParameter(AdapterValue const& value)
      { m_params.push_back(value); }

    void ClearParameters()
      { m_params.clear(); }

  private:
    void SetInterfaceName(std::string const& name)
      { m_interface = name; }

  private:
    std::string           m_interface;
    AdapterValue::Vector  m_params;
  };

  inline AdapterSignal MakeAdapterSignal(std::string const& name,
    AdapterValue::Vector const& params = AdapterValue::Vector())
  {
    common::AdapterSignal signal(name);
    for (auto const& param : params)
      signal.AddParameter(param);
    return std::move(signal);
  }
}
