#pragma once

#include "Common/AdapterSignal.h"

namespace common
{
  typedef std::function<void (AdapterSignal const&, void*)> AdapterSignalListener;
}

