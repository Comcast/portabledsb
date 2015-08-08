#include "Adapters/MockAdapter/MockAdapter.h"
#include "Adapters/MockAdapter/MockDevices.h"
#include "Adapters/MockAdapter/MockAdapterDevice.h"

namespace
{
  Bridge::IAdapter::RegistrationHandle nextHandle = 0;
  Bridge::IAdapter::RegistrationHandle GetNextRegistrationHandle()
  {
    return ++nextHandle;
  }
}

AdapterLib::MockAdapter::MockAdapter()
  : m_vendor("Acme")
  , m_adapterName("DSB Mock Adapter")
  , m_exposedAdapterPrefix("com." + m_vendor)
  , m_exposedApplicationName("DeviceSystemBridge")
  , m_exposedApplicationGuid("C27BC425-0058-4829-8775-441B5D8740C0")
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
    std::string const& signalName,
    shared_ptr<Bridge::IAdapterSignalListener> const& listener,
    void* argp,
    Bridge::IAdapter::RegistrationHandle& handle)
{
  RegisteredSignal reg;
  reg.Listener = listener;
  reg.Context = argp;
  reg.RegHandle = GetNextRegistrationHandle();
  m_signalListeners.insert(SignalMap::value_type(signalName, reg));
  handle = reg.RegHandle;
  return ER_OK;
}


QStatus AdapterLib::MockAdapter::UnregisterSignalListener(Bridge::IAdapter::RegistrationHandle const& h)
{
  QStatus st = ER_FAIL;
  for (SignalMap::iterator begin = m_signalListeners.begin(), end = m_signalListeners.end();
    begin != end; ++begin)
  {
    if (begin->second.RegHandle == h)
    {
      m_signalListeners.erase(begin);
      st = ER_OK;
      break;
    }
  }
  return st;

  return ER_NOT_IMPLEMENTED;
}

void AdapterLib::MockAdapter::CreateMockDevices()
{
  typedef std::vector<AdapterLib::MockDeviceDescriptor> vector;

  vector devices = AdapterLib::GetMockDevices();
  for (vector::const_iterator begin = devices.begin(), end = devices.end();
    begin != end; ++begin)
  {
    shared_ptr<MockAdapter> self = shared_from_this();
    shared_ptr<MockAdapterDevice> dev(new MockAdapterDevice(*begin, self));
    // TODO
  }
}

void AdapterLib::MockAdapter::CreateSignals()
{
}

QStatus AdapterLib::MockAdapter::NotifySignalListeners(shared_ptr<MockAdapterSignal> const& signal)
{
  QStatus st = ER_OK;
  return st;
}



