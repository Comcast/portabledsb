#include "AdapterNamedValue.h"

using namespace adapter;

NamedValue::NamedValue(std::string const& name, Value const& value)
  : m_name(name)
  , m_value(value)
{
}
