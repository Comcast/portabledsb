#pragma once

#include "Bridge/ConfigManager.h"
#include "Common/Adapter.h"

#include <alljoyn/Status.h>

#include <map>
#include <string>

namespace bridge
{
  class BridgeDevice;

  typedef std::map<common::AdapterDevice*, std::shared_ptr<BridgeDevice> > BridgeDeviceList;

  class DeviceSystemBridge : public std::enable_shared_from_this<DeviceSystemBridge>
  {
  public:
    static std::shared_ptr<DeviceSystemBridge> GetInstance();
    static void InitializeSingleton(std::shared_ptr<common::Adapter> const& adapter);

    virtual ~DeviceSystemBridge();

    QStatus Initialize();
    QStatus Shutdown();

    inline std::shared_ptr<common::Adapter> GetAdapter() const
      { return m_adapter; }

    //inline ConfigManager *GetConfigManager()
    //  { return &m_configManager; }

    QStatus InitializeDevices(bool isUpdate = false);

    inline BridgeDeviceList GetDeviceList() const
      { return m_deviceList; }

  private:
    DeviceSystemBridge(std::shared_ptr<common::Adapter> const& adapter);

    QStatus RegisterAdapterSignalHandlers(bool isRegister);
    QStatus InitializeAdapter();
    QStatus InitializeInternal();
    QStatus ShutdownInternal();

    QStatus CreateDevice(common::AdapterDevice const& device);
    QStatus UpdateDevice(common::AdapterDevice const& device, bool exposedOnAllJoynBus);

    void OnAdapterSignal(common::AdapterSignal const& signal, void* argp);

  private:
    #if 0
    class AdapterSignalListener : public IAdapterSignalListener
    {
    public:
      AdapterSignalListener(DeviceSystemBridge& parent)
        : m_parent(parent)
        , m_shuttingDown(false)
      {
      }

      virtual void AdapterSignalHandler(IAdapterSignal const& signal, void* argp)
      {
        if (m_shuttingDown)
        {
          return;
        }
        m_parent.OnAdapterSignal(signal, argp);
      }

      void Shutdown()
      {
        m_shuttingDown = true;
      }
      
    private:
      DeviceSystemBridge& m_parent;
      bool m_shuttingDown;
    };
    #endif

  private:
    bool                                      m_alljoynInitialized;
    std::shared_ptr<common::Adapter>                      m_adapter;
    BridgeDeviceList                          m_deviceList;
    std::vector<common::RegistrationHandle> m_registeredSignalListeners;
    ConfigManager m_configManager;
  };
}

