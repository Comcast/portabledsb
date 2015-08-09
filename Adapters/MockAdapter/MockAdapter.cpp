#include "Adapters/MockAdapter/MockAdapter.h"
#include "Adapters/MockAdapter/MockDevices.h"
#include "Adapters/MockAdapter/MockAdapterDevice.h"

#include <vector>
#include <algorithm>

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


int32_t AdapterLib::MockAdapter::Initialize()
{
  CreateMockDevices();
  CreateSignals();
  return ER_OK;
}


int32_t AdapterLib::MockAdapter::Shutdown()
{
  return ER_NOT_IMPLEMENTED;
}


int32_t AdapterLib::MockAdapter::EnumDevices(
    Bridge::EnumDeviceOptions /*opts*/,
    Bridge::AdapterDeviceVector& deviceList,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  if (req)
    *req = NULL;

  deviceList.clear();
  std::copy(m_devices.begin(), m_devices.end(), std::back_inserter(deviceList));

  return ER_OK;
}


int32_t AdapterLib::MockAdapter::GetProperty(
    shared_ptr<Bridge::IAdapterProperty>& prop,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  if (req)
    *req = NULL;

  shared_ptr<MockAdapterProperty> mockAdapterProperty = dynamic_pointer_cast<MockAdapterProperty>(prop);
  if (!mockAdapterProperty)
    return ER_BAD_ARG_1;

  shared_ptr<MockAdapterDevice> device = mockAdapterProperty->GetParent();
  if (!device)
    return ER_INVALID_ADDRESS;

  shared_ptr<MockAdapter> adapter = device->GetParent();
  if (!adapter)
    return ER_INVALID_ADDRESS;

  // TODO: set prop with latest stuff
  
  return ER_OK;
}


int32_t AdapterLib::MockAdapter::SetProperty(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  if (req)
    *req = NULL;

  shared_ptr<MockAdapterProperty> mockAdapterProperty = dynamic_pointer_cast<MockAdapterProperty>(prop);
  if (!mockAdapterProperty)
    return ER_BAD_ARG_1;

  shared_ptr<MockAdapterDevice> device = mockAdapterProperty->GetParent();
  if (!device)
    return ER_INVALID_ADDRESS;

  shared_ptr<MockAdapter> adapter = device->GetParent();
  if (!adapter)
    return ER_INVALID_ADDRESS;

  // TODO: set internal state from prop->GetData()
  
  return ER_OK;
}


int32_t AdapterLib::MockAdapter::GetPropertyValue(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    std::string const& attributeName,
    shared_ptr<Bridge::IAdapterValue>& value,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  if (req)
    *req = NULL;

  shared_ptr<MockAdapterProperty> mockAdapterProperty = dynamic_pointer_cast<MockAdapterProperty>(prop);
  if (!mockAdapterProperty)
    return ER_BAD_ARG_1;

  shared_ptr<MockAdapterValue> attr = mockAdapterProperty->GetAttributeByName(attributeName);
  if (!attr)
    return ER_INVALID_ADDRESS;

  value = dynamic_pointer_cast<Bridge::IAdapterValue>(attr);

  return ER_OK;
}


int32_t AdapterLib::MockAdapter::SetPropertyValue(
    shared_ptr<Bridge::IAdapterProperty> const& prop,
    shared_ptr<Bridge::IAdapterValue> const& value,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  if (req)
    *req = NULL;

  shared_ptr<MockAdapterProperty> mockAdapterProperty = dynamic_pointer_cast<MockAdapterProperty>(prop);
  if (!mockAdapterProperty)
    return ER_BAD_ARG_1;

  shared_ptr<MockAdapterValue> attr = mockAdapterProperty->GetAttributeByName(value->GetName());
  if (!attr)
    return ER_INVALID_ADDRESS;

  attr->SetData(value->GetData());

  return ER_OK;
}


int32_t AdapterLib::MockAdapter::CallMethod(
    shared_ptr<Bridge::IAdapterMethod>& method,
    shared_ptr<Bridge::IAdapterIoRequest>* req)
{
  shared_ptr<MockAdapterMethod> m = dynamic_pointer_cast<MockAdapterMethod>(method);
  if (!m)
    return ER_BAD_ARG_1;

  shared_ptr<MockAdapterDevice> device = m->GetParent();
  if (!device)
    return ER_INVALID_ADDRESS;

  return device->DispatchMethod(m, req);
}


int32_t AdapterLib::MockAdapter::RegisterSignalListener(
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

int32_t AdapterLib::MockAdapter::UnregisterSignalListener(Bridge::IAdapter::RegistrationHandle const& h)
{
  int32_t st = ER_FAIL;
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
}

void AdapterLib::MockAdapter::CreateMockDevices()
{
  shared_ptr<MockAdapter> self = dynamic_pointer_cast<MockAdapter>(shared_from_this());

  typedef std::vector<AdapterLib::MockDeviceDescriptor> vector;

  vector devices = AdapterLib::GetMockDevices();
  for (vector::const_iterator begin = devices.begin(), end = devices.end();
    begin != end; ++begin)
  {
    shared_ptr<MockAdapterDevice> dev = MockAdapterDevice::Create(*begin, self);
    m_devices.push_back(dev);
  }
}

void AdapterLib::MockAdapter::CreateSignals()
{
  shared_ptr<MockAdapterDevice> parent;

  Bridge::AdapterValueVector values;

  shared_ptr<MockAdapterValue> val(new MockAdapterValue(Bridge::kDeviceArravalHandle));
  shared_ptr<MockAdapterSignal> signal(new MockAdapterSignal(Bridge::kDeviceArrivalSignal,
    parent, values));
  m_signals.push_back(signal);
}

int32_t AdapterLib::MockAdapter::NotifySignalListeners(shared_ptr<MockAdapterSignal> const& signal)
{
  int32_t st = ER_FAIL;
  if (!signal)
    return st;

  std::pair< SignalMap::iterator, SignalMap::iterator > range = 
    m_signalListeners.equal_range(signal->GetName());

  for (SignalMap::iterator begin = range.first, end = range.second; begin != end; ++begin)
  {
    RegisteredSignal& handler = begin->second;
    handler.Listener->AdapterSignalHandler(*signal, handler.Context);
  }

  return st;
}

