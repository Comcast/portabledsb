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
    BridgeDevice(adapter::Device&, const std::shared_ptr<adapter::Adapter>&);
    virtual ~BridgeDevice();

    QStatus Shutdown();
    QStatus Initialize();

    ajn::BusAttachment& GetBusAttachment()
    {
      return m_busAttachment;
    }

    adapter::Device const& GetAdapterDevice() const
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

    adapter::Device m_device;
    std::shared_ptr<adapter::Adapter> m_adapter;
    ajn::BusAttachment m_busAttachment;
    DeviceMain m_deviceMain;
    AllJoynAbout m_about;
    std::string m_rootStringForAllJoynNames;
    std::string m_serviceName;
  };
}

