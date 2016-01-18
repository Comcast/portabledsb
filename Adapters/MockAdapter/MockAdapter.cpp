
#include "Adapters/MockAdapter/MockAdapter.h"

#include "Common/AdapterProperty.h"
#include "Common/Log.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

namespace
{
  DSB_DECLARE_LOGNAME(MockAdapter);
  
  common::RegistrationHandle nextHandle = 0;
  common::RegistrationHandle GetNextRegistrationHandle()
  {
    return ++nextHandle;
  }

  std::map< uint64_t, common::AdapterValue > valueCache;
}

adapters::mock::MockAdapter::MockAdapter()
  : m_exposedApplicationName("DeviceSystemBridge")
  , m_exposedApplicationGuid(common::Guid::Parse("C27BC425-0058-4829-8775-441B5D8740C0"))
{
  m_info.SetName("MockAdapter");
  m_info.SetVendor("Comcast");
  m_exposedAdapterPrefix = "com." + m_info.GetVendor();

  // TODO: get m_exposedApplicatioName and Prefix from config
}

adapters::mock::MockAdapter::~MockAdapter()
{
  Shutdown();
}

std::string
adapters::mock::MockAdapter::GetExposedAdapterPrefix()
{
  return m_exposedAdapterPrefix;
}

std::string
adapters::mock::MockAdapter::GetExposedApplicationName()
{
  return m_exposedApplicationName;
}

common::Guid
adapters::mock::MockAdapter::GetExposedApplicationGuid()
{
  return m_exposedApplicationGuid;
}

common::AdapterStatus
adapters::mock::MockAdapter::GetSignals(
  common::AdapterSignal::Vector& signals,
  std::shared_ptr<common::AdapterIoRequest>* req)
{
  DSBLOG_ASSERT_NOT_IMPLEMENTED();
  return 0;
}


common::AdapterStatus
adapters::mock::MockAdapter::Initialize(
  std::shared_ptr<common::Logger> const& log,
  std::shared_ptr<common::AdapterIoRequest>* req)
{
  m_log = log;
  CreateMockDevices();
  CreateSignals();

  if (req && *req)
    (*req)->SetComplete(0);

  return 0;
}


common::AdapterStatus
adapters::mock::MockAdapter::Shutdown(std::shared_ptr<common::AdapterIoRequest>* req)
{
  m_exposedAdapterPrefix.clear();
  m_exposedApplicationName.clear();
  m_devices.clear();
  m_signals.clear();
  m_signalListeners.clear();

  if (req && *req)
    (*req)->SetComplete(0);

  return 0;
}


common::AdapterStatus
adapters::mock::MockAdapter::EnumDevices(
    common::EnumDeviceOptions /*opts*/,
    common::AdapterDevice::Vector& devices,
    std::shared_ptr<common::AdapterIoRequest>* req)
{
  devices.clear();
  std::copy(m_devices.begin(), m_devices.end(), std::back_inserter(devices));

  if (req && *req)
    (*req)->SetComplete(0);

  return 0;
}

common::AdapterStatus
adapters::mock::MockAdapter::GetProperty(
  common::AdapterInterface const& ifc,
  common::AdapterProperty const& prop,
  common::AdapterValue& value,
  std::shared_ptr<common::AdapterIoRequest>* req)
{
  uint64_t id = prop.GetId();

  auto itr = valueCache.find(id);
  if (itr != valueCache.end())
  {
    value = itr->second;
  }
  else
  {
    common::Variant v = prop.GetAttribute("default");
    if (!v.IsEmpty())
    {
      value = common::AdapterValue(prop.GetName(), v);
    }
    else
    {
      // use default for type
      common::TypeId type = prop.GetType();
      value = common::AdapterValue(prop.GetName(), common::Variant(type));
    }
  }

  return 0;
}

common::AdapterStatus
adapters::mock::MockAdapter::SetProperty(
  common::AdapterInterface const& ifc,
  common::AdapterProperty const& prop,
  common::AdapterValue const& value,
  std::shared_ptr<common::AdapterIoRequest>* req)
{
  // TODO: I think we make the bridge do the type checking, but the adapter would
  // have to do range checking
  valueCache[prop.GetId()] = value;
  return 0;
}

