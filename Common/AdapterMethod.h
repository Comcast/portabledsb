#pragma once

#include "Common/AdapterObject.h"
#include "Common/AdapterNamedValue.h"

#include <string>
#include <vector>

namespace adapter
{
  class Method : public adapter::Object
  {
    friend class Interface;

  public:
    typedef std::vector<Method> Vector;

    Method(std::string const& name, std::string const& description = std::string())
      : adapter::Object(name)
    {
      SetDescription(description);
    }

    std::string const& GetInterface() const
      { return m_interface; }

    NamedValue::Vector const& GetInputArguments() const
      { return m_inArgs; }

    void CearInputArguments()
      { m_inArgs.clear(); }

    void AddInputArgument(NamedValue const& value)
      { m_inArgs.push_back(value); }

    NamedValue::Vector const& GetOutputArguments() const
      { return m_outArgs; }

    void ClearOutputArguments()
      { m_outArgs.clear(); }

    void AddOutputArgument(NamedValue const& value)
      { m_outArgs.push_back(value); }

  private:
    void SetInterfaceName(std::string const& name)
      { m_interface = name; }

  private:
    std::string m_interface;
    NamedValue::Vector m_inArgs;
    NamedValue::Vector m_outArgs;
  };

  Method MakeMethod(
      std::string const& name,
      NamedValue::Vector const& attrs = NamedValue::Vector(),
      std::string const& description = std::string(),
      NamedValue::Vector const& inputArgs = NamedValue::Vector(),
      NamedValue::Vector const& outputArgs = NamedValue::Vector());
}
