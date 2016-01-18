#pragma once

#include "Common/AdapterMethod.h"
#include "Common/AdapterObject.h"
#include "Common/AdapterProperty.h"
#include "Common/AdapterSignal.h"

#include <vector>

namespace common
{
  class AdapterInterface : public AdapterObject
  {
  public:
    typedef std::vector<AdapterInterface> Vector;

    AdapterInterface(std::string const& name)
      : AdapterObject(name)
    {
    }

    AdapterProperty::Vector const& GetProperties() const
      { return m_props; }

    void ClearProperties()
      { m_props.clear(); }

    void AddProperty(AdapterProperty const& prop)
    {
      AdapterProperty myProp = prop;
      myProp.SetInterfaceName(GetName());
      m_props.push_back(std::move(myProp));
    }

    AdapterMethod::Vector const& GetMethods() const
      { return m_methods; }

    void ClearMethods()
      { m_methods.clear(); }

    void AddMethod(AdapterMethod const& method)
    {
      AdapterMethod myMethod = method;
      myMethod.SetInterfaceName(GetName());
      m_methods.push_back(std::move(myMethod));
    }

    AdapterSignal::Vector const& GetSignals() const
      { return m_signals; }

    void ClearSignals()
      { m_signals.clear(); }

    void AddSignal(AdapterSignal const& signal)
    {
      AdapterSignal mySignal = signal;
      mySignal.SetInterfaceName(GetName());
      m_signals.push_back(std::move(mySignal));
    }

  private:
    AdapterProperty::Vector m_props;
    AdapterMethod::Vector   m_methods;
    AdapterSignal::Vector   m_signals;
  };
}
