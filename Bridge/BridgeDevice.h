#pragma once

#include "Bridge/IAdapter.h"
#include <alljoyn/Status.h>

namespace bridge
{
  class BridgeDevice
  {
  public:
    QStatus Shutdown();
    QStatus Initialize(shared_ptr<IAdapterDevice> const& dev);
  };
}

