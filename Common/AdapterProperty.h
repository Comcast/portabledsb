#pragma once

#include <atomic>
#include <string>

#include "Common/AdapterObject.h"
#include "Common/AdapterNamedValue.h"
#include "Common/AccessType.h"
#include "Common/SignalBehavior.h"
#include "Common/TypeId.h"

namespace adapter
{
  class Interface;

  class Property : public adapter::Object
  {
    friend class Interface;

  public:
    using Vector = std::vector<Property>;

    Property(std::string const& name, TypeId type, AccessType access);

    inline std::string const& GetInterfaceName() const
      { return m_interface; }

    inline TypeId GetType() const
      { return m_type; }

    inline AccessType GetAccess() const
      { return m_access; }

    inline void SetAccess(AccessType access)
      { m_access = access; }

    inline SignalBehavior GetChangeOfValueBehavior() const
      { return m_changeOfValueBehavior; }

    inline void SetGetChangeOfValueBehavior(SignalBehavior changeOfValueBehavior)
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

  Property MakeProperty(std::string const& name, TypeId type,
    AccessType access, NamedValue::Vector const& attrs = NamedValue::Vector());
}
