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
    static QStatus SetMsgArg(ajn::MsgArg& msgArg, std::string const& sig, std::vector<T> const& arr);

    template<class T>
    static QStatus GetArrayFromMsgArg(ajn::MsgArg& msgArg, std::string const& sig, std::vector<T>& arr);

    
    static QStatus SetMsgArgFromAdapterObject(IAdapterValue const& adapterValue, ajn::MsgArg& msg, DeviceMain* deviceMain);

    static QStatus GetAdapterValue(IAdapterValue& adapterValue, ajn::MsgArg const& msg);

    static QStatus GetAdapterObject(IAdapterValue& adapterValue, ajn::MsgArg const& msg, DeviceMain* deviceMain);

    static QStatus GetSignature(common::Variant::DataType type, std::string& sig);

    static void EncodeBusObjectName(std::string const& s, std::string &builtName);
    static void EncodePropertyOrMethodOrSignalName(std::string const& s, std::string &builtName);
    static void EncodeStringForInterfaceName(std::string const& s, std::string& encoded);
    static void EncodeStringForServiceName(std::string const& s, std::string &encoded);
    static void EncodeStringForRootServiceName(std::string const& s, std::string &encoded);
    static void EncodeStringForAppName(std::string const& s, std::string &encodeString);
    static std::string TrimChar(std::string const& s, char c);
  };

  template<class T>
  QStatus AllJoynHelper::SetMsgArg(ajn::MsgArg& msgArg, std::string const& sig, std::vector<T> const& arr)
  {
    QStatus st = ER_OK;
    return st;
  }
}

