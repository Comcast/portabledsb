#include "Variant.h"
#include <string.h>
#include <sstream>

namespace
{
  template<class TFrom, class TOut>
  void printArray(std::stringstream& buff, void* arr, int n)
  {
    TFrom* typed_array = reinterpret_cast<TFrom *>(arr);
    for (int i = 0; i < n; ++i)
    {
      if (i > 0) buff << ',';
      buff << static_cast<TOut>(typed_array[i]);
    }
  }

  template<class T>
  void releaseArray(void* p)
  {
    T* arr = reinterpret_cast<T *>(p);
    delete [] arr;
  }

  void freeArray2(void* p, common::Variant::DataType t)
  {
    typedef common::Variant::DataType dt;
    switch (t)
    {
      case dt::BooleanArray:  releaseArray<bool>(p); break;
      case dt::UInt8Array:    releaseArray<uint8_t>(p); break;
      case dt::Int16Array:    releaseArray<int16_t>(p); break;
      case dt::UInt16Array:   releaseArray<uint16_t>(p); break;
      case dt::Int32Array:    releaseArray<int32_t>(p); break;
      case dt::UInt32Array:   releaseArray<uint32_t>(p); break;
      case dt::Int64Array:    releaseArray<int64_t>(p); break;
      case dt::UInt64Array:   releaseArray<uint64_t>(p); break;
      case dt::DoubleArray:   releaseArray<double>(p); break;
      case dt::StringArray:   releaseArray<std::string>(p); break;
      default:
        assert(false);
        break;
    }
  }
}

common::Variant::Data::Data()
{
  Type = DataType::Invalid;
  memset(&Item, 0, sizeof(m_data.Item));
  Size = 0;
}

common::Variant::Variant()
  : m_data()
{
}

common::Variant::Variant(bool b)
  : m_data()
{
  m_data.Type = DataType::Boolean;
  m_data.Item.v_bool = b;
}

