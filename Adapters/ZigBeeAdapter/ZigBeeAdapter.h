#pragma once

#include "Bridge/IAdapter.h"

namespace AdapterLib
{
  class ZigBeeAdapter;
  class ZigBeeAdapterDevice;
  class ZigBeeAdapterValue;

  class ZigBeeAdapterMethod : public Bridge::IAdapterMethod
  {
  public:
    ZigBeeAdapterMethod(weak_ptr<ZigBeeAdapterDevice> const& target, std::string const& name);

  public:
    virtual std::string GetName() const;
    virtual std::string GetDescritpion() const;

    virtual Bridge::AdapterValueVector const& GetInputParams() const;
    virtual Bridge::AdapterValueVector const& GetOutputParams() const;

    virtual void SetInputParams(Bridge::AdapterValueVector const& params);
    virtual void SetOutputParams(Bridge::AdapterValueVector const& params);

    void AddInputParam(shared_ptr<Bridge::IAdapterValue> const& p);
    void AddOutputParam(shared_ptr<Bridge::IAdapterValue> const& p);

    virtual int32_t GetResult();
    void SetResult(int32_t st);

  private:
    shared_ptr<ZigBeeAdapterDevice> GetParent() const
      { return m_parent.lock(); }

  private:
    std::string                   m_name;
    std::string                   m_description;
    Bridge::AdapterValueVector    m_inputParams;
    Bridge::AdapterValueVector    m_outputParams;
    weak_ptr<ZigBeeAdapterDevice> m_parent;
    int32_t                       m_result;
  };

  class ZigBeeAdapter : public Bridge::IAdapter
  {
  public:
    ZigBeeAdapter();
    virtual ~ZigBeeAdapter();

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

    virtual int32_t UnregisterSignalListener(
      Bridge::IAdapter::RegistrationHandle const& h);
  };
}

