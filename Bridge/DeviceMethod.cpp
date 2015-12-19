#include "DeviceMethod.h"

bridge::DeviceMethod::DeviceMethod(DeviceMain& dev)
  : m_parent(dev)
{
}

bridge::DeviceMethod::~DeviceMethod()
{
}

uint32_t
bridge::DeviceMethod::InvokeMethod(ajn::Message const&, ajn::MsgArg*, size_t)
{
  return 0;
}

QStatus
bridge::DeviceMethod::Initialize(shared_ptr<IAdapterMethod> const&)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::DeviceMethod::SetName(std::string const&)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::DeviceMethod::BuildSignature(AdapterValueVector const&, std::string&, std::string&)
{
  return ER_NOT_IMPLEMENTED;
}

