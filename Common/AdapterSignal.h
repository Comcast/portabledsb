#pragma once

#include "Common/AdapterObject.h"
#include "Common/AdapterNamedValue.h"

#include <string>
#include <vector>

namespace adapter
{
  class Interface;
  
  class Signal : public adapter::Object
  {
    friend class Interface;

  public:
    using Vector = std::vector<Signal>;

    Signal(std::string const& name);

    std::string const& GetInterface() const
      { return m_interface; }

    NamedValue::Vector const& GetParameters() const
      { return m_params; }

    void AddParameter(NamedValue const& value)
      { m_params.push_back(value); }

    void ClearParameters()
      { m_params.clear(); }

  private:
    void SetInterfaceName(std::string const& name)
      { m_interface = name; }

  private:
    std::string         m_interface;
    NamedValue::Vector  m_params;
  };

  Signal MakeAdapterSignal(std::string const& name,
    NamedValue::Vector const& params = NamedValue::Vector());
}
