#ifndef __DEVICE_MAIN_H__
#define __DEVICE_MAIN_H__

#include "BridgeDevice.h"
#include <map>

#include <alljoyn/BusObject.h>
#include <alljoyn/InterfaceDescription.h>

namespace bridge
{
  class DeviceMethod;
  class DeviceSignal;

  class DeviceMain
  {
    DeviceMain();
    virtual ~DeviceMain();

    QStatus Initialize(shared_ptr<BridgeDevice> const& parent);

    bool IsMethodNameUnique(std::string const& name);
    bool IsSignalNameUnique(std::string const& name);
    void HandleSignal(IAdapterSignal const& adapterSignal);

    inline shared_ptr<ajn::BusObject> GetBusObject() const
      { return m_busObject; }

    inline int GetIndexForMethod()
      { return m_indexForMethod++; }

    inline std::string GetBusObjectPath(IAdapterProperty const& prop)
      { return std::string(); }

  private:
    void Shutdown();

  private:
    int                                         m_indexForSignal;
    int                                         m_indexForMethod;
    shared_ptr<ajn::BusObject>                  m_busObject;
    std::unique_ptr<ajn::InterfaceDescription>  m_interfaceDescrtipion;
    std::string                                 m_busObjectPath;
    std::string                                 m_interfaceName;
    std::shared_ptr<BridgeDevice>               m_parent;
    std::map<std::string, DeviceMethod* >       m_deviceMethods;
    std::map<std::string, DeviceSignal* >       m_deviceSignals;
    bool                                        m_registeredOnAllJoyn;
  };
}

#endif

