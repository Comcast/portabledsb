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
    ajn::InterfaceDescription* intf = nullptr;

    DSBLOG_DEBUG("looking for interface: %s", interface.GetName().c_str());
    bridge::Error::ThrowIfNotOk(bus.CreateInterface(interface.GetName().c_str(), intf, ajn::AJ_IFC_SECURITY_OFF),
      "failed to create interface: %s", interface.GetName().c_str());

    for (auto prop : interface.GetProperties())
    {
      DSBLOG_DEBUG("adding property to interface: %s", prop.GetName().c_str());
      std::string sig = bridge::AllJoynHelper::GetSignature(prop.GetType());

      bridge::Error::ThrowIfNotOk(intf->AddProperty(prop.GetName().c_str(), sig.c_str(), GetAccess(prop)),
          "failed to add property %s to interface %s", prop.GetName().c_str(), interface.GetName().c_str());
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
bridge::BusObject::BuildFromAdapterDevice(adapter::Device const& dev)
{
  // TODO: what appanme and what path?
  std::string appname = "some-appname";
  std::string path = "/zigbee/" + dev.GetBasicInfo().GetName();

  std::shared_ptr<BusObject> obj(new BusObject(appname, path));

  Error::ThrowIfNotOk(obj->m_bus.Start(),
      "failed to start bus attachment");

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
  return obj;
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
