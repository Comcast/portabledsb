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
  , m_bus(appname.c_str())
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
      obj->AddInterface(*intf);
    }
  }

  DSB_ASSERT(!obj->m_sessionPortListener);
  obj->m_sessionPortListener.reset(new BusObject::SessionPortListener(*obj));

  ajn::SessionPort port = ajn::SESSION_PORT_ANY;
  ajn::SessionOpts opts(ajn::SessionOpts::TRAFFIC_MESSAGES, false, ajn::SessionOpts::PROXIMITY_ANY, ajn::TRANSPORT_ANY);
  st = obj->m_bus.BindSessionPort(port, opts, *(obj->m_sessionPortListener.get()));

  return obj;
}

void
bridge::BusObject::AnnounceAndRegister()
{
  DSBLOG_NOT_IMPLEMENTED();

  // TODO;

  ajn::AboutData about("en");
  m_bus.RegisterBusObject(*this);
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

