#include "Bridge/BridgeBusObject.h"
#include "Bridge/BridgeError.h"
#include "Bridge/AllJoynHelper.h"
#include "Common/AdapterLog.h"

namespace
{
  DSB_DECLARE_LOGNAME(BusObject);

  uint8_t GetAccess(adapter::Property const& p)
  {
    uint8_t ret = 0;
    switch (p.GetAccess())
    {
      case adapter::AccessType::Read:
        ret = ajn::PROP_ACCESS_READ;
        break;
      case adapter::AccessType::Write:
        ret = ajn::PROP_ACCESS_WRITE;
        break;
      case adapter::AccessType::ReadWrite:
        ret = ajn::PROP_ACCESS_RW;
        break;
    }
    return ret;
  }

  ajn::InterfaceDescription const* BuildInterface(ajn::BusAttachment& bus, adapter::Interface const& interface)
  {
    QStatus st = ER_OK;
    ajn::InterfaceDescription* intf = nullptr;

    char const* ifcName = interface.GetName().c_str();

    DSBLOG_DEBUG("looking for interface: %s", interface.GetName().c_str());
    
    st = bus.CreateInterface(interface.GetName().c_str(), intf, ajn::AJ_IFC_SECURITY_OFF);
    bridge::Error::ThrowIfNotOk(st, "failed to create interface: %s", interface.GetName().c_str());

    for (auto attr : interface.GetAttributes())
    {
      DSBLOG_DEBUG("adding annotation to interface");
      st = intf->AddAnnotation(attr.first.c_str(), attr.second.ToString().c_str());
      bridge::Error::ThrowIfNotOk(st, "failed to add annotation to interface");
    }

    for (auto prop : interface.GetProperties())
    {
      char const* name = prop.GetName().c_str();
      DSBLOG_DEBUG("adding property to interface: %s", name);

      std::string sig = bridge::AllJoynHelper::GetSignature(prop.GetType());

      st = intf->AddProperty(name, sig.c_str(), GetAccess(prop));
      bridge::Error::ThrowIfNotOk(st, "failed to add property %s to interface %s", name, ifcName);

      for (auto attr : prop.GetAttributes())
      {
        st = intf->AddPropertyAnnotation(name, attr.first.c_str(), attr.second.ToString().c_str());
        bridge::Error::ThrowIfNotOk(st, "failed to add annotation to property");
      }
    }

    for (auto method : interface.GetMethods())
    {
      // TODO:
    }

    for (auto signal : interface.GetSignals())
    {
      // TODO:
    }

    if (intf != nullptr)
      intf->Activate();

    return bus.GetInterface(interface.GetName().c_str());
  }
}

bridge::BusObject::BusObject(std::string const& appname, std::string const& path)
  : ajn::BusObject(path.c_str())
  , m_bus(appname.c_str(), true)
{
}

bridge::BusObject::~BusObject()
{
}

std::shared_ptr<bridge::BusObject>
bridge::BusObject::BuildFromAdapterDevice(std::string const& appname, std::string const& path,
  std::shared_ptr<adapter::Adapter> const& adapter, adapter::Device const& dev)
{
  QStatus st = ER_OK;

  std::shared_ptr<BusObject> obj(new BusObject(appname, path));
  obj->m_adapter = adapter;
  obj->m_adapterDevice = dev;

  st = obj->m_bus.Start();
  Error::ThrowIfNotOk(st, "failed to start bus attachment");

  st = obj->m_bus.Connect();
  Error::ThrowIfNotOk(st, "failed to connect to alljoyn router");

  for (auto interface : dev.GetInterfaces())
  {
    ajn::InterfaceDescription const* intf = obj->m_bus.GetInterface(interface.GetName().c_str());
    if (intf == nullptr)
      intf = BuildInterface(obj->m_bus, interface);

    if (intf != nullptr)
    {
      DSBLOG_DEBUG("activating interface: %s", interface.GetName().c_str());
      obj->AddInterface(*intf, ajn::BusObject::ANNOUNCED);
    }
  }

  DSB_ASSERT(!obj->m_sessionPortListener);
  obj->m_sessionPortListener.reset(new BusObject::SessionPortListener(*obj));

  ajn::SessionPort port = ajn::SESSION_PORT_ANY;
  ajn::SessionOpts opts(ajn::SessionOpts::TRAFFIC_MESSAGES, false, ajn::SessionOpts::PROXIMITY_ANY, ajn::TRANSPORT_ANY);

  st = obj->m_bus.BindSessionPort(port, opts, *(obj->m_sessionPortListener.get()));
  Error::ThrowIfNotOk(st, "failed to bind session port");

  obj->m_sessionPort = port;;
  return obj;
}

