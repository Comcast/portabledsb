#pragma once

#include "Bridge/AllJoynHeaders.h"
#include "Common/AdapterDevice.h"
#include <memory>


namespace bridge
{
  class BusObject : public ajn::BusObject
  {
  public:
    ~BusObject();

  private:
    BusObject(std::string const& appname, std::string const& path);

  protected:
    virtual QStatus Get(const char *ifcName, const char *propName, ajn::MsgArg &val);
    virtual QStatus Set(const char *ifcName, const char *propName, ajn::MsgArg &val);

  public:
  	static std::shared_ptr<BusObject> BuildFromAdapterDevice(adapter::Device const& dev);

  private:
    ajn::BusAttachment m_bus;
  };
}
