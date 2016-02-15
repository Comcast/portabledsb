#pragma once

#include "Common/AdapterMethod.h"
#include "Common/AdapterObject.h"
#include "Common/AdapterProperty.h"
#include "Common/AdapterSignal.h"

#include <algorithm>
#include <list>
#include <vector>

namespace adapter
{
  class Interface : public adapter::Object
  {
  public:
    typedef std::vector<Interface> Vector;

    Interface(std::string const& name);

    // properties
    void AddProperty(Property prop);

    inline void ClearProperties()
      { m_props.clear(); }

    inline Property::Vector const& GetProperties() const
      { return m_props; }

    Property const* FindProperty(char const* name) const;


    // methods
    void AddMethod(Method method);

    inline void ClearMethods()
      { m_methods.clear(); }

    inline Method::Vector const& GetMethods() const
      { return m_methods; }

    Method const* FindMethod(char const* name) const;


    // signals
    void AddSignal(Signal signal);

    inline void ClearSignals()
      { m_signals.clear(); }

    inline Signal::Vector const& GetSignals() const
      { return m_signals; }


    static std::list< std::shared_ptr<Interface> > FromFile(std::string const& file);

  private:
    template<typename T>
    void TakeOwnership(std::vector<T>& v, T&& item);

  private:
    Property::Vector m_props;
    Method::Vector   m_methods;
    Signal::Vector   m_signals;
  };
}
