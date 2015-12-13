#ifndef __DEVICE_METHOD_H__
#define __DEVICE_METHOD_H__

#include "IAdapter.h"

#include <alljoyn/Message.h>
#include <alljoyn/MsgArg.h>
#include <alljoyn/Status.h>

namespace bridge
{
  class DeviceMain;

  class DeviceMethod
  {
  public:
    DeviceMethod();
    virtual ~DeviceMethod();

    QStatus Initialize(shared_ptr<DeviceMain> const& parent, shared_ptr<IAdapterMethod> const& adapterMethod);
    uint32_t InvokeMethod(ajn::Message const& msg, ajn::MsgArg* outArgs, size_t numOutArgs);

    inline std::string const& GetName() const
      { return m_exposedName; }

  private:
    QStatus SetName(std::string const& name);
    QStatus BuildSignature(AdapterValueVector const& valueList, std::string& sig, std::string& parameterNames);

  private:
    std::string             m_exposedName;
    std::string             m_inSignature;
    std::string             m_outSignature;
    std::string             m_parameterNames;
    shared_ptr<DeviceMain>  m_parent;
  };
}

#endif
