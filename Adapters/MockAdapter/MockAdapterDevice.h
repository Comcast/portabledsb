#pragma once

#include "Adapters/MockAdapter/MockDevices.h"
#include "Bridge/IAdapter.h"

namespace AdapterLib
{
  std::string const kDeviceResetMethod = "Device_Reset";
  std::string const kDeviceResetPropertyHandle = "Property_Handle";
  std::string const kDsbMethodReturnValue = "Return_Value";

  class MockAdapter;
  class MockAdapterDevice;
  class MockAdapterValue;

  class MockAdapterSignal : public Bridge::IAdapterSignal
  {
  public:
    MockAdapterSignal(
      std::string const& name,
      weak_ptr<MockAdapterDevice> const& parent,
      Bridge::AdapterValueVector const& params);

    virtual std::string GetName() const;
    virtual Bridge::AdapterValueVector const& GetParams() const;

    shared_ptr<MockAdapterSignal> Clone();

  private:
    std::string                     m_name;
    weak_ptr<MockAdapterDevice>     m_parent;
    Bridge::AdapterValueVector      m_params;
  };

  class MockAdapterMethod : public Bridge::IAdapterMethod
  {
    friend class MockAdapter;

  public:
    MockAdapterMethod(std::string const& name, weak_ptr<MockAdapterDevice> const& parent);

    virtual std::string GetName() const;
    virtual std::string GetDescription();

    virtual Bridge::AdapterValueVector const& GetInputParams() const;
    virtual Bridge::AdapterValueVector const& GetOutputParams() const;

    virtual void SetInputParams(Bridge::AdapterValueVector const& params);
    virtual void SetOutputParams(Bridge::AdapterValueVector const& params);

    void AddInputParam(shared_ptr<Bridge::IAdapterValue> const& p);
    void AddOutputParam(shared_ptr<Bridge::IAdapterValue> const& p);

    virtual int32_t GetResult();
    void SetResult(int32_t st);

  private:
    shared_ptr<MockAdapterDevice> GetParent() const
      { return m_parent.lock(); }

  private:
    std::string                   m_name;
    std::string                   m_description;
    Bridge::AdapterValueVector    m_inputParams;
    Bridge::AdapterValueVector    m_outputParams;
    weak_ptr<MockAdapterDevice>   m_parent;
    int32_t                       m_result;
  };

  class MockAdapterProperty : public Bridge::IAdapterProperty
  {
  public:
    MockAdapterProperty(MockPropertyDescriptor const& desc, weak_ptr<MockAdapterDevice> const& parent);

    virtual std::string GetName();
    virtual Bridge::AdapterValueVector GetAttributes();

    shared_ptr<MockAdapterDevice> GetParent()
      { return m_parent.lock(); }

    shared_ptr<MockAdapterValue> GetAttributeByName(std::string const& name);

  private:
    std::string                 m_name;
    Bridge::AdapterValueVector  m_attributes;
    weak_ptr<MockAdapterDevice> m_parent;
  };

  class MockAdapterValue : public Bridge::IAdapterValue
  {
  public:
    MockAdapterValue(std::string const& name);

    virtual std::string GetName();
    virtual Common::Variant GetData();
    virtual void SetData(Common::Variant const& v);

  private:
    std::string                 m_name;
    Common::Variant             m_data;
  };

  class MockAdapterDevice : public Bridge::IAdapterDevice, public enable_shared_from_this<MockAdapterDevice> 
  {
  public:
    static shared_ptr<MockAdapterDevice> Create(
      MockDeviceDescriptor const& desc,
      weak_ptr<MockAdapter> const& parent);

    virtual std::string GetName();
    virtual std::string GetVendor();
    virtual std::string GetModel();
    virtual std::string GetVersion();
    virtual std::string GetFirmwareVersion();
    virtual std::string GetSerialNumber();
    virtual std::string GetDescription();

    virtual Bridge::AdapterPropertyVector const& GetProperties() const;
    virtual Bridge::AdapterMethodVector const& GetMethods() const;
    virtual Bridge::AdapterSignalVector const& GetSignals() const;

    int32_t DispatchMethod(
      shared_ptr<MockAdapterMethod>& method,
      shared_ptr<Bridge::IAdapterIoRequest>* req);

    int32_t SendSignal(
      std::string const& signalName,
      shared_ptr<MockAdapterProperty> const& prop,
      shared_ptr<MockAdapterValue> const& attr);

    shared_ptr<MockAdapter> GetParent() const
      { return m_parent.lock(); }

  private:
    MockAdapterDevice(MockDeviceDescriptor const& desc, weak_ptr<MockAdapter> const& parent);

    void CreateMethods();
    void CreateSignals();

    int32_t Reset(shared_ptr<MockAdapterMethod>& method);

  private:
    std::string                         m_name;
    weak_ptr<MockAdapter>               m_parent;
    std::string                         m_vendor;
    std::string                         m_model;
    std::string                         m_version;
    std::string                         m_firmwareVersion;
    std::string                         m_serialNumber;
    std::string                         m_description;
    Bridge::AdapterPropertyVector       m_properties;
    Bridge::AdapterMethodVector         m_methods; 
    Bridge::AdapterSignalVector         m_signalPrototypes;
  };
};
