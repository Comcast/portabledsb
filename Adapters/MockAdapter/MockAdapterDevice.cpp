#include "Adapters/MockAdapter/MockAdapterDevice.h"

using namespace AdapterLib;

MockAdapterDevice::MockAdapterDevice(
    MockDeviceDescriptor const& desc,
    shared_ptr<Bridge::IAdapter> const& parent)
  : m_name(desc.Name)
  , m_parent(parent)
  , m_vendor(desc.VendorName)
  , m_model(desc.Model)
  , m_version("never set") // TODO
  , m_firmwareVersion(desc.Version)
  , m_serialNumber(desc.SerialNumber)
  , m_description(desc.Description)
{
  for (int i = 0; i < kMockDeviceMaxProperties; ++i)
  {
    MockPropertyDescriptor const& d = desc.PropertyDescriptors[i];
    if (d.Id == kLastDescriptorId)
      break;

    m_properties.push_back(shared_ptr<Bridge::IAdapterProperty>(
      new MockAdapterProperty(d)));
  }
}

void MockAdapterDevice::CreateSignals()
{
}

void MockAdapterDevice::CreateMethods()
{
  shared_ptr<MockAdapterMethod> m(new MockAdapterMethod(kDeviceResetMethod, this));
  m_methods.push_back(m);
}

std::string MockAdapterDevice::GetName()
{
  return m_name;
}

std::string MockAdapterDevice::GetVendor()
{
  return m_vendor;
}

std::string MockAdapterDevice::GetModel()
{
  return m_model;
}

std::string MockAdapterDevice::GetVersion()
{
  return m_version;
}

std::string MockAdapterDevice::GetFirmwareVersion()
{
  return m_firmwareVersion;
}

std::string MockAdapterDevice::GetSerialNumber()
{
  return m_serialNumber;
}

std::string MockAdapterDevice::GetDescription()
{
  return m_description;
}

Bridge::AdapterPropertyVector MockAdapterDevice::GetProperties()
{
  return m_properties;
}

Bridge::AdapterMethodVector MockAdapterDevice::GetMethods()
{
  return m_methods;
}

Bridge::AdapterSignalVector MockAdapterDevice::GetSignals()
{
  return m_signals;
}

MockAdapterProperty::MockAdapterProperty(MockPropertyDescriptor const& desc)
  : m_name(desc.Name)
{
}

std::string MockAdapterProperty::GetName()
{
  return m_name;
}

Bridge::AdapterValueVector MockAdapterProperty::GetAttributes()
{
  return m_attributes;
}



// MockAdapterMethod 
MockAdapterMethod::MockAdapterMethod(std::string const& name, MockAdapterDevice* parent)
  : m_name(name)
  , m_parent(parent)
{
}

std::string MockAdapterMethod::GetName()
{
  return m_name;
}

std::string MockAdapterMethod::GetDescription()
{
  return m_description;
}

Bridge::AdapterValueVector MockAdapterMethod::GetInputParams()
{
  return m_inputParams;
}

Bridge::AdapterValueVector MockAdapterMethod::GetOutputParams()
{
  return m_outputParams;
}

void MockAdapterMethod::SetInputParams(Bridge::AdapterValueVector const& params)
{
  m_inputParams = params;
}

void MockAdapterMethod::SetOutputParams(Bridge::AdapterValueVector const& params)
{
  m_outputParams = params;
}

int32_t MockAdapterMethod::GetResult()
{
  return 0;
}


