
#include "AllJoynHelper.h"


QStatus
Bridge::AllJoynHelper::SetMsgArg(IAdapterValue const& adapterValue, ajn::MsgArg& msgArg)
{
  return ER_NOT_IMPLEMENTED;
}
   
QStatus
Bridge::AllJoynHelper::SetMsgArgFromAdapterObject(IAdapterValue const& adapterValue, ajn::MsgArg& msg, DeviceMain* deviceMain)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
Bridge::AllJoynHelper::GetAdapterValue(IAdapterValue& adapterValue, ajn::MsgArg const& msg)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
Bridge::AllJoynHelper::GetAdapterObject(IAdapterValue& adapterValue, ajn::MsgArg const& msg, DeviceMain *deviceMain)
{
  return ER_NOT_IMPLEMENTED;
}

// TODO:
// static QStatus GetSignature(_In_ Windows::Foundation::PropertyType propertyType, _Out_ std::string &signature);

void
Bridge::AllJoynHelper::EncodeBusObjectName(std::string const& s, std::string &builtName)
{
}

void
Bridge::AllJoynHelper::EncodePropertyOrMethodOrSignalName(std::string const& s, std::string &builtName)
{
}

void
Bridge::AllJoynHelper::EncodeStringForInterfaceName(std::string const& s, std::string& encoded)
{
}

void
Bridge::AllJoynHelper::EncodeStringForServiceName(std::string const& s, std::string &encoded)
{
}

void
Bridge::AllJoynHelper::EncodeStringForRootServiceName(std::string const& s, std::string &encoded)
{
}

void
Bridge::AllJoynHelper::EncodeStringForAppName(std::string const& s, std::string &encodeString)
{
}

std::string TimeChar(std::string const& s, char c)
{
  return std::string();
}
