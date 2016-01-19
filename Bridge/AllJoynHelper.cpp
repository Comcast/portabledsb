
#include "AllJoynHelper.h"
#include "Common/Assert.h"


QStatus
bridge::AllJoynHelper::SetMsgArg(adapter::Value const& adapterValue, ajn::MsgArg& m)
{
  QStatus st = ER_OK;
  std::string sig;
  adapter::Variant const& val = adapterValue.GetValue();

  sig = GetSignature(val.GetType());

  switch (val.GetType())
  {
    case adapter::TypeId::Null:
    break;

    case adapter::TypeId::Boolean:
    {
      bool b = val.ToBoolean();
      st = m.Set(sig.c_str(), &b);
    }
    break;

    case adapter::TypeId::UInt8:
    {
      uint8_t i = val.ToUInt8();
      st = m.Set(sig.c_str(), &i);
    }
    break;

    case adapter::TypeId::Int16:
    {
      int16_t i = val.ToInt16();
      st = m.Set(sig.c_str(), &i);
    }
    break;

    case adapter::TypeId::UInt16:
    {
      uint16_t u = val.ToUInt16();
      st = m.Set(sig.c_str(), &u);
    }
    break;

    case adapter::TypeId::Int32:
    {
      int32_t i = val.ToInt32();
      st = m.Set(sig.c_str(), &i);
    }
    break;

    case adapter::TypeId::UInt32:
    {
      uint32_t u = val.ToUInt32();
      st = m.Set(sig.c_str(), &u);
    }
    break;

    case adapter::TypeId::Int64:
    {
      int64_t l = val.ToInt64();
      st = m.Set(sig.c_str(), &l);
    }
    break;

    case adapter::TypeId::UInt64:
    {
      uint64_t l = val.ToUInt64();
      st = m.Set(sig.c_str(), &l);
    }
    break;

    case adapter::TypeId::Double:
    {
      double d = val.ToDouble();
      st = m.Set(sig.c_str(), &d);
    }
    break;

    case adapter::TypeId::String:
    {
      std::string s = val.ToString();
      if (!s.empty())
      {
        st = m.Set(sig.c_str(), s.c_str());
        if (st == ER_OK)
          m.Stabilize();
      }
      else
      {
        st = m.Set(sig.c_str(), "");
      }
    }
    break;

    case adapter::TypeId::BooleanArray:
      /* Using vector<uint8_t> instead of vector<bool>, since vector<bool> is a
       * bitfield and we can't access the underlying array easily.
       * http://en.cppreference.com/w/cpp/container/vector_bool */
      st = SetMsgArg<uint8_t>(m, sig.c_str(), val.ToUInt8Array());
    break;

    case adapter::TypeId::UInt8Array:
      st = SetMsgArg<uint8_t>(m, sig.c_str(), val.ToUInt8Array());
    break;

    case adapter::TypeId::Int16Array:
      st = SetMsgArg<int16_t>(m, sig.c_str(), val.ToInt16Array());
    break;

    case adapter::TypeId::UInt16Array:
      st = SetMsgArg<uint16_t>(m, sig.c_str(), val.ToUInt16Array());
    break;

    case adapter::TypeId::Int32Array:
      st = SetMsgArg<int32_t>(m, sig.c_str(), val.ToInt32Array());
    break;

    case adapter::TypeId::UInt32Array:
      st = SetMsgArg<uint32_t>(m, sig.c_str(), val.ToUInt32Array());
    break;

    case adapter::TypeId::Int64Array:
      st = SetMsgArg<int64_t>(m, sig.c_str(), val.ToInt64Array());
    break;

    case adapter::TypeId::UInt64Array:
      st = SetMsgArg<uint64_t>(m, sig.c_str(), val.ToUInt64Array());
    break;

    case adapter::TypeId::DoubleArray:
      st = SetMsgArg<double>(m, sig.c_str(), val.ToDoubleArray());
    break;

    case adapter::TypeId::StringArray:
      st = SetMsgArg<std::string>(m, sig.c_str(), val.ToStringArray());
    break;
  }

  return st;
}

template<>
QStatus bridge::AllJoynHelper::SetMsgArg(ajn::MsgArg& msg, std::string const& sig, std::vector<std::string> const& arr)
{
  QStatus st = ER_OK;

  if (!arr.empty())
  {
    int n = static_cast<int>(arr.size());

    typedef char const* value_type;
    value_type* p = new value_type[n];

    for (int i = 0; i < n; ++i)
      p[i] = arr[i].c_str();

    st = msg.Set(sig.c_str(), n, p);
    msg.Stabilize();
  }
  else
  {
    st = msg.Set(sig.c_str(), 1, "");
    msg.Stabilize();
  }

  return st;
}
   
QStatus
bridge::AllJoynHelper::SetMsgArgFromAdapterObject(adapter::Value const& adapterValue, ajn::MsgArg&)
{
  // TODO:
  QStatus st = ER_OK;
  adapter::Variant const& val = adapterValue.GetValue();
  QCC_UNUSED(val);
  // std::string path = deviceMain->GetBusObjectPath(adapterValue);

  return st;
}

QStatus
bridge::AllJoynHelper::GetValue(adapter::Value& adapterValue, ajn::MsgArg const& msg)
{
  QStatus                 st;
  std::string             sig;
  adapter::Variant const&  val = adapterValue.GetValue();

  // TODO: why not just hard-code signatures? GetSignature is never used outside
  // AllJoynHelper
  sig = GetSignature(val.GetType());

  switch (val.GetType())
  {
    case adapter::TypeId::Boolean:
      {
        bool b;
        st = msg.Get(sig.c_str(), &b);
        if (st == ER_OK)
          adapterValue.SetValue(adapter::Variant(b));
      }
      break;

    // TODO: reset of enumerations

    default:
      st = ER_NOT_IMPLEMENTED;
  }

  return st;
}

QStatus
bridge::AllJoynHelper::GetAdapterObject(adapter::Value&, ajn::MsgArg const&)
{
  return ER_NOT_IMPLEMENTED;
}

std::string
bridge::AllJoynHelper::GetSignature(adapter::TypeId type)
{
  std::string sig;

  #define setSignature(T, S) case adapter::TypeId::T: sig = S; break

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
      DSB_ASSERT(false);
      break;
  }

  #undef setSignature

  return sig;
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

std::string
bridge::AllJoynHelper::EncodeStringForServiceName(std::string const& s)
{
  std::string t;
  std::string temp;

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
  return t;
}

std::string
bridge::AllJoynHelper::EncodeStringForRootServiceName(std::string const& s)
{
  std::string t;
  char curr = '\0';

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
  return t;
}

std::string
bridge::AllJoynHelper::EncodeStringForAppName(std::string const& s)
{
  std::string t;
  for (char ch : s)
  {
    if (std::isalnum(ch))
      t += ch;
  }
  return t;
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
