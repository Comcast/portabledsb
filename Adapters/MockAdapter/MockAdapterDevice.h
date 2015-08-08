#pragma once

#include "Adapters/MockAdapter/MockDevices.h"
#include "Bridge/IAdapter.h"

namespace AdapterLib
{
  std::string const kDeviceResetMethod = "Device_Reset";
  std::string const kDeviceResetPropertyHandle = "Property_Handle";
  std::string const kDsbMethodReturnValue = "Return_Value";

  class MockAdapterDevice;

  class MockAdapterSignal : public Bridge::IAdapterSignal
  {
  public:
    MockAdapterSignal(
      std::string const& name,
      Bridge::IAdapterDevice const& dev,
      Bridge::AdapterValueVector const& params);

    virtual std::string GetName() const;
    virtual Bridge::AdapterValueVector const& GetParams() const;

  private:
    std::string                     m_name;
    Bridge::IAdapterDevice const&   m_parent;
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

    virtual QStatus GetResult();
    void SetResult(QStatus st);

  private:
    shared_ptr<MockAdapterDevice> GetParent() const
      { return m_parent.lock(); }

  private:
    std::string                   m_name;
    std::string                   m_description;
    Bridge::AdapterValueVector    m_inputParams;
    Bridge::AdapterValueVector    m_outputParams;
    weak_ptr<MockAdapterDevice>   m_parent;
    QStatus                       m_result;
  };

  class MockAdapterProperty : public Bridge::IAdapterProperty
  {
  public:
    MockAdapterProperty(MockPropertyDescriptor const& desc);

    virtual std::string GetName();
    virtual Bridge::AdapterValueVector GetAttributes();

  private:
    std::string                 m_name;
    Bridge::AdapterValueVector  m_attributes;
  };

  class MockAdapterValue : public Bridge::IAdapterValue
  {
  public:
    MockAdapterValue(
      std::string const& name,
      MockAdapterDevice const& parent);

    virtual std::string GetName();
    virtual ajn::MsgArg GetData();
    virtual void SetData(ajn::MsgArg const& msg);

  private:
    std::string                 m_name;
    ajn::MsgArg                 m_data;
    MockAdapterDevice const&    m_parent;
  };

  class MockAdapterDevice : public Bridge::IAdapterDevice, public enable_shared_from_this<MockAdapterDevice> 
  {
  public:
    MockAdapterDevice(
      MockDeviceDescriptor const& desc,
      Bridge::IAdapter const& parent);
      
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

    QStatus DispatchMethod(
      shared_ptr<MockAdapterMethod>& method,
      Bridge::IAdapterIoRequest** req);

    QStatus SendSignal(
      std::string const& signalName,
      shared_ptr<MockAdapterProperty> const& prop,
      shared_ptr<MockAdapterValue> const& attr);

  private:
    void CreateMethods();
    void CreateSignals();

    QStatus Reset(shared_ptr<MockAdapterMethod>& method);

  private:
    std::string                         m_name;
    Bridge::IAdapter const&             m_parent;
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