common::Variant::Variant(std::vector<bool> const& v)
  : m_data()
{
  m_data.Type = DataType::BooleanArray;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(uint8_t y)
  : m_data()
{
  m_data.Type = DataType::UInt8;
  m_data.Item.v_uint8 = y;
}

common::Variant::Variant(std::vector<uint8_t> const& v)
  : m_data()
{
  m_data.Type = DataType::UInt8Array;
  AllocAndCopy(m_data, v);
}


common::Variant::Variant(int16_t n)
  : m_data()
{
  m_data.Type = DataType::Int16;
  m_data.Item.v_int16 = n;
}

common::Variant::Variant(std::vector<uint16_t> const& v)
  : m_data()
{
  m_data.Type = DataType::UInt16Array;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(uint16_t q)
  : m_data()
{
  m_data.Type = DataType::UInt16;
  m_data.Item.v_uint16 = q;
}

common::Variant::Variant(std::vector<int16_t> const& v)
  : m_data()
{
  m_data.Type = DataType::Int16Array;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(int32_t i)
  : m_data()
{
  m_data.Type = DataType::Int32;
  m_data.Item.v_int32 = i;
}

common::Variant::Variant(std::vector<int32_t> const& v)
  : m_data()
{
  m_data.Type = DataType::Int32Array;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(uint32_t u)
  : m_data()
{
  m_data.Type = DataType::UInt32;
  m_data.Item.v_uint32 = u;
}

common::Variant::Variant(std::vector<uint32_t> const& v)
  : m_data()
{
  m_data.Type = DataType::UInt32Array;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(int64_t x)
  : m_data()
{
  m_data.Type = DataType::Int64;
  m_data.Item.v_int64 = x;
}

common::Variant::Variant(std::vector<int64_t> const& v)
  : m_data()
{
  m_data.Type = DataType::Int64Array;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(uint64_t t)
  : m_data()
{
  m_data.Type = DataType::UInt64;
  m_data.Item.v_uint64 = t;
}

common::Variant::Variant(std::vector<uint64_t> const& v)
  : m_data()
{
  m_data.Type = DataType::UInt64Array;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(double d)
  : m_data()
{
  m_data.Type = DataType::Double;
  m_data.Item.v_double = d;
}

common::Variant::Variant(std::vector<double> const& v)
  : m_data()
{
  m_data.Type = DataType::DoubleArray;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(DataType t)
  : m_data()
{
  m_data.Type = t;
}

common::Variant::Variant(std::string const& s)
  : m_data()
{
  m_data.Type = DataType::String;
  if (!m_data.Item.v_string)
    m_data.Item.v_string = new std::string();
  *m_data.Item.v_string = s;
}

common::Variant::Variant(char const* s)
  : m_data()
{
  m_data.Type = DataType::String;
  if (s)
  {
    if (!m_data.Item.v_string)
      m_data.Item.v_string = new std::string();
    *m_data.Item.v_string = s;
  }
}

common::Variant::Variant(std::vector<std::string> const& v)
  : m_data()
{
  m_data.Type = DataType::StringArray;
  AllocAndCopy(m_data, v);
}

common::Variant::Variant(Variant const& other)
{
  AssignFrom(other);
}

common::Variant::~Variant()
{
  if (m_data.Type == DataType::String && m_data.Item.v_string)
    delete m_data.Item.v_string;

  if (IsArray() && m_data.Item.v_arr)
    freeArray2(m_data.Item.v_arr, m_data.Type);
}

common::Variant&
common::Variant::operator=(Variant const& other)
{
  if (&other != this)
    AssignFrom(other);
  return *this;
}

bool
common::Variant::IsArray() const
{
  // relies on knowing order of enum
  return GetType() > DataType::String;
}

int
common::Variant::Length() const
{
  if (!IsArray()) return -1;
  return m_data.Size;
}

std::vector<bool>
common::Variant::ToBooleanArray(bool* ok) const
{
  return GetArray<bool>(DataType::BooleanArray, ok);
}

std::vector<uint8_t>
common::Variant::ToUInt8Array(bool* ok) const
{
  return GetArray<uint8_t>(DataType::UInt8Array, ok);
}

std::vector<int16_t>
common::Variant::ToInt16Array(bool* ok) const
{
  return GetArray<int16_t>(DataType::Int16Array, ok);
}

std::vector<uint16_t>
common::Variant::ToUInt16Array(bool* ok) const
{
  return GetArray<uint16_t>(DataType::UInt16Array, ok);
}

std::vector<int32_t>
common::Variant::ToInt32Array(bool* ok) const
{
  return GetArray<int32_t>(DataType::Int32Array, ok);
}

std::vector<uint32_t>
common::Variant::ToUInt32Array(bool* ok) const
{
  return GetArray<uint32_t>(DataType::UInt32Array, ok);
}

std::vector<int64_t>
common::Variant::ToInt64Array(bool* ok) const
{
  return GetArray<int64_t>(DataType::Int64Array, ok);
}

std::vector<uint64_t>
common::Variant::ToUInt64Array(bool* ok) const
{
  return GetArray<uint64_t>(DataType::UInt64Array, ok);
}

std::vector<double>
common::Variant::ToDoubleArray(bool* ok) const
{
  return GetArray<double>(DataType::DoubleArray, ok);
}

std::vector<std::string>
common::Variant::ToStringArray(bool* ok) const
{
  return GetArray<std::string>(DataType::StringArray, ok);
}

void
common::Variant::AssignFrom(Variant const& other)
{
  switch (other.m_data.Type)
  {
    case DataType::Invalid: break;
    case DataType::Boolean: m_data.Item.v_bool = other.m_data.Item.v_bool; break;
    case DataType::UInt8:   m_data.Item.v_uint8 = other.m_data.Item.v_uint8; break;
    case DataType::Int16:   m_data.Item.v_int16 = other.m_data.Item.v_int16; break;
    case DataType::UInt16:  m_data.Item.v_uint16 = other.m_data.Item.v_uint16; break;
    case DataType::Int32:   m_data.Item.v_int32 = other.m_data.Item.v_int32; break;
    case DataType::UInt32:  m_data.Item.v_uint32 = other.m_data.Item.v_uint32; break;
    case DataType::Int64:   m_data.Item.v_int64 = other.m_data.Item.v_int64; break;
    case DataType::UInt64:  m_data.Item.v_uint64 = other.m_data.Item.v_uint64; break;
    case DataType::Double:  m_data.Item.v_double = other.m_data.Item.v_double; break;
    case DataType::String:
    {
      if (other.m_data.Item.v_string)
      {
        if (!m_data.Item.v_string)
          m_data.Item.v_string = new std::string();
        *m_data.Item.v_string = *other.m_data.Item.v_string;
      }
    }
    break;

    // arrays
    case DataType::BooleanArray:
      AssignFromArray<bool>(m_data, other.m_data);
      break;

    case DataType::UInt8Array:
      AssignFromArray<uint8_t>(m_data, other.m_data);
      break;

    case DataType::Int16Array:
      AssignFromArray<int16_t>(m_data, other.m_data);
      break;

    case DataType::UInt16Array:
      AssignFromArray<uint16_t>(m_data, other.m_data);
      break;

    case DataType::Int32Array:
      AssignFromArray<int32_t>(m_data, other.m_data);
      break;

    case DataType::UInt32Array:
      AssignFromArray<uint32_t>(m_data, other.m_data);
      break;

    case DataType::Int64Array:
      AssignFromArray<int64_t>(m_data, other.m_data);
      break;

    case DataType::UInt64Array:
      AssignFromArray<uint64_t>(m_data, other.m_data);
      break;

    case DataType::DoubleArray:
      AssignFromArray<double>(m_data, other.m_data);
      break;

    case DataType::StringArray:
      AssignFromArray<std::string>(m_data, other.m_data);
      break;
  }
  m_data.Type = other.m_data.Type;
}

bool
common::Variant::CanConvert(DataType t) const
{
  // TODO: better type conversion
  return m_data.Type == t;
}

std::string
common::Variant::ToString(bool* ok) const
{
  if (IsArray())
  {
    std::stringstream buff;
    buff << '[';
    switch (m_data.Type)
    {
      case DataType::Invalid: break;
      case DataType::BooleanArray: printArray<bool, bool>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::UInt8Array: printArray<uint8_t, int>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::Int16Array: printArray<int16_t, int>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::UInt16Array: printArray<uint16_t, int>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::Int32Array: printArray<int32_t, int>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::UInt32Array: printArray<uint32_t, uint32_t>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::Int64Array: printArray<int64_t, int64_t>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::UInt64Array: printArray<uint64_t, uint64_t>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::DoubleArray: printArray<double, double>(buff, m_data.Item.v_arr, m_data.Size); break;
      case DataType::StringArray: printArray<std::string, std::string>(buff, m_data.Item.v_arr, m_data.Size); break;
      default:
        assert(false);
        break;
    }
    buff << ']';
    return buff.str();
  }

  // string handled special to allow for other data types
  // to be converted later.
  if (CanConvert(DataType::String))
  {
    if (ok)
      *ok = true;
    if (m_data.Item.v_string)
      return *m_data.Item.v_string;
    return std::string();
  }
  if (ok)
    *ok = false;
  return std::string();
}
