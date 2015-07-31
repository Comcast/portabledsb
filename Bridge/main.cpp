
#include "Bridge/BridgeConfig.h"
#include <iostream>

int main(int /*argc*/, char* /*argv*/ [])
{
  Bridge::BridgeConfig config;
  config.FromFile("/Users/jgladi200/Work/DeviceSystemBridge/dsb/Samples/ZWaveAdapter/BridgeConfig.xml");

  std::cout << config.GetBridgeKeyX() << std::endl;
  std::cout << config.GetDeviceKeyX() << std::endl;
  std::cout << config.GetDeviceUsername() << std::endl;
  std::cout << config.GetDevicePassword() << std::endl;
  std::cout << config.GetDeviceEcdheEcdsaPrivateKey() << std::endl;
  std::cout << config.GetDeviceEcdheEcdsaCertChain() << std::endl;
  std::cout << config.GetDefaultVisibility() << std::endl;

  Bridge::DsbObjectConfig obj;
  QStatus st = config.FindObject("/my/bus/object", obj);
  std::cout << "Found:" << st << std::endl;

  return 0;
}

