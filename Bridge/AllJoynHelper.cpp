
#include "AllJoynHelper.h"


QStatus
bridge::AllJoynHelper::SetMsgArg(IAdapterValue const& adapterValue, ajn::MsgArg& msgArg)
{
  return ER_NOT_IMPLEMENTED;
}
   
QStatus
bridge::AllJoynHelper::SetMsgArgFromAdapterObject(IAdapterValue const& adapterValue, ajn::MsgArg& msg, DeviceMain* deviceMain)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::AllJoynHelper::GetAdapterValue(IAdapterValue& adapterValue, ajn::MsgArg const& msg)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::AllJoynHelper::GetAdapterObject(IAdapterValue& adapterValue, ajn::MsgArg const& msg, DeviceMain *deviceMain)
{
  return ER_NOT_IMPLEMENTED;
}

// TODO:
// static QStatus GetSignature(_In_ Windows::Foundation::PropertyType propertyType, _Out_ std::string &signature);

void
bridge::AllJoynHelper::EncodeBusObjectName(std::string const& s, std::string &builtName)
{
}

void
bridge::AllJoynHelper::EncodePropertyOrMethodOrSignalName(std::string const& s, std::string &builtName)
{
}

void
bridge::AllJoynHelper::EncodeStringForInterfaceName(std::string const& s, std::string& encoded)
{
}

void
bridge::AllJoynHelper::EncodeStringForServiceName(std::string const& s, std::string &encoded)
{
}

void
bridge::AllJoynHelper::EncodeStringForRootServiceName(std::string const& s, std::string &encoded)
{
}

void
bridge::AllJoynHelper::EncodeStringForAppName(std::string const& s, std::string &encodeString)
{
}

std::string
bridge::AllJoynHelper::TimeChar(std::string const& s, char c)
{
  return std::string();
}
