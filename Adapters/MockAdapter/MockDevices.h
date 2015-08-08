#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <alljoyn/Message.h>

namespace AdapterLib
{
  static const int kMockValueMaxSize = 128;
  static const int kMockPropertyMaxValues = 10;
  static const int kMockDeviceMaxProperties = 10;
  static const int32_t kLastDescriptorId = -1;

  enum class MockPropertyUnits
  {
    NoUnits,
    Percent,
    DegressFahrenheit
  };

  enum class MockPropertyAccess
  {
    Invalid,
    ReadOnly,
    ReadWrite
  };

  enum class MockPropertyType
  {
    AnalogInput,
    AnalogOutput,
    AnalogValue,
    BinaryInput,
    BinaryOutput,
    BinaryValue
  };

  enum class MockPropertyAttributeId
  {
    Name,
    Type,
    Id,
    Access,
    Units,
    PresentValue
  };

  struct MockValueDescriptor
  {
    ajn::AllJoynTypeId      Type;
    std::string             Name;
    struct 
    {
      double                AsSimpleType;
      uint8_t               AsBytes[kMockValueMaxSize];
      std::string           AsString;
    } InitialValue;
  };

  struct MockPropertyDescriptor
  {
    int32_t                  Id;
    MockPropertyType        Type;
    std::string             Name;
    MockPropertyAccess      Access;
    MockPropertyUnits       Units;
    MockValueDescriptor     CurrentValue;
  };

  struct MockDeviceDescriptor
  {
    int32_t                 Id;
    std::string             Name;
    std::string             VendorName;
    std::string             Model;
    std::string             SerialNumber;
    std::string             Version;
    std::string             Description;
    MockPropertyDescriptor  PropertyDescriptors[kMockDeviceMaxProperties];
  };

  std::vector<MockDeviceDescriptor> GetMockDevices();
  std::string PropertyUnitsToString(MockPropertyUnits units);
  std::string AccessToString(MockPropertyAccess access);
  std::string PropertyTypeToString(MockPropertyType type);
  std::string PropertyAttributeIdToString(MockPropertyAttributeId attrid);
}

