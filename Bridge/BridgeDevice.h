#pragma once

#include "Bridge/IAdapter.h"
#include <alljoyn/Status.h>

namespace Bridge
{
  class BridgeDevice
  {
  public:
    QStatus Shutdown();
    QStatus Initialize(shared_ptr<IAdapterDevice> const& dev);

  private:
  };
}

