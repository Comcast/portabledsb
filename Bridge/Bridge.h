#pragma once

#include "Common/defines.h"
#include "Bridge/IAdapter.h"

#include <alljoyn/Status.h>

#include <map>
#include <string>

namespace Bridge
{
  class IAdapter;
  class BridgeDevice;

  typedef std::map<int, shared_ptr<BridgeDevice> > BridgeDeviceList;

  class DeviceSystemBridge : public enable_shared_from_this<DeviceSystemBridge>
  {
  public:
    static DeviceSystemBridge& Instance();

    // TODO: really should be scoped_ptr as onwership is transferred
    DeviceSystemBridge(shared_ptr<IAdapter> const& adapter);

    virtual ~DeviceSystemBridge();

    QStatus Initialize();
    QStatus Shutdown();

    inline shared_ptr<IAdapter> GetAdapter() const
      { return m_adapter; }

    //inline ConfigManager *GetConfigManager()
    //  { return &m_configManager; }

    QStatus InitializeDevices(bool isUpdate = false);

    inline BridgeDeviceList GetDeviceList() const
      { return m_deviceList; }

  private:
    QStatus RegisterAdapterSignalHandlers(bool isRegister);
    QStatus InitializeAdapter();
    QStatus InitializeInternal();
    QStatus ShutdownInternal();

    QStatus CreateDevice(shared_ptr<IAdapterDevice> const& device);
    QStatus UpdateDevice(shared_ptr<IAdapterDevice> const& device, bool exposedOnAllJoynBus);

    void OnAdapterSignal(IAdapterSignal const& signal, void* argp);

  private:
    class AdapterSignalListener : public IAdapterSignalListener
    {
    public:
      AdapterSignalListener(shared_ptr<DeviceSystemBridge> const& parent)
        : m_parent(parent) { }

      virtual void AdapterSignalHandler(IAdapterSignal const& signal, void* argp)
      {
        shared_ptr<DeviceSystemBridge> dsb = m_parent.lock();
        if (dsb)
          dsb->OnAdapterSignal(signal, argp);
      }
      
    private:
      weak_ptr<DeviceSystemBridge> m_parent;
    };

  private:
    bool                              m_alljoynInitialized;
    shared_ptr<IAdapter>              m_adapter;
    BridgeDeviceList                  m_deviceList;
    shared_ptr<AdapterSignalListener> m_adapterSignaListener;
  };
}

