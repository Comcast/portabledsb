#pragma once

#include "Adapters/MockAdapter/MockDevices.h"
#include "Bridge/IAdapter.h"

namespace AdapterLib
{
  std::string const kDeviceResetMethod = "Device_Reset";
  std::string const kDeviceResetPropertyHandle = "Property_Handle";
  std::string const kDsbMethodReturnValue = "Return_Value";

  class MockAdapterDevice;

  class MockAdapterMethod : public Bridge::IAdapterMethod
  {
  public:
    MockAdapterMethod(std::string const& name, MockAdapterDevice* parent);

    virtual std::string GetName();
    virtual std::string GetDescription();

    virtual Bridge::AdapterValueVector GetInputParams();
    virtual Bridge::AdapterValueVector GetOutputParams();

    virtual void SetInputParams(Bridge::AdapterValueVector const& params);
    virtual void SetOutputParams(Bridge::AdapterValueVector const& params);

    virtual int32_t GetResult();

  private:
    std::string                   m_name;
    std::string                   m_description;
    Bridge::AdapterValueVector    m_inputParams;
    Bridge::AdapterValueVector    m_outputParams;
    MockAdapterDevice*            m_parent;
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

  class MockAdapterDevice : public Bridge::IAdapterDevice
  {
  public:
    MockAdapterDevice(
      MockDeviceDescriptor const& desc,
      shared_ptr<Bridge::IAdapter> const& parent);
      
    virtual std::string GetName();
    virtual std::string GetVendor();
    virtual std::string GetModel();
    virtual std::string GetVersion();
    virtual std::string GetFirmwareVersion();
    virtual std::string GetSerialNumber();
    virtual std::string GetDescription();

    virtual Bridge::AdapterPropertyVector GetProperties();
    virtual Bridge::AdapterMethodVector GetMethods();
    virtual Bridge::AdapterSignalVector GetSignals();

  private:
    void CreateMethods();
    void CreateSignals();

  private:
    std::string                         m_name;
    shared_ptr<Bridge::IAdapter>        m_parent;
    std::string                         m_vendor;
    std::string                         m_model;
    std::string                         m_version;
    std::string                         m_firmwareVersion;
    std::string                         m_serialNumber;
    std::string                         m_description;
    Bridge::AdapterPropertyVector       m_properties;
    Bridge::AdapterMethodVector         m_methods; 
    Bridge::AdapterSignalVector         m_signals;
  };
};

