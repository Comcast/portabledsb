#include "AdapterSignal.h"

using namespace adapter;

Signal::Signal(std::string const& name)
  : Object(name)
{
}

Signal
adapter::MakeAdapterSignal(std::string const& name, NamedValue::Vector const& params)
{
  Signal signal(name);
  for (auto const& param : params)
    signal.AddParameter(param);
  return std::move(signal);
}
