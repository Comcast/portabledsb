#pragma once

#include <atomic>
#include <string>

#include "Common/AdapterObject.h"
#include "Common/AdapterValue.h"
#include "Common/AccessType.h"
#include "Common/SignalBehavior.h"
#include "Common/TypeId.h"

namespace common
{
  class AdapterInterface;

  class AdapterProperty : public AdapterObject
  {
    friend class AdapterInterface;

  public:
    typedef std::vector<AdapterProperty> Vector;

    AdapterProperty(std::string const& name, TypeId type, AccessType access) 
      : AdapterObject(name)
      , m_type(type)
      , m_access(access)
    {
    }

    std::string const& GetInterfaceName() const
      { return m_interface; }

    TypeId GetType() const
      { return m_type; }

    AccessType GetAccess() const
      { return m_access; }

    void SetAccess(AccessType access)
      { m_access = access; }

    SignalBehavior GetChangeOfValueBehavior() const
      { return m_changeOfValueBehavior; }

    void SetGetChangeOfValueBehavior(SignalBehavior changeOfValueBehavior)
      { m_changeOfValueBehavior = changeOfValueBehavior; }

  private:
    void SetInterfaceName(std::string const& name)
      { m_interface = name; }

  private:
    std::string           m_interface;
    TypeId                m_type;
    AccessType            m_access;
    SignalBehavior        m_changeOfValueBehavior;
  };

  inline AdapterProperty MakeAdapterProperty(std::string const& name, TypeId type,
    AccessType access, AdapterValue::Vector const& attrs = AdapterValue::Vector())
  {
    common::AdapterProperty prop(name, type, access);
    for (auto const& attr : attrs)
      prop.AddAttribute(attr.GetName(), attr.GetValue());
    return std::move(prop);
  }
}
