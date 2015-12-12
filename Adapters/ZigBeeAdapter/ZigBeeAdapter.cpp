#include "Adapters/ZigBeeAdapter/ZigBeeAdapter.h"

namespace
{
  std::string const kVendor("Charlie Root");
  std::string const kAdapterName("ZigBeeAdapter");
  std::string const kVersion("1.0");
  std::string const kExposedPrefix("com.allseen");
  std::string const kExposedAppName("The ZigBee Adapter");
  std::string const kExposedGuid("B8D50823-9F64-4110-AA0C-3CB3D17B73F2");
}

adapter::zigbee::Adapter::Adapter()
{
}

adapter::zigbee::Adapter::~Adapter()
{
}

std::string
adapter::zigbee::Adapter::GetVendor()
{
  return kVendor;
}

std::string
adapter::zigbee::Adapter::GetAdapterName()
{
  return kAdapterName;
}

std::string
adapter::zigbee::Adapter::GetVersion()
{
  return kVersion;
}

std::string
adapter::zigbee::Adapter::GetExposedAdapterPrefix()
{
  return kExposedPrefix;
}

std::string
adapter::zigbee::Adapter::GetExposedApplicationName()
{
  return kExposedAppName;
}

std::string
adapter::zigbee::Adapter::GetExposedApplicationGuid()
{
  return kExposedGuid;
}

bridge::AdapterSignalVector
adapter::zigbee::Adapter::GetSignals()
{
  // TODO: wouldn't this list be per device?
  return bridge::AdapterSignalVector();
}

int32_t
adapter::zigbee::Adapter::Initialize()
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::Shutdown()
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::EnumDevices(
    bridge::EnumDeviceOptions opts,
    bridge::AdapterDeviceVector& deviceList,
    shared_ptr<bridge::IAdapterIoRequest>* req)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::GetProperty(
    shared_ptr<bridge::IAdapterProperty>& prop,
    shared_ptr<bridge::IAdapterIoRequest>* req)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::SetProperty(
    shared_ptr<bridge::IAdapterProperty> const& prop,
    shared_ptr<bridge::IAdapterIoRequest>* req)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::GetPropertyValue(
    shared_ptr<bridge::IAdapterProperty> const& prop,
    std::string const& attributeName,
    shared_ptr<bridge::IAdapterValue>& value,
    shared_ptr<bridge::IAdapterIoRequest>* req)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::SetPropertyValue(
    shared_ptr<bridge::IAdapterProperty> const& prop,
    shared_ptr<bridge::IAdapterValue> const& value,
    shared_ptr<bridge::IAdapterIoRequest>* req)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::CallMethod(
    shared_ptr<bridge::IAdapterMethod>& method,
    shared_ptr<bridge::IAdapterIoRequest>* req)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::RegisterSignalListener(
  std::string const& signalName,
  shared_ptr<bridge::IAdapterSignalListener> const& listener,
  void* argp,
  bridge::IAdapter::RegistrationHandle& handle)
{
  return -1;
}

int32_t
adapter::zigbee::Adapter::UnregisterSignalListener(bridge::IAdapter::RegistrationHandle const& h)
{
  return -1;
}
