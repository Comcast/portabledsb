#include "AdapterMethod.h"

using namespace adapter;

Method
adapter::MakeMethod(std::string const& name, NamedValue::Vector const& attrs, std::string const& description,
  NamedValue::Vector const& inputArgs, NamedValue::Vector const& outputArgs)
{
  Method method(name);
  for (auto attr : attrs)
    method.AddAttribute(attr.GetName(), attr.GetValue());
  for (auto in : inputArgs)
    method.AddInputArgument(in);
  for (auto out : outputArgs)
    method.AddOutputArgument(out);
  method.SetDescription(description);
  return std::move(method);
}

