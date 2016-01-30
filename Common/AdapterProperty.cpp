#include "AdapterProperty.h"

using namespace adapter;

Property::Property(std::string const& name, TypeId type, AccessType access) 
  : adapter::Object(name)
  , m_type(type)
  , m_access(access)
{
}

Property
adapter::MakeProperty(std::string const& name, TypeId type, AccessType access, NamedValue::Vector const& attrs)
{
  Property prop(name, type, access);
  for (auto const& attr : attrs)
    prop.AddAttribute(attr.GetName(), attr.GetValue());
  return std::move(prop);
}
