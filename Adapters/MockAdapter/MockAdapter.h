#pragma once

#include "Bridge/IAdapter.h"
#include "Common/defines.h"

#include <map>

namespace AdapterLib
{
  class MockAdapterDevice;
  class MockAdapterSignal;

  class MockAdapter : public Bridge::IAdapter, public enable_shared_from_this<Bridge::IAdapter>
  {
  public:
    MockAdapter();
    virtual ~MockAdapter();

    virtual std::string GetVendor();
    virtual std::string GetAdapterName();
    virtual std::string GetVersion();
    virtual std::string GetExposedAdapterPrefix();
    virtual std::string GetExposedApplicationName();
    virtual std::string GetExposedApplicationGuid();
    virtual Bridge::AdapterSignalVector GetSignals();

    virtual int32_t Initialize();
    virtual int32_t Shutdown();

    virtual int32_t EnumDevices(
      Bridge::EnumDeviceOptions opts,
      Bridge::AdapterDeviceVector& deviceList,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    virtual int32_t GetProperty(
      shared_ptr<Bridge::IAdapterProperty>& prop,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    virtual int32_t SetProperty(
      shared_ptr<Bridge::IAdapterProperty> const& prop,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    virtual int32_t GetPropertyValue(
      shared_ptr<Bridge::IAdapterProperty> const& prop,
      std::string const& attributeName,
      shared_ptr<Bridge::IAdapterValue>& value,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    virtual int32_t SetPropertyValue(
      shared_ptr<Bridge::IAdapterProperty> const& prop,
      shared_ptr<Bridge::IAdapterValue> const& value,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    virtual int32_t CallMethod(
      shared_ptr<Bridge::IAdapterMethod>& method,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    virtual int32_t RegisterSignalListener(
      std::string const& signalName,
      shared_ptr<Bridge::IAdapterSignalListener> const& listener,
      void* argp,
      Bridge::IAdapter::RegistrationHandle& handle);

    virtual int32_t UnregisterSignalListener(Bridge::IAdapter::RegistrationHandle const& h);

    int32_t NotifySignalListeners(shared_ptr<MockAdapterSignal> const& signal);

  private:
    void CreateMockDevices();
    void CreateSignals();

  private:
    std::string m_vendor;
    std::string m_adapterName;
    std::string m_version;
    std::string m_exposedAdapterPrefix;
    std::string m_exposedApplicationName;
    std::string m_exposedApplicationGuid;

    std::vector< shared_ptr<MockAdapterDevice> > m_devices;
    std::vector< shared_ptr<MockAdapterSignal> > m_signals;

    struct RegisteredSignal
    {
      shared_ptr<Bridge::IAdapterSignalListener> Listener;
      void* Context;
      Bridge::IAdapter::RegistrationHandle RegHandle;
    };

    typedef std::multimap<std::string, RegisteredSignal> SignalMap;
    SignalMap m_signalListeners;
  };
}

