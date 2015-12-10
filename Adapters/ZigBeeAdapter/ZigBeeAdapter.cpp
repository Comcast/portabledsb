#include "Adapters/ZigBeeAdapter/ZigBeeAdapter.h"

namespace
{
  std::string const kVendor("Charlie Root");
  std::string const kAdapterName("ZigBee");
  std::string const kVersion("1.0");
  std::string const kExposedPrefix("com.allseen");
  std::string const kExposedAppName("The ZigBee Adapter");
  std::string const kExposedGuid("B8D50823-9F64-4110-AA0C-3CB3D17B73F2");
}


AdapterLib::ZigBeeAdapter::ZigBeeAdapter()
{
}

AdapterLib::ZigBeeAdapter::~ZigBeeAdapter()
{
}

std::string AdapterLib::ZigBeeAdapter::GetVendor()
{
  return kVendor;
}

std::string AdapterLib::ZigBeeAdapter::GetAdapterName()
{
  return kAdapterName;
}

std::string AdapterLib::ZigBeeAdapter::GetVersion()
{
  return kVersion;
}

std::string AdapterLib::ZigBeeAdapter::GetExposedAdapterPrefix()
{
  return kExposedPrefix;
}

std::string AdapterLib::ZigBeeAdapter::GetExposedApplicationName()
{
  return kExposedAppName;
}

std::string AdapterLib::ZigBeeAdapter::GetExposedApplicationGuid()
{
  return kExposedGuid;
}

Bridge::AdapterSignalVector AdapterLib::ZigBeeAdapter::GetSignals()
{
  // TODO: wouldn't this list be per device?
  return Bridge::AdapterSignalVector();
}


int32_t AdapterLib::ZigBeeAdapter::Initialize()
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::Shutdown()
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::EnumDevices(
    Bridge::EnumDeviceOptions opts,
    Bridge::AdapterDeviceVector& deviceList,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::GetProperty(
    shared_ptr<Bridge::IAdapterProperty>& prop,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::SetProperty(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::GetPropertyValue(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    std::string const& attributeName,
    shared_ptr<Bridge::IAdapterValue>& value,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::SetPropertyValue(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    shared_ptr<Bridge::IAdapterValue> const& value,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::CallMethod(
    shared_ptr<Bridge::IAdapterMethod>& method,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::RegisterSignalListener(
  std::string const& signalName,
  shared_ptr<Bridge::IAdapterSignalListener> const& listener,
  void* argp,
  Bridge::IAdapter::RegistrationHandle& handle)
{
  return -1;
}


int32_t AdapterLib::ZigBeeAdapter::UnregisterSignalListener(
Bridge::IAdapter::RegistrationHandle const& h)
{
  return -1;
}
