
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
  QStatus                 st;
  std::string             sig;
  common::Variant const&  val = adapterValue.GetData();

  // TODO: why not just hard-code signatures? GetSignature is never used outside
  // AllJoynHelper
  st = GetSignature(val.GetType(), sig);
  if (st != ER_OK)
    return st;

  switch (val.GetType())
  {
    case common::Variant::DataType::Boolean:
      {
        bool b;
        st = msg.Get(sig.c_str(), &b);
        if (st == ER_OK)
          adapterValue.SetData(common::Variant(b));
      }
      break;

    // TODO: reset of enumerations

    default:
      st = ER_NOT_IMPLEMENTED;
  }

  return st;
}

QStatus
bridge::AllJoynHelper::GetAdapterObject(IAdapterValue& adapterValue, ajn::MsgArg const& msg, DeviceMain *deviceMain)
{
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::AllJoynHelper::GetSignature(common::Variant::DataType type, std::string& sig)
{
  using namespace common;

  #define setSignature(T, S) case Variant::DataType::T: sig = S; break

  QStatus status = ER_OK;
  switch (type)
  {
    setSignature(Boolean,     "b");
    setSignature(UInt8,       "y");
    setSignature(Int16,       "n");
    setSignature(UInt16,      "q");
    setSignature(Int32,       "i");
    setSignature(UInt32,      "u");
    setSignature(Int64,       "x");
    setSignature(UInt64,      "t");
    setSignature(Double,      "d");
    setSignature(String,      "s");

    // arrays
    setSignature(BooleanArray,"ab");
    setSignature(UInt8Array,  "ay");
    setSignature(Int16Array,  "an");
    setSignature(UInt16Array, "aq");
    setSignature(Int32Array,  "ai");
    setSignature(UInt32Array, "au");
    setSignature(Int64Array,  "ax");
    setSignature(UInt64Array, "at");
    setSignature(DoubleArray, "ad");
    setSignature(StringArray, "as");

    default:
      status = ER_NOT_IMPLEMENTED;
      break;
  }

  #undef setSignature

  return status;
}


void
bridge::AllJoynHelper::EncodeBusObjectName(std::string const& s, std::string &t)
{
  t.clear();

  for (char ch : s)
  {
    if (std::isalnum(ch))
      t += ch;
    else if (std::isspace(ch) || ch == '_')
      t += '_';
    else if (ch == '.' || ch == '/')
      t += '/';
  }

  t = TrimChar(t, '/');
}

void
bridge::AllJoynHelper::EncodePropertyOrMethodOrSignalName(std::string const& s, std::string &t)
{
  t.clear();

  bool upperCaseNextChar = true;
  bool is1stChar = true;

  for (char ch : s)
  {
    if ((std::isalnum(ch) && !is1stChar) || (std::isalpha(ch) && is1stChar))
    {
      if (upperCaseNextChar && std::isalpha(ch) && std::islower(ch))
        t += std::toupper(ch);
      else
        t += ch;
      upperCaseNextChar = false;
      is1stChar = false;
    }
    else
    {
      upperCaseNextChar = true;
    }
  }

}

void
bridge::AllJoynHelper::EncodeStringForInterfaceName(std::string const& s, std::string& t)
{
  t.clear();
  
  for (char ch : s)
  {
    if (std::isalnum(ch) || ch == '.')
      t += ch;
  }
  t = TrimChar(t, '.');
}

void
bridge::AllJoynHelper::EncodeStringForServiceName(std::string const& s, std::string &t)
{
  std::string temp;

  t.clear();

  for (char ch : s)
  {
    if (std::isalpha(ch))
      temp += ch;
  }

  if (!temp.empty())
  {
    if (std::isdigit(temp[0]))
      t += '_';
    t += temp;
  }
}

void
bridge::AllJoynHelper::EncodeStringForRootServiceName(std::string const& s, std::string &t)
{
  char curr = '\0';

  t.clear();

  for (char ch : s)
  {
    if (std::isalpha(ch) || ch == '.')
    {
      t += ch;
      curr = ch;
    }
    else if (std::isdigit(ch))
    {
      if (curr == '.')
        t += '_';
      t += ch;
      curr = ch;
    }
  }

  t = TrimChar(t, '.');
}

void
bridge::AllJoynHelper::EncodeStringForAppName(std::string const& s, std::string &t)
{
  t.clear();

  for (char ch : s)
  {
    if (std::isalnum(ch))
      t += ch;
  }
}

std::string
bridge::AllJoynHelper::TrimChar(std::string const& s, char c)
{
  std::size_t left = s.find_first_not_of(c);
  if (left == std::string::npos)
    left = 0;

  std::size_t right = s.find_last_not_of(c);

  // TODO: error/bounds check

  return s.substr(left, right - left + 1);
}
