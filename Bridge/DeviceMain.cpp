#include "DeviceMain.h"

bridge::DeviceMain::DeviceMain()
  : m_indexForSignal(1)
  , m_indexForMethod(1)
  , m_registeredOnAllJoyn(false)
{
}

bridge::DeviceMain::~DeviceMain()
{
  Shutdown();
}

void
bridge::DeviceMain::Shutdown()
{
}

QStatus
bridge::DeviceMain::Initialize(shared_ptr<BridgeDevice> const& parent)
{
  QStatus st = ER_OK;
  return st;
}

bool
bridge::DeviceMain::IsMethodNameUnique(std::string const& name)
{
  return m_deviceMethods.find(name) != m_deviceMethods.end();
}

bool
bridge::DeviceMain::IsSignalNameUnique(std::string const& name)
{
  // TODO:
  return false;
}

