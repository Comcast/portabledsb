#include "Adapters/MockAdapter/MockAdapter.h"
#include "Adapters/MockAdapter/MockDevices.h"
#include "Adapters/MockAdapter/MockAdapterDevice.h"

AdapterLib::MockAdapter::MockAdapter()
  : m_vendor("Acme")
  , m_adapterName("DSB Mock Adapter")
  , m_exposedAdapterPrefix("com." + m_vendor)
  , m_exposedApplicationGuid("C27BC425-0058-4829-8775-441B5D8740C0")
  , m_exposedApplicationName("DeviceSystemBridge")
{
  // TODO: get m_exposedApplicatioName and Prefix from config
}

AdapterLib::MockAdapter::~MockAdapter()
{
  CreateMockDevices();
  CreateSignals();
}

std::string AdapterLib::MockAdapter::GetVendor()
{
  return m_vendor;
}

std::string AdapterLib::MockAdapter::GetAdapterName()
{
  return m_adapterName;
}

std::string AdapterLib::MockAdapter::GetVersion()
{
  return m_version;
}

std::string AdapterLib::MockAdapter::GetExposedAdapterPrefix()
{
  return m_exposedAdapterPrefix;
}

std::string AdapterLib::MockAdapter::GetExposedApplicationName()
{
  return m_exposedApplicationName;
}

std::string AdapterLib::MockAdapter::GetExposedApplicationGuid()
{
  return m_exposedApplicationGuid;
}

Bridge::AdapterSignalVector AdapterLib::MockAdapter::GetSignals()
{
  return Bridge::AdapterSignalVector();
}


QStatus AdapterLib::MockAdapter::Initialize()
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::Shutdown()
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::EnumDevices(
    Bridge::EnumDeviceOptions opts,
    Bridge::AdapterDeviceVector& deviceList,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::GetProperty(
    shared_ptr<Bridge::IAdapterProperty>& prop,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::SetProperty(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::GetPropertyValue(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    std::string const& attributeName,
    shared_ptr<Bridge::IAdapterValue>& value,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::SetPropertyValue(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    shared_ptr<Bridge::IAdapterValue> const& value,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::CallMethod(
    shared_ptr<Bridge::IAdapterMethod>& method,
    Bridge::IAdapterIoRequest** req)
{
  shared_ptr<MockAdapterMethod> m = dynamic_pointer_cast<MockAdapterMethod>(method);
  if (!m)
    return ER_BAD_ARG_1;

  shared_ptr<MockAdapterDevice> device = m->GetParent();
  if (!device)
    return ER_INVALID_ADDRESS;

  return device->DispatchMethod(m, req);
}


QStatus AdapterLib::MockAdapter::RegisterSignalListener(
    shared_ptr<Bridge::IAdapterSignal> const& signal,
    shared_ptr<Bridge::IAdapterSignalListener> const& listener,
    void* argp)
{
  return ER_NOT_IMPLEMENTED;
}


QStatus AdapterLib::MockAdapter::UnregisterSignalListener(
    shared_ptr<Bridge::IAdapterSignal> const& signal,
    shared_ptr<Bridge::IAdapterSignalListener> const& listener)
{
  return ER_NOT_IMPLEMENTED;
}

void AdapterLib::MockAdapter::CreateMockDevices()
{
  typedef std::vector<AdapterLib::MockDeviceDescriptor> vector;

  vector devices = AdapterLib::GetMockDevices();
  for (vector::const_iterator begin = devices.begin(), end = devices.end();
    begin != end; ++begin)
  {
    shared_ptr<MockAdapterDevice> dev(new MockAdapterDevice(*begin, *this));
    // TODO
  }
}

void AdapterLib::MockAdapter::CreateSignals()
{
}


