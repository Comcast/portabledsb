#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "defines.h"

namespace common
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
      String,

      // array types
      BooleanArray,
      UInt8Array,
      Int16Array,
      UInt16Array,
      Int32Array,
      UInt32Array,
      Int64Array,
      UInt64Array,
      DoubleArray,
      StringArray,
    };

    Variant();
    Variant(bool b);
    Variant(uint8_t y);
    Variant(int16_t n);
    Variant(uint16_t q);
    Variant(int32_t i);
    Variant(uint32_t u);
    Variant(int64_t x);
    Variant(uint64_t t);
    Variant(double d);
    Variant(DataType t);
    Variant(char const* s);
    Variant(std::string const& s);
    Variant(Variant const& other);

    Variant& operator=(Variant const& other);

    ~Variant();

    inline DataType GetType() const
      { return m_type; }

    inline bool ToBoolean(bool* ok = NULL) const
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

    std::vector<bool>         ToBooleanArray(bool* ok = NULL) const;
    std::vector<uint8_t>      ToUInt8Array(bool* ok = NULL) const;
    std::vector<int16_t>      ToInt16Array(bool* ok = NULL) const;
    std::vector<uint16_t>     ToUInt16Array(bool* ok = NULL) const;
    std::vector<int32_t>      ToInt32Array(bool* ok = NULL) const;
    std::vector<uint32_t>     ToUInt32Array(bool* ok = NULL) const;
    std::vector<int64_t>      ToInt64Array(bool* ok = NULL) const;
    std::vector<uint64_t>     ToUInt64Array(bool* ok = NULL) const;
    std::vector<double>       ToDoubleArray(bool* ok = NULL) const;
    std::vector<std::string>  ToStringArray(bool* ok = NULL) const;

    std::string ToString(bool* ok = NULL) const;

  private:
    bool CanConvert(DataType t) const;

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
      bool          v_bool;
      uint8_t       v_uint8;
      int16_t       v_int16;
      uint16_t      v_uint16;
      int32_t       v_int32;
      uint32_t      v_uint32;
      int64_t       v_int64;
      uint64_t      v_uint64;
      double        v_double;
      std::string*  v_string;
    } m_data;

    void AssignFrom(Variant const& other);
  };
}
