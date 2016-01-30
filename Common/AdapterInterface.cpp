#include "AdapterInterface.h"

using namespace adapter;

namespace
{
  template<typename T>
  T const* FindByName(std::vector<T> const& v, char const* name)
  {
    T const* item = nullptr;

    auto itr = std::find_if(v.begin(), v.end(), 
      [&name](T const& obj) { return strcmp(obj.GetName().c_str(), name) == 0; });

    if (itr != v.end())
      item = &(*itr);

    return item;
  }
}

template<typename T>
void Interface::TakeOwnership(std::vector<T>& v, T&& item)
{
  item.SetInterfaceName(GetName());
  v.push_back(std::move(item));
}

Interface::Interface(std::string const& name)
  : Object(name)
{
}

Property const*
Interface::FindProperty(char const* name) const
{
  return FindByName(m_props, name);
}

void
Interface::AddProperty(Property prop)
{
  TakeOwnership(m_props, std::move(prop));
}

Method const*
Interface::FindMethod(char const* name) const
{
  return FindByName(m_methods, name);
}

void
Interface::AddMethod(Method method)
{
  TakeOwnership(m_methods, std::move(method));
}

void
Interface::AddSignal(Signal signal)
{
  TakeOwnership(m_signals, std::move(signal));
}

std::list< std::shared_ptr<Interface> >
Interface::FromFile(std::string const& file)
{
  // TODO:
  return std::list< std::shared_ptr<Interface> >();
}
