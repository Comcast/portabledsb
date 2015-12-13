#include "DeviceMethod.h"

bridge::DeviceMethod::DeviceMethod()
{
}

bridge::DeviceMethod::~DeviceMethod()
{
}

uint32_t
bridge::DeviceMethod::InvokeMethod(
  ajn::Message const&   msg,
  ajn::MsgArg*          outargs,
  size_t                numOutArgs)
{
  return 0;
}

QStatus
bridge::DeviceMethod::Initialize(shared_ptr<DeviceMain> const& parent, shared_ptr<IAdapterMethod> const& adapterMethod)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::DeviceMethod::SetName(std::string const& name)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::DeviceMethod::BuildSignature(
  AdapterValueVector const&     valueList,
  std::string&                  signature,
  std::string&                  parameterNames)
{
  return ER_NOT_IMPLEMENTED;
}

