#include "Bridge/Bridge.h"
#include "Bridge/BridgeDevice.h"
#include "Common/AdapterValue.h"
#include "Common/EnumDeviceOptions.h"
#include "Common/Log.h"

#include <qcc/Debug.h>
#include <alljoyn/Init.h>

namespace
{
  DSB_DECLARE_LOGNAME(DeviceSystemBridge);

  int32_t const kWaitTimeoutForAdapterOperation = 20000; //ms
  std::string const kDeviceArrivalSignal = "Device_Arrival";
  std::string const kDeviceArrivalDeviceHandle = "Device_Handle";
  std::string const kDeviceRemovalSignal = "Device_Removal";
  std::string const kDeviceRemovalDeviceHandle = "Device_Handle";

  bridge::BridgeDeviceList::key_type GetKey(common::AdapterDevice const& dev)
  {
    // return dev.get();
    return 0;
  }

  void alljoynLogger(DbgMsgType type, char const* module, char const* msg, void* /*ctx*/)
  {
    static std::string logName = "alljoyn";
    
    common::LogLevel level = common::LogLevel::Info;
    switch (type)
    {
      case DBG_LOCAL_ERROR:
      case DBG_REMOTE_ERROR:
        level = common::LogLevel::Error;
        break;
      case DBG_HIGH_LEVEL:
        level = common::LogLevel::Warn;
        break;
      case DBG_GEN_MESSAGE:
        level = common::LogLevel::Info;
        break;
      case DBG_API_TRACE:
      case DBG_REMOTE_DATA:
      case DBG_LOCAL_DATA:
        level = common::LogLevel::Debug;
    }

    common::Logger::GetLogger(logName)->Write(level, NULL, 0, "[%s] %s", module, msg);
  }

  void RegisterAllJoynLogger()
  {
    static bool alljoynLoggerRegistered = false;
    if (!alljoynLoggerRegistered)
    {
      alljoynLoggerRegistered = true;
      QCC_RegisterOutputCallback(alljoynLogger, NULL);
    }
  }

  std::shared_ptr<bridge::DeviceSystemBridge> gBridge;
}

std::shared_ptr<bridge::DeviceSystemBridge>
bridge::DeviceSystemBridge::GetInstance()
{
  return gBridge;
}

void
bridge::DeviceSystemBridge::InitializeSingleton(std::shared_ptr<common::Adapter> const& adapter)
{
  gBridge.reset(new DeviceSystemBridge(adapter));
}

bridge::DeviceSystemBridge::DeviceSystemBridge(std::shared_ptr<common::Adapter> const& adapter)
  : m_alljoynInitialized(false)
  , m_adapter(adapter)
  , m_configManager(*this, *adapter)
{
  // TODO: bug in alljoyn prevents registering logger.
  // RegisterAllJoynLogger();
}

bridge::DeviceSystemBridge::~DeviceSystemBridge()
{
  Shutdown();
  m_adapter.reset();
}

QStatus
bridge::DeviceSystemBridge::Initialize()
{
  QStatus st = ER_OK;

  DSBLOG_DEBUG(__FUNCTION__);
  if (!m_alljoynInitialized)
  {
    DSBLOG_INFO("initialize AllJoyn");
    st = AllJoynInit();
    if (st != ER_OK)
    {
      DSBLOG_WARN("initialize AllJoyn failed: %s", QCC_StatusText(st));
      return st;
    }

    m_alljoynInitialized = true;
  }

  st = InitializeInternal();
  if (st != ER_OK)
    m_alljoynInitialized = false;

  return st;
}

QStatus
bridge::DeviceSystemBridge::InitializeInternal()
{
  QStatus st;

  DSBLOG_INFO("initialize configuration manager");
  st = m_configManager.Initialize();

  if (st != ER_OK)
  {
    DSBLOG_WARN("initialize configuration manager failed: %s", QCC_StatusText(st));
    return st;
  }

  DSBLOG_INFO("initialize adapter");
  st = InitializeAdapter();

  if (st != ER_OK)
  {
    DSBLOG_WARN("initialize adapter failed: %s", QCC_StatusText(st));
    return st;
  }

  DSBLOG_INFO("connect to AllJoyn router");
  st = m_configManager.ConnectToAllJoyn();

  if (st != ER_OK)
  {
    DSBLOG_INFO("connect to AllJoyn router failed: %s", QCC_StatusText(st));
    return st;
  }

  DSBLOG_INFO("initialize devices");
  st = InitializeDevices();
  if (st != ER_OK)
  {
    DSBLOG_WARN("initialize devices failed: %s", QCC_StatusText(st));
    return st;
  }

  DSBLOG_INFO("registering signal handlers");
  st = RegisterAdapterSignalHandlers(true);
  if (st != ER_OK)
  {
    DSBLOG_WARN("register signal handlers failed: %s", QCC_StatusText(st));
    return st;
  }

  return st;
}

QStatus
bridge::DeviceSystemBridge::Shutdown()
{
  QStatus st = ER_OK;

  st = ShutdownInternal();
  if (st != ER_OK)
  {
    DSBLOG_WARN("failed to shutdown internal: %s", QCC_StatusText(st));
    return st;
  }

  if (m_alljoynInitialized)
  {
    AllJoynShutdown();
    m_alljoynInitialized = false;
  }

  return st;
}

QStatus
bridge::DeviceSystemBridge::InitializeAdapter()
{
  if (!m_adapter)
  {
    DSBLOG_ERROR("can't initialize null adapter");
    return ER_FAIL;
  }

  common::AdapterItemInformation info;
  m_adapter->GetBasicInformation(info);

  std::shared_ptr<common::Logger> log = common::Logger::GetLogger(info.GetName());
  int ret = m_adapter->Initialize(log);
  return ret == 0 ? ER_OK : ER_FAIL;
}

