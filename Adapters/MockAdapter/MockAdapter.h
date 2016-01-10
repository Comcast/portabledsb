#pragma once

#include "Bridge/IAdapter.h"
#include "Common/defines.h"

#include <map>
#include <memory>

namespace adapters
{
namespace mock
{
  class MockAdapterDevice;
  class MockAdapterSignal;

  class MockAdapter : public bridge::IAdapter, public std::enable_shared_from_this<bridge::IAdapter>
  {
  public:
    MockAdapter();
    virtual ~MockAdapter();

    virtual std::string GetVendor();
    virtual std::string GetAdapterName();
    virtual std::string GetVersion();
    virtual std::string GetExposedAdapterPrefix();
    virtual std::string GetExposedApplicationName();
    virtual common::Guid GetExposedApplicationGuid();
    virtual bridge::AdapterSignalVector GetSignals();

    virtual int32_t Initialize(std::shared_ptr<bridge::IAdapterLog> const& log);
    virtual int32_t Shutdown();

    virtual int32_t GetConfiguration(std::vector<uint8_t>* /*configData*/)
      { return 0; }

    virtual int32_t SetConfiguration(std::vector<uint8_t> const& /*configData*/)
      { return 0; }

    virtual int32_t EnumDevices(
      bridge::EnumDeviceOptions opts,
      bridge::AdapterDeviceVector& deviceList,
      std::shared_ptr<bridge::IAdapterIoRequest>* req);

    virtual int32_t GetProperty(
      std::shared_ptr<bridge::IAdapterProperty>& prop,
      std::shared_ptr<bridge::IAdapterIoRequest>* req);

    virtual int32_t SetProperty(
      std::shared_ptr<bridge::IAdapterProperty> const& prop,
      std::shared_ptr<bridge::IAdapterIoRequest>* req);

    virtual int32_t GetPropertyValue(
      std::shared_ptr<bridge::IAdapterProperty> const& prop,
      std::string const& attributeName,
      std::shared_ptr<bridge::IAdapterValue>& value,
      std::shared_ptr<bridge::IAdapterIoRequest>* req);

    virtual int32_t SetPropertyValue(
      std::shared_ptr<bridge::IAdapterProperty> const& prop,
      std::shared_ptr<bridge::IAdapterValue> const& value,
      std::shared_ptr<bridge::IAdapterIoRequest>* req);

    virtual int32_t CallMethod(
      std::shared_ptr<bridge::IAdapterMethod>& method,
      std::shared_ptr<bridge::IAdapterIoRequest>* req);

    virtual int32_t RegisterSignalListener(
      std::string const& signalName,
      std::shared_ptr<bridge::IAdapterSignalListener> const& listener,
      void* argp,
      bridge::IAdapter::RegistrationHandle& handle);

    virtual int32_t UnregisterSignalListener(bridge::IAdapter::RegistrationHandle const& h);

    int32_t NotifySignalListeners(std::shared_ptr<MockAdapterSignal> const& signal);

  private:
    void CreateMockDevices();
    void CreateSignals();

  private:
    std::string m_vendor;
    std::string m_adapterName;
    std::string m_version;
    std::string m_exposedAdapterPrefix;
    std::string m_exposedApplicationName;
    common::Guid m_exposedApplicationGuid;
    std::shared_ptr<bridge::IAdapterLog> m_log;

    std::vector< std::shared_ptr<MockAdapterDevice> > m_devices;
    std::vector< std::shared_ptr<MockAdapterSignal> > m_signals;

    struct RegisteredSignal
    {
      std::shared_ptr<bridge::IAdapterSignalListener> Listener;
      void* Context;
      bridge::IAdapter::RegistrationHandle RegHandle;
    };

    typedef std::multimap<std::string, RegisteredSignal> SignalMap;
    SignalMap m_signalListeners;
  };
}
}