void
bridge::BusObject::AnnounceAndRegister()
{
  adapter::ItemInformation const& devInfo = m_adapterDevice.GetInfo();

  // TODO: What AppId should we use?
  adapter::Guid appId = adapter::Guid::NewGuid();

  QStatus st = ER_OK;

  m_aboutData.reset(new ajn::AboutData("en"));
  st = m_aboutData->SetAppId(appId.ToByteArray());
  Error::ThrowIfNotOk(st, "failed to set appid");

  st = m_aboutData->SetAppName(m_adapter->GetApplicationName().c_str());
  Error::ThrowIfNotOk(st, "failed to set application name");

  st = m_aboutData->SetDeviceId((char const *)devInfo.GetId().ToByteArray());
  Error::ThrowIfNotOk(st, "failed to set device id");

  st = m_aboutData->SetDeviceName(devInfo.GetName().c_str());
  Error::ThrowIfNotOk(st, "failed to set device name");

  st = m_aboutData->SetManufacturer(devInfo.GetVendor().c_str());
  Error::ThrowIfNotOk(st, "failed to set manufacturer");

  st = m_aboutData->SetDescription(devInfo.GetDescription().c_str());
  Error::ThrowIfNotOk(st, "failed to set description");

  st = m_aboutData->SetDateOfManufacture("2016-01-01");
  Error::ThrowIfNotOk(st, "failed to set date of manufacture");

  st = m_aboutData->SetSoftwareVersion(devInfo.GetFirmwareVersion().c_str());
  Error::ThrowIfNotOk(st, "failed to set software version");

  st = m_aboutData->SetHardwareVersion(devInfo.GetVersion().c_str());
  Error::ThrowIfNotOk(st, "failed to set hardware version");

  st = m_aboutData->SetModelNumber(devInfo.GetModel().c_str());
  Error::ThrowIfNotOk(st, "failed to set model");

  m_aboutData->SetSupportUrl("http://www.xfinity.com");
  if (!m_aboutData->IsValid())
    Error::Throw("invalid about data");

  st = m_bus.RegisterBusObject(*this);
  Error::ThrowIfNotOk(st, "failed to register bus object");

  m_aboutObject.reset(new ajn::AboutObj(m_bus));

  st = m_aboutObject->Announce(m_sessionPort, *m_aboutData.get());
  Error::ThrowIfNotOk(st, "failed to announce device");
}

QStatus
bridge::BusObject::Get(const char *ifcName, const char *propName, ajn::MsgArg &val)
{
  DSBLOG_NOT_IMPLEMENTED();
  return ER_OK;
}

QStatus
bridge::BusObject::Set(const char *ifcName, const char *propName, ajn::MsgArg &val)
{
  DSBLOG_NOT_IMPLEMENTED();
  return ER_OK;
}

bool
bridge::BusObject::SessionPortListener::AcceptSessionJoiner(ajn::SessionPort /*sessionPort*/,
  char const* /*joiner*/, ajn::SessionOpts const& /*opts*/)
{
  DSBLOG_NOT_IMPLEMENTED();
  return true;
}

void
bridge::BusObject::SessionPortListener::SessionJoined(ajn::SessionPort /*sessionPort*/, ajn::SessionId /*id*/,
  char const* /*joiner*/)
{
  DSBLOG_NOT_IMPLEMENTED();
}

