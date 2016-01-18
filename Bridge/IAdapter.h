#pragma once

#include "Common/AdapterDevice.h"
#include "Common/AdapterProperty.h"
#include "Common/AccessType.h"
#include "Common/AdapterItemInformation.h"
#include "Common/AdapterMethod.h"
#include "Common/AdapterSignal.h"
#include "Common/AdapterSignalListener.h"
#include "Common/AdapterValue.h"
#include "Common/EnumDeviceOptions.h"
#include "Common/Guid.h"
#include "Common/Log.h"
#include "Common/SignalBehavior.h"
#include "Common/Variant.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace bridge
{
  typedef uint32_t AdapterStatus;

  inline bool IsOk(AdapterStatus st)
  {
    return st == 0;
  }

  class IAdapterIcon
  {
  public:
    virtual std::vector<uint8_t> GetImage() = 0;
    virtual std::string GetMimeType() = 0;
    virtual std::string GetUrl() = 0;
  };
}

