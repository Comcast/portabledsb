#include "Adapters/MockAdapter/MockAdapterDevice.h"
#include "Adapters/MockAdapter/MockAdapter.h"

using namespace AdapterLib;

MockAdapterDevice::MockAdapterDevice(MockDeviceDescriptor const& desc, shared_ptr<MockAdapter> const& parent)
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
   Bridge::AdapterValueVector params;
   m_signalPrototypes.push_back(shared_ptr<MockAdapterSignal>(new MockAdapterSignal(
    Bridge::kChangeOfValueSignal, *this, params)));

  // TODO: This is incomplete, re-visit once signal emit is worked on 
}

void MockAdapterDevice::CreateMethods()
{
  shared_ptr<MockAdapterDevice> self = shared_from_this();

  shared_ptr<MockAdapterMethod> m(new MockAdapterMethod(kDeviceResetMethod, self));
  m->AddInputParam(shared_ptr<Bridge::IAdapterValue>(
    new MockAdapterValue(kDeviceResetPropertyHandle, *this)));
  m_methods.push_back(m);
}

QStatus MockAdapterDevice::DispatchMethod(
  shared_ptr<MockAdapterMethod>& method,
  Bridge::IAdapterIoRequest** req)
{
  if (req)
    *req = NULL;

  QStatus st = ER_OK;
  if (method->GetName() == kDeviceResetMethod)
    st = Reset(method);

  return st;
}

QStatus MockAdapterDevice::SendSignal(
    std::string const& signalName,
    shared_ptr<MockAdapterProperty> const& prop,
    shared_ptr<MockAdapterValue> const& value)
{
  if (!prop)
    return ER_BAD_ARG_2;
  if (!value)
    return ER_BAD_ARG_3;

  QStatus st = ER_FAIL;

  shared_ptr<MockAdapterSignal> signal;
  shared_ptr<MockAdapter> parent = m_parent.lock();
  if (!parent)
    return ER_FAIL;

  typedef Bridge::AdapterSignalVector::const_iterator iterator;
  for (iterator begin = m_signalPrototypes.begin(), end = m_signalPrototypes.end();
    ((begin != end) && !signal); ++begin)
  {
    if ((*begin)->GetName() == signalName)
    {
      signal = dynamic_pointer_cast<MockAdapterSignal>(*begin);
      DSB_ASSERT(signal != NULL);
      if (signal)
        signal = signal->Clone();
    }
  }

  if (!signal)
    return st;

  if (signal->GetName() == Bridge::kChangeOfValueSignal)
  {
    Bridge::AdapterValueVector params;

    shared_ptr<MockAdapterValue> p(new MockAdapterValue("PropertyName", *this));
    p->SetData(ajn::MsgArg("s", prop->GetName().c_str(), NULL));

    shared_ptr<MockAdapterValue> v(new MockAdapterValue("PropertyValue", *this));
    v->SetData(value->GetData());

    params.push_back(p);
    params.push_back(v);
  }

  if (signal)
    parent->NotifySignalListeners(signal);

  return st;
}

QStatus MockAdapterDevice::Reset(shared_ptr<MockAdapterMethod>& method)
{
  QStatus st = ER_OK;

  Bridge::AdapterValueVector const& inParams = method->GetInputParams();
  if (inParams.size() == 0)
  {
    // reset device
  }
  else if (inParams.size() == 1)
  {
    // reset property by name
    shared_ptr<Bridge::IAdapterValue> arg1 = inParams[0];
    if (!arg1)
      return ER_BAD_ARG_1;
  }

  method->SetResult(ER_OK);

  return st;
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

Bridge::AdapterPropertyVector const& MockAdapterDevice::GetProperties() const
{
  return m_properties;
}

Bridge::AdapterMethodVector const& MockAdapterDevice::GetMethods() const
{
  return m_methods;
}

Bridge::AdapterSignalVector const& MockAdapterDevice::GetSignals() const
{
  return m_signalPrototypes;
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
MockAdapterMethod::MockAdapterMethod(
      std::string const& name,
      weak_ptr<MockAdapterDevice> const& parent)
  : m_name(name)
  , m_parent(parent)
{
}

std::string MockAdapterMethod::GetName() const
{
  return m_name;
}

std::string MockAdapterMethod::GetDescription()
{
  return m_description;
}

Bridge::AdapterValueVector const& MockAdapterMethod::GetInputParams() const
{
  return m_inputParams;
}

Bridge::AdapterValueVector const& MockAdapterMethod::GetOutputParams() const
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

void MockAdapterMethod::AddInputParam(shared_ptr<Bridge::IAdapterValue> const& p)
{
  m_inputParams.push_back(p);
}

void MockAdapterMethod::AddOutputParam(shared_ptr<Bridge::IAdapterValue> const& p)
{
  m_outputParams.push_back(p);
}

QStatus MockAdapterMethod::GetResult()
{
  return m_result;
}

void MockAdapterMethod::SetResult(QStatus st)
{
  m_result = st;
}

MockAdapterValue::MockAdapterValue(
    std::string const& name,
    MockAdapterDevice const& parent)
  : m_name(name)
  , m_parent(parent)
{
}


std::string MockAdapterValue::GetName()
{
  return m_name;
}

ajn::MsgArg MockAdapterValue::GetData()
{
  return m_data;
}

void MockAdapterValue::SetData(ajn::MsgArg const& msg)
{
  m_data = msg;
}

MockAdapterSignal::MockAdapterSignal(
      std::string const& name,
      Bridge::IAdapterDevice const& parent,
      Bridge::AdapterValueVector const& params)
  : m_name(name)
  , m_parent(parent)
  , m_params(params)
{
}

shared_ptr<MockAdapterSignal> MockAdapterSignal::Clone()
{
  shared_ptr<MockAdapterSignal> clone(new MockAdapterSignal(
    m_name,
    m_parent,
    m_params));
  return clone;
}


std::string MockAdapterSignal::GetName() const
{
  return m_name;
}

Bridge::AdapterValueVector const& MockAdapterSignal::GetParams() const
{
  return m_params;
}


