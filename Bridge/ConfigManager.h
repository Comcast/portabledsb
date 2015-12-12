#pragma once

#include "Common/defines.h"
#include "Bridge/BridgeConfig.h"

#include <alljoyn/BusAttachment.h>
#include <alljoyn/BusListener.h>
#include <alljoyn/SessionListener.h>
#include <alljoyn/SessionPortListener.h>

namespace bridge
{
  class IAdapter;
  class DeviceSystemBridge;

  class ConfigManager
  {
  public:
    ConfigManager();
    ~ConfigManager();

    int32_t Initialize(shared_ptr<DeviceSystemBridge> const& bridge);
    int32_t ConnectToAllJoyn(shared_ptr<IAdapter> const& adapter);
    int32_t Shutdown();

    bool IsConfigurationAccessSecured();
    bool IsDeviceAccessSecured();
    bool GetObjectConfigItem(shared_ptr<IAdapter> const& adapter);
    void ToFile();

    shared_ptr<BridgeConfig> GetBridgeConfig();

  private:
    int32_t ShutdownAllJoyn();
    int32_t InitializeCSPBusObjects();
    int32_t BuildServiceName();

    class BusListener : public ajn::BusListener
    {
    };

    class SessionListener : public ajn::SessionListener
    {
    };

    class SessionPortListener : public ajn::SessionPortListener
    {
    };

  private:
    shared_ptr<DeviceSystemBridge>      m_parent;
    shared_ptr<IAdapter>                m_adapter;
    std::auto_ptr<ajn::BusAttachment>   m_busAttachment;
    BusListener                         m_busListener;
    SessionListener                     m_sessionListener;
    SessionPortListener                 m_sessionPortListener;
    std::string                         m_serviceName;

    // BridgeAuthHandler
    BridgeConfig                        m_bridgeConfig;
  };
};

