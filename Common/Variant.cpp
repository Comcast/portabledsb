#include "Variant.h"

common::Variant::Variant()
  : m_type(DataType::Invalid)
{
  memset(&m_data, 0, sizeof(m_data));
}

common::Variant::Variant(bool b)
  : m_type(DataType::Boolean)
{
  m_data.v_bool = b;
}

common::Variant::Variant(uint8_t y)
  : m_type(DataType::UInt8)
{
  m_data.v_uint8 = y;
}

common::Variant::Variant(int16_t n)
  : m_type(DataType::Int16)
{
  m_data.v_int16 = n;
}

common::Variant::Variant(uint16_t q)
  : m_type(DataType::UInt16)
{
  m_data.v_uint16 = q;
}

common::Variant::Variant(int32_t i)
  : m_type(DataType::Int32)
{
  m_data.v_int32 = i;
}

common::Variant::Variant(uint32_t u)
  : m_type(DataType::UInt32)
{
  m_data.v_uint32 = u;
}

common::Variant::Variant(int64_t x)
  : m_type(DataType::Int64)
{
  m_data.v_int64 = x;
}

common::Variant::Variant(uint64_t t)
  : m_type(DataType::UInt64)
{
  m_data.v_uint64 = t;
}

common::Variant::Variant(double d)
  : m_type(DataType::Double)
{
  m_data.v_double = d;
}

common::Variant::Variant(DataType t)
  : m_type(t)
{
  memset(&m_data, 0, sizeof(m_data));
}

common::Variant::Variant(std::string const& s)
  : m_type(DataType::String)
{
  if (!m_data.v_string)
    m_data.v_string = new std::string();
  *m_data.v_string = s;
}

common::Variant::Variant(char const* s)
  : m_type(DataType::String)
{
  if (s)
  {
    if (!m_data.v_string)
      m_data.v_string = new std::string();
    *m_data.v_string = s;
  }
}

common::Variant::Variant(Variant const& other)
{
  AssignFrom(other);
}

common::Variant::~Variant()
{
  if (m_type == DataType::String && m_data.v_string)
    delete m_data.v_string;
}

common::Variant&
common::Variant::operator=(Variant const& other)
{
  if (&other != this)
  {
    AssignFrom(other);
  }
  return *this;
}

std::vector<bool>
common::Variant::ToBooleanArray(bool* ok) const
{
  return std::vector<bool>();
}

std::vector<uint8_t>
common::Variant::ToUInt8Array(bool* ok) const
{
  return std::vector<uint8_t>();
}

std::vector<int16_t>
common::Variant::ToInt16Array(bool* ok) const
{
  return std::vector<int16_t>();
}

std::vector<uint16_t>
common::Variant::ToUInt16Array(bool* ok) const
{
  return std::vector<uint16_t>();
}

std::vector<int32_t>
common::Variant::ToInt32Array(bool* ok) const
{
  return std::vector<int32_t>();
}

std::vector<uint32_t>
common::Variant::ToUInt32Array(bool* ok) const
{
  return std::vector<uint32_t>();
}

std::vector<int64_t>
common::Variant::ToInt64Array(bool* ok) const
{
  return std::vector<int64_t>();
}

std::vector<uint64_t>
common::Variant::ToUInt64Array(bool* ok) const
{
  return std::vector<uint64_t>();
}

std::vector<double>
common::Variant::ToDoubleArray(bool* ok) const
{
  return std::vector<double>();
}

std::vector<std::string>
common::Variant::ToStringArray(bool* ok) const
{
  return std::vector<std::string>();
}

void
common::Variant::AssignFrom(Variant const& other)
{
  switch (other.m_type)
  {
    case DataType::Invalid: break;
    case DataType::Boolean: m_data.v_bool = other.m_data.v_bool; break;
    case DataType::UInt8:   m_data.v_uint8 = other.m_data.v_uint8; break;
    case DataType::Int16:   m_data.v_int16 = other.m_data.v_int16; break;
    case DataType::UInt16:  m_data.v_uint16 = other.m_data.v_uint16; break;
    case DataType::Int32:   m_data.v_int32 = other.m_data.v_int32; break;
    case DataType::UInt32:  m_data.v_uint32 = other.m_data.v_uint32; break;
    case DataType::Int64:   m_data.v_int64 = other.m_data.v_int64; break;
    case DataType::UInt64:  m_data.v_uint64 = other.m_data.v_uint64; break;
    case DataType::Double:  m_data.v_double = other.m_data.v_double; break;
    case DataType::String:
    {
      if (other.m_data.v_string)
      {
        if (!m_data.v_string)
          m_data.v_string = new std::string();
        *m_data.v_string = *other.m_data.v_string;
      }
    }
    break;
  }
  m_type = other.m_type;
}

bool
common::Variant::CanConvert(DataType t) const
{
  return m_type == t;
}

std::string
common::Variant::ToString(bool* ok) const
{
  // string handled special to allow for other data types
  // to be converted later.
  if (CanConvert(DataType::String))
  {
    if (ok) *ok = true;
    if (m_data.v_string) return *m_data.v_string;
    return std::string();
  }
  if (ok) *ok = false;
  return std::string();
}

