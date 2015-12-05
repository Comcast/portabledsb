#include "Bridge/BridgeDevice.h"

QStatus Bridge::BridgeDevice::Shutdown()
{
  QStatus st = ER_OK;
  return st;
}

QStatus Bridge::BridgeDevice::Initialize(shared_ptr<IAdapterDevice> const&)
{
  QStatus st = ER_OK;
  return st;
}