common::AdapterStatus
adapters::mock::MockAdapter::InvokeMethod(
  common::AdapterInterface const& ifc,
  common::AdapterMethod const& method,
  std::shared_ptr<common::AdapterIoRequest>* req)
{
  DSBLOG_ASSERT_NOT_IMPLEMENTED();
  return 0;
}


common::AdapterStatus
adapters::mock::MockAdapter::RegisterSignalListener(
    std::string const& signalName,
    common::AdapterSignalListener const& listener,
    void* argp,
    common::RegistrationHandle& handle)
{
  DSBLOG_ASSERT_NOT_IMPLEMENTED();
#if 0
  RegisteredSignal reg;
  reg.Listener = listener;
  reg.Context = argp;
  reg.RegHandle = GetNextRegistrationHandle();
  m_signalListeners.insert(SignalMap::value_type(signalName, reg));
  handle = reg.RegHandle;
#endif
  return 0;
}

common::AdapterStatus
adapters::mock::MockAdapter::UnregisterSignalListener(common::RegistrationHandle const& h)
{
  DSBLOG_ASSERT_NOT_IMPLEMENTED();

  common::AdapterStatus st = 1;
  for (SignalMap::iterator begin = m_signalListeners.begin(), end = m_signalListeners.end();
    begin != end; ++begin)
  {
    if (begin->second.RegHandle == h)
    {
      m_signalListeners.erase(begin);
      st = 0;
      break;
    }
  }
  return st;
}

void
adapters::mock::MockAdapter::CreateMockDevices()
{
  m_devices.push_back(MockAdapter::CreateDoorWindowSensor());

#if 0
  std::shared_ptr<MockAdapter> self = std::dynamic_pointer_cast<MockAdapter>(shared_from_this());

  typedef std::vector<adapters::mock::MockDeviceDescriptor> vector;

  vector devices = adapters::mock::GetMockDevices();
  for (vector::const_iterator begin = devices.begin(), end = devices.end();
    begin != end; ++begin)
  {
    std::shared_ptr<MockAdapterDevice> dev = MockAdapterDevice::Create(*begin, self);
    m_devices.push_back(dev);
  }
#endif
}

void
adapters::mock::MockAdapter::CreateSignals()
{
  DSBLOG_NOT_IMPLEMENTED();
#if 0
  std::shared_ptr<MockAdapterDevice> parent;

  common::AdapterValue::Vector values;

  // TODO
  // std::shared_ptr<MockAdapterValue> val(new MockAdapterValue(bridge::kDeviceArravalHandle));
  // std::shared_ptr<MockAdapterSignal> signal(new MockAdapterSignal(bridge::kDeviceArrivalSignal,
  //   parent, values));
  // m_signals.push_back(signal);
#endif
}

common::AdapterStatus
adapters::mock::MockAdapter::NotifySignalListeners(common::AdapterSignal const& signal)
{
#if 0
  bridge::AdapterStatus st = 1;
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
  #endif
  return 0;
}

std::string
adapters::mock::MockAdapter::GetStatusText(common::AdapterStatus st)
{
  if (st == 0)
    return std::string("ok");

  std::stringstream buff;
  buff << "MockError:";
  buff << static_cast<uint32_t>(st);
  return buff.str();
}

common::AdapterStatus
adapters::mock::MockAdapter::GetBasicInformation(
    common::AdapterItemInformation& info,
    std::shared_ptr<common::AdapterIoRequest>* req)
{
  info = m_info;
  if (req && *req)
    (*req)->SetComplete(0);
  return 0;
}

common::AdapterStatus
adapters::mock::MockAdapter::GetConfiguration(
    std::vector<uint8_t>& configData,
    std::shared_ptr<common::AdapterIoRequest>* req)
{
  if (req && *req)
    (*req)->SetComplete(0);

  return 0;
}

common::AdapterStatus
adapters::mock::MockAdapter::SetConfiguration(
    std::vector<uint8_t> const& configData,
    std::shared_ptr<common::AdapterIoRequest>* req)
{
  if (req && *req)
    (*req)->SetComplete(0);

  return 0;
}

