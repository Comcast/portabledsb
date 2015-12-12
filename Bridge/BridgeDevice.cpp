#include "Bridge/BridgeDevice.h"

QStatus
bridge::BridgeDevice::Shutdown()
{
  QStatus st = ER_OK;
  return st;
}

QStatus
bridge::BridgeDevice::Initialize(shared_ptr<IAdapterDevice> const&)
{
  QStatus st = ER_OK;
  return st;
}

