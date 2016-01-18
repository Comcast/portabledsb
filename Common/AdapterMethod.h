#pragma once

#include "Common/AdapterObject.h"
#include "Common/AdapterValue.h"

#include <string>
#include <vector>

namespace common
{
  class AdapterInterface;

  class AdapterMethod : public AdapterObject
  {
    friend class AdapterInterface;

  public:
    typedef std::vector<AdapterMethod> Vector;

    AdapterMethod(std::string const& name, std::string const& description = std::string())
      : AdapterObject(name)
    {
      SetDescription(description);
    }

    std::string const& GetInterface() const
      { return m_interface; }

    AdapterValue::Vector const& GetInputArguments() const
      { return m_inArgs; }

    void CearInputArguments()
      { m_inArgs.clear(); }

    void AddInputArgument(AdapterValue const& value)
      { m_inArgs.push_back(value); }

    AdapterValue::Vector const& GetOutputArguments() const
      { return m_outArgs; }

    void ClearOutputArguments()
      { m_outArgs.clear(); }

    void AddOutputArgument(AdapterValue const& value)
      { m_outArgs.push_back(value); }

  private:
    void SetInterfaceName(std::string const& name)
      { m_interface = name; }

  private:
    std::string m_interface;
    AdapterValue::Vector m_inArgs;
    AdapterValue::Vector m_outArgs;
  };

  inline AdapterMethod MakeAdapterMethod(
      std::string const& name,
      AdapterValue::Vector const& attrs = AdapterValue::Vector(),
      std::string const& description = std::string(),
      AdapterValue::Vector const& inputArgs = AdapterValue::Vector(),
      AdapterValue::Vector const& outputArgs = AdapterValue::Vector())
  {
    common::AdapterMethod method(name);
    for (auto attr : attrs)
      method.AddAttribute(attr.GetName(), attr.GetValue());
    for (auto in : inputArgs)
      method.AddInputArgument(in);
    for (auto out : outputArgs)
      method.AddOutputArgument(out);
    method.SetDescription(description);
    return std::move(method);
  }
}
