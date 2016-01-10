#pragma once

#include "Bridge/IAdapter.h"

namespace adapter
{
namespace zigbee
{
  class Adapter;
  class Device;
  class Value;

  class Method : public bridge::IAdapterMethod
  {
  public:
    Method(std::weak_ptr<Device> const& target, std::string const& name);

  public:
    virtual std::string GetName() const;
    virtual std::string GetDescritpion() const;

    virtual bridge::AdapterValueVector const& GetInputParams() const;
    virtual bridge::AdapterValueVector const& GetOutputParams() const;

    virtual void SetInputParams(bridge::AdapterValueVector const& params);
    virtual void SetOutputParams(bridge::AdapterValueVector const& params);

    void AddInputParam(std::shared_ptr<bridge::IAdapterValue> const& p);
    void AddOutputParam(std::shared_ptr<bridge::IAdapterValue> const& p);

    virtual int32_t GetResult();
    void SetResult(int32_t st);

  private:
    std::shared_ptr<Device> GetParent() const
      { return m_parent.lock(); }

  private:
    std::string                   m_name;
    std::string                   m_description;
    bridge::AdapterValueVector    m_inputParams;
    bridge::AdapterValueVector    m_outputParams;
    std::weak_ptr<Device>              m_parent;
    int32_t                       m_result;
  };

  class Adapter : public bridge::IAdapter
  {
  public:
    Adapter();
    virtual ~Adapter();

    virtual std::string GetVendor();
    virtual std::string GetAdapterName();
    virtual std::string GetVersion();
    virtual std::string GetExposedAdapterPrefix();
    virtual std::string GetExposedApplicationName();
    virtual common::Guid GetExposedApplicationGuid();
    virtual bridge::AdapterSignalVector GetSignals();

    virtual int32_t Initialize();
    virtual int32_t Shutdown();

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

    virtual int32_t UnregisterSignalListener(
      bridge::IAdapter::RegistrationHandle const& h);
  };
}
}
