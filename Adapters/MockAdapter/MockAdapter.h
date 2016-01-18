#pragma once

#include "Common/Adapter.h"

#include <map>
#include <memory>

namespace adapters
{
namespace mock
{
  struct InterfaceDefinition
  {
    std::string                     Name;
    common::AdapterProperty::Vector Properties;
    common::AdapterMethod::Vector   Methods;
    common::AdapterSignal::Vector   Signals;
  };

  std::shared_ptr<InterfaceDefinition> GetZigBeeCluster(uint16_t clusterId);

  class MockAdapter : public common::Adapter, public std::enable_shared_from_this<common::Adapter>
  {
  public:
    MockAdapter();
    virtual ~MockAdapter();

    static InterfaceDefinition GetZigbeeInterface(uint16_t clusterId);

    virtual common::AdapterStatus GetBasicInformation(
      common::AdapterItemInformation& info,
      std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual std::string GetExposedAdapterPrefix();
    virtual std::string GetExposedApplicationName();
    virtual common::Guid GetExposedApplicationGuid();

    virtual common::AdapterStatus GetSignals(
        common::AdapterSignal::Vector& signals,
        std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual std::string GetStatusText(common::AdapterStatus st);

    virtual common::AdapterStatus Initialize(
        std::shared_ptr<common::Logger> const& log,
        std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus Shutdown(
        std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus GetConfiguration(
        std::vector<uint8_t>& configData,
        std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus SetConfiguration(
        std::vector<uint8_t> const& configData,
        std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus EnumDevices(
      common::EnumDeviceOptions opts,
      common::AdapterDevice::Vector& devices,
      std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus GetProperty(
      common::AdapterInterface const& ifc,
      common::AdapterProperty const& prop,
      common::AdapterValue& value,
      std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus SetProperty(
      common::AdapterInterface const& ifc,
      common::AdapterProperty const& prop, 
      common::AdapterValue const& value,
      std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus InvokeMethod(
      common::AdapterInterface const& ifc,
      common::AdapterMethod const& method,
      std::shared_ptr<common::AdapterIoRequest>* req = NULL);

    virtual common::AdapterStatus RegisterSignalListener(
      std::string const& signalName,
      common::AdapterSignalListener const& listener,
      void* argp,
      common::RegistrationHandle& handle);

    virtual common::AdapterStatus UnregisterSignalListener(common::RegistrationHandle const& h);

    common::AdapterStatus NotifySignalListeners(common::AdapterSignal const& signal);

  private:
    void CreateMockDevices();
    void CreateSignals();

    common::AdapterDevice CreateDoorWindowSensor();

  private:
    common::AdapterItemInformation m_info;

    std::string m_exposedAdapterPrefix;
    std::string m_exposedApplicationName;
    common::Guid m_exposedApplicationGuid;
    std::shared_ptr<common::Logger> m_log;

    common::AdapterDevice::Vector m_devices;
    common::AdapterSignal::Vector m_signals;

    struct RegisteredSignal
    {
      common::AdapterSignalListener Listener;
      void* Context;
      common::RegistrationHandle RegHandle;
    };

    typedef std::multimap<std::string, RegisteredSignal> SignalMap;
    SignalMap m_signalListeners;
  };
}
}