QStatus
bridge::DeviceSystemBridge::InitializeDevices(bool update)
{
  common::AdapterDevice::Vector deviceList;
  std::shared_ptr<common::AdapterIoRequest> request;

  common::EnumDeviceOptions opts = common::EnumDeviceOptions::CacheOnly;
  if (update)
    opts = common::EnumDeviceOptions::ForceRefresh;

  AdapterStatus adapterStatus  = m_adapter->EnumDevices(opts, deviceList, &request);
  if (!IsOk(adapterStatus))
  {
    DSBLOG_WARN("failed to enumerate devices: %d", m_adapter->GetStatusText(adapterStatus).c_str());

    // TODO: we need some translation from AdapterStatus to QStatus
    return ER_FAIL;
  }

  if (request)
  {
    if (!request->Wait(kWaitTimeoutForAdapterOperation))
    {
      DSBLOG_WARN("timeout waiting for async i/o request");
      return ER_FAIL;
    }
  }

  typedef common::AdapterDevice::Vector::iterator iterator;
  for (iterator begin = deviceList.begin(), end = deviceList.end(); begin != end; ++begin)
  {
    // TODO: get configuration for device, only expose visible devices
    QStatus st;
    if (update)
      st = UpdateDevice(*begin, true);
    else
      st = CreateDevice(*begin);

    if (st != ER_OK)
    {
      DSBLOG_WARN("Failed to %s device: %s", update ? "update" : "create", QCC_StatusText(st));
      return st;
    }
  }

  // TODO: Save bridge configuration to XML

  return ER_OK;
}

void
bridge::DeviceSystemBridge::OnAdapterSignal(common::AdapterSignal const& signal, void*)
{
  #if 0
  // TODO
  std::shared_ptr<IAdapterDevice> adapterDevice;

  std::string const name = signal.GetName();
  if (name == kDeviceArrivalSignal || name == kDeviceRemovalSignal)
  {
    common::AdapterValue::Vector params = signal.GetParams();
    for (auto const& param: params)
    {
      std::string const& paramName = param.GetName();
      if (paramName == kDeviceArrivalDeviceHandle || paramName == kDeviceRemovalDeviceHandle)
      {
        // TODO: figure out where this signal is generated and figure out how we're 
        // going to put an arbitrary pointer inside.
        //
        // adapterDevice = param->GetData();
      }

      if (!adapterDevice)
        return;
    }
  }

  if (name == kDeviceArrivalSignal)
    CreateDevice(adapterDevice);

  else if (name == kDeviceRemovalSignal)
    UpdateDevice(adapterDevice, false);
  #endif
}

QStatus
bridge::DeviceSystemBridge::RegisterAdapterSignalHandlers(bool isRegister)
{
  QStatus st = ER_OK;

  if (isRegister)
  {
      #if 0
    common::AdapterSignal::Vector const& signals = m_adapter->GetSignals();
    for (auto const& signal : signals)
    {
        Adapter::RegistrationHandle handle;
        int ret = m_adapter->RegisterSignalListener(signal.GetName(), m_adapterSignalListener, NULL, handle);
        if (ret != 0)
        {
          DSBLOG_WARN("failed to register signal listener on adapter: %s", QCC_StatusText(st));
          if (st == ER_OK)
            ret = st;
        }
        else
        {
          m_registeredSignalListeners.push_back(handle);
        }
      }
      #endif
  }
  else
  {
    typedef std::vector<common::RegistrationHandle>::const_iterator iterator;
    for (iterator begin = m_registeredSignalListeners.begin(), end = m_registeredSignalListeners.end();
      begin != end; ++begin)
    {
      int ret = m_adapter->UnregisterSignalListener(*begin);
      if (ret != 0)
      {
        DSBLOG_WARN("failed to unregister signal listener on adapter: %s", QCC_StatusText(st));
        if (st == ER_OK)
          st = ER_FAIL;
      }
    }
  }

  return st;
}

QStatus
bridge::DeviceSystemBridge::ShutdownInternal()
{
  m_configManager.Shutdown();

  if (m_adapter)
  {
    RegisterAdapterSignalHandlers(false);
    m_adapter->Shutdown();
  }

  for (BridgeDeviceList::iterator itr = m_deviceList.begin(); itr != m_deviceList.end(); ++itr)
    itr->second->Shutdown();

  m_deviceList.clear();

  return ER_OK;
}

QStatus
bridge::DeviceSystemBridge::UpdateDevice(common::AdapterDevice const& dev, bool exposedOnAllJoynBus)
{
  QStatus st = ER_OK;

  BridgeDeviceList::const_iterator itr = m_deviceList.find(GetKey(dev));
  if (itr == m_deviceList.end() && exposedOnAllJoynBus)
  {
    st = CreateDevice(dev);
  }
  else if (itr != m_deviceList.end() && !exposedOnAllJoynBus)
  {
    m_deviceList.erase(itr);
    if ((st = itr->second->Shutdown()) != ER_OK)
      DSBLOG_WARN("failed to shutdown BridgeDevice: %s", QCC_StatusText(st));
  }

  return st;
}

QStatus
bridge::DeviceSystemBridge::CreateDevice(common::AdapterDevice const& dev)
{
#if 0
  std::shared_ptr<BridgeDevice> newDevice(new BridgeDevice(dev, m_adapter));
  
  QStatus st = newDevice->Initialize();
  if (st == ER_OK)
    m_deviceList.insert(std::make_pair(GetKey(dev), newDevice));

  return st;
#endif
  return ER_OK;
}
