#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string>

namespace Common
{
  class Variant
  {
  public:
   enum class DataType
    {
      Invalid,
      Boolean,
      UInt8,
      Int16,
      UInt16,
      Int32,
      UInt32,
      Int64,
      UInt64,
      Double,
      String
    };

    Variant()               : m_type(DataType::Invalid) { m_data.v_uint64 = 0; }
    Variant(bool b)         : m_type(DataType::Boolean) { m_data.v_bool = b; }
    Variant(uint8_t y)      : m_type(DataType::UInt8)   { m_data.v_uint8 = y; }
    Variant(int16_t n)      : m_type(DataType::Int16)   { m_data.v_int16 = n; }
    Variant(uint16_t q)     : m_type(DataType::UInt16)  { m_data.v_uint16 = q; }
    Variant(int32_t i)      : m_type(DataType::Int32)   { m_data.v_int32 = i; }
    Variant(uint32_t u)     : m_type(DataType::UInt32)  { m_data.v_uint32 = u; }
    Variant(int64_t x)      : m_type(DataType::Int64)   { m_data.v_int64 = x; }
    Variant(uint64_t t)     : m_type(DataType::UInt64)  { m_data.v_uint64 = t; }
    Variant(double d)       : m_type(DataType::Double)  { m_data.v_double = d; }

    Variant(std::string const& s) : m_type(DataType::String)
    {
      if (s.size())
        m_data.v_string = strdup(s.c_str());
      else
        m_data.v_string = NULL;
    }

    Variant(char const* s)  : m_type(DataType::String)
    {
      if (s)
        m_data.v_string = strdup(s);
    }

    Variant(Variant const& other)
    {
      AssignFrom(other);
    }

    Variant& operator=(Variant const& other)
    {
      if (&other != this)
      {
        AssignFrom(other);
      }
      return *this;
    }

    ~Variant()
    {
      if (m_type == DataType::String && m_data.v_string)
        free(m_data.v_string);
    }

    inline DataType GetType() const
      { return m_type; }

    inline bool ToBool(bool* ok = NULL) const
      { return Get<bool>(DataType::Boolean, ok); }

    inline uint8_t ToUInt8(bool* ok = NULL) const
      { return Get<uint8_t>(DataType::UInt8, ok); }

    inline int16_t ToInt16(bool* ok = NULL) const
      { return Get<int16_t>(DataType::Int16, ok); }

    inline uint16_t ToUInt16(bool* ok = NULL) const
      { return Get<uint16_t>(DataType::UInt16, ok); }

    inline int32_t ToInt32(bool* ok = NULL) const
      { return Get<int32_t>(DataType::Int32, ok); }

    inline uint32_t ToUInt32(bool* ok = NULL) const
      { return Get<uint32_t>(DataType::UInt32, ok); }

    inline int64_t ToInt64(bool* ok = NULL) const
      { return Get<int64_t>(DataType::Int64, ok); }

    inline uint64_t ToUInt64(bool* ok = NULL) const
      { return Get<uint64_t>(DataType::UInt64, ok); }

    inline double ToDouble(bool* ok = NULL) const
      { return Get<uint16_t>(DataType::Double, ok); }

    inline std::string ToString(bool* ok = NULL) const
    {
      // string handled special to allow for other data types
      // to be converted later.
      if (CanConvert(DataType::String))
      {
        if (ok)
          *ok = true;
        if (m_data.v_string)
          return std::string(m_data.v_string);
        return std::string();
      }
      if (ok)
        *ok = false;
      return std::string();
    }

  private:
    inline bool CanConvert(DataType t) const
    {
      // no conversion for now
      return m_type == t;
    }

    template<class T>
    T Convert(DataType t) const
    {
      // no conversion for now
      switch (t)
      {
        case DataType::Boolean: return m_data.v_bool;
        case DataType::UInt8:   return m_data.v_uint8;
        case DataType::Int16:   return m_data.v_int16;
        case DataType::UInt16:  return m_data.v_uint16;
        case DataType::Int32:   return m_data.v_int32;
        case DataType::UInt32:  return m_data.v_uint32;
        case DataType::Int64:   return m_data.v_int64;
        case DataType::UInt64:  return m_data.v_uint64;
        case DataType::Double:  return m_data.v_double;
        default:
          DSB_ASSERT(false);
          break;
      }
    }

    template<class T>
    T Get(DataType t, bool* ok) const
    {
      if (CanConvert(t))
      {
        if (*ok)
          *ok = true;
        return Convert<T>(t);
      }
      if (ok)
        *ok = false;
      return T();
    }
   
    DataType m_type;

    union {
      bool      v_bool;
      uint8_t   v_uint8;
      int16_t   v_int16;
      uint16_t  v_uint16;
      int32_t   v_int32;
      uint32_t  v_uint32;
      int64_t   v_int64;
      uint64_t  v_uint64;
      double    v_double;
      char*     v_string;
    } m_data;

    void AssignFrom(Variant const& other)
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
            if (m_data.v_string)
              free(m_data.v_string);
            m_data.v_string = strdup(other.m_data.v_string);
          }
        }
        break;
      }
      m_type = other.m_type;
    }
  };
}

