#pragma once

#include "IAdapter.h"
#include <string>

#include <alljoyn/MsgArg.h>

namespace Bridge
{
  class DeviceMain;

  class AllJoynHelper
  {
  public:
    static QStatus SetMsgArg(IAdapterValue const& adapterValue, ajn::MsgArg& msgArg);
    static QStatus SetMsgArgFromAdapterObject(IAdapterValue const& adapterValue, ajn::MsgArg& msg, DeviceMain* deviceMain);
    static QStatus GetAdapterValue(IAdapterValue& adapterValue, ajn::MsgArg const& msg);
    static QStatus GetAdapterObject(IAdapterValue& adapterValue, ajn::MsgArg const& msg _In_ DeviceMain *deviceMain);
//    static QStatus GetSignature(_In_ Windows::Foundation::PropertyType propertyType, _Out_ std::string &signature);

    static void BuildBusObjectName(std::string const& s, std::string &builtName);
    static void BuildPropertyOrMethodOrSignalName(std::string const& s, std::string &builtName);
    static void EncodeStringForServiceName(std::string const& s, std::string &encodeString);
    static void EncodeStringForRootServiceName(std::string const& s, std::string &encodeString);
    static void EncodeStringForAppName(std::string const& s, std::string &encodeString);
  };
}

