#pragma once

#include "IAdapter.h"
#include <string>
#include <vector>

#include <alljoyn/MsgArg.h>

namespace bridge
{
  class DeviceMain;

  class AllJoynHelper
  {
  public:
    static QStatus SetMsgArg(IAdapterValue const& adapterValue, ajn::MsgArg& msgArg);
    
    template<class T>
    static QStatus SetMsgArg(ajn::MsgArg& msgArg, std::string const& signature, std::vector<T> const& arr);

    template<class T>
    static QStatus GetArrayFromMsgArg(ajn::MsgArg& msgArg, std::string const& signature, std::vector<T>& arr);

    
    static QStatus SetMsgArgFromAdapterObject(IAdapterValue const& adapterValue, ajn::MsgArg& msg, DeviceMain* deviceMain);

    static QStatus GetAdapterValue(IAdapterValue& adapterValue, ajn::MsgArg const& msg);

    static QStatus GetAdapterObject(IAdapterValue& adapterValue, ajn::MsgArg const& msg, DeviceMain* deviceMain);

    // TODO:
    // static QStatus GetSignature(_In_ Windows::Foundation::PropertyType propertyType, _Out_ std::string &signature);

    static void EncodeBusObjectName(std::string const& s, std::string &builtName);
    static void EncodePropertyOrMethodOrSignalName(std::string const& s, std::string &builtName);
    static void EncodeStringForInterfaceName(std::string const& s, std::string& encoded);
    static void EncodeStringForServiceName(std::string const& s, std::string &encoded);
    static void EncodeStringForRootServiceName(std::string const& s, std::string &encoded);
    static void EncodeStringForAppName(std::string const& s, std::string &encodeString);
    static std::string TimeChar(std::string const& s, char c);
  };
}

