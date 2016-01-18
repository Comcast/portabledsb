#pragma once

#include "AllJoynAbout.h"
#include "DeviceMain.h"
#include "DeviceProperty.h"

#include "Common/Adapter.h"

#include <alljoyn/BusAttachment.h>

namespace bridge
{
  class BridgeDevice : private ajn::SessionPortListener, private ajn::BusListener
  {
  public:
    BridgeDevice(common::AdapterDevice&, const std::shared_ptr<common::Adapter>&);
    virtual ~BridgeDevice();

    QStatus Shutdown();
    QStatus Initialize();

    ajn::BusAttachment& GetBusAttachment()
    {
      return m_busAttachment;
    }

    common::AdapterDevice const& GetAdapterDevice() const
      { return m_device; }

    inline std::string &GetRootNameForInterface()
    {
      return m_rootStringForAllJoynNames;
    }

  private:
    QStatus BuildServiceName();
    QStatus InitializeAllJoyn();
    QStatus CreateDeviceProperties();
    QStatus InitControlPanel();
    QStatus InitLightingService();
    QStatus ConnectToAllJoyn();
    QStatus ShutdownAllJoyn();
    QStatus RegisterSignalHandlers(bool isRegister);

    common::AdapterDevice m_device;
    std::shared_ptr<common::Adapter> m_adapter;
    ajn::BusAttachment m_busAttachment;
    DeviceMain m_deviceMain;
    AllJoynAbout m_about;
    std::string m_rootStringForAllJoynNames;
    std::string m_serviceName;
    std::map<std::string, std::shared_ptr<DeviceProperty>> m_deviceProperties;
  };
}

