#include "Bridge/ConfigManager.h"
#include "Bridge/IAdapter.h"

using namespace Bridge;

namespace
{
  // TODO: make this configurable;
  std::string const kBridgeConfigFile = "BridgeConfig.xml";
  int const kMaxConnectionAttempts = 60;
  int const kReconnectDelay = 500; 
}

ConfigManager::ConfigManager()
  : m_busAttachment(NULL)
{
}

ConfigManager::~ConfigManager()
{
  Shutdown();
}

int32_t ConfigManager::Initialize(shared_ptr<DeviceSystemBridge> const& bridge)
{
  m_parent = bridge;
  m_bridgeConfig.FromFile(kBridgeConfigFile);
  return 0;
}

int32_t ConfigManager::Shutdown()
{
  ShutdownAllJoyn();
  m_adapter.reset();
  m_parent.reset();
  return 0;
}

int32_t ConfigManager::ShutdownAllJoyn()
{
  return 0;
}

int32_t ConfigManager::ConnectToAllJoyn(shared_ptr<IAdapter> const& adapter)
{
  m_adapter = adapter;

  int ret = BuildServiceName();
  if (ret != 0)
  {
    return -1;
  }

  m_busAttachment.reset();
  m_busAttachment.reset(new ajn::BusAttachment(m_adapter->GetExposedApplicationName().c_str(), true));
  m_busAttachment->RegisterBusListener(m_busListener);
  m_busAttachment->Start();

  return 0;
}

int32_t ConfigManager::BuildServiceName()
{
  return 0;
}


