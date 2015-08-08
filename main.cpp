
#include "Bridge/Bridge.h"
#include "Bridge/IAdapter.h"
#include "Common/Log.h"

#include "Adapters/ZigBeeAdapter/ZigBeeAdapter.h"

namespace
{
  DSB_DECLARE_LOGNAME(Main);
}

int main(int /*argc*/, char* /*argv*/ [])
{
  #if 0
  Bridge::BridgeConfig config;
  config.FromFile("/some/file.xml");

  std::cout << config.GetBridgeKeyX() << std::endl;
  std::cout << config.GetDeviceKeyX() << std::endl;
  std::cout << config.GetDeviceUsername() << std::endl;
  std::cout << config.GetDevicePassword() << std::endl;
  std::cout << config.GetDeviceEcdheEcdsaPrivateKey() << std::endl;
  std::cout << config.GetDeviceEcdheEcdsaCertChain() << std::endl;
  std::cout << config.GetDefaultVisibility() << std::endl;

  Bridge::DsbObjectConfig obj;
  QStatus st = config.FindObject("/my/bus/object", obj);

  obj.SetId("/my/bus/object2");
  obj.SetDescription("This is bus object2");
  obj.SetIsVisible(true);
  config.AddObject(obj);

  obj.SetId("/my/bus/object3");
  obj.SetDescription("This is bus object3");
  obj.SetIsVisible(true);
  config.AddObject(obj);
  config.ToFile("/tmp/test.xml");
  config.FromFile("/tmp/test.xml");
  config.RemoveObject("/my/bus/object2");
  config.ToFile("/tmp/test2.xml");
  #endif

  QStatus st = ER_OK;

  // TODO: should be unique_ptr and ownership give to bridge
  shared_ptr<Bridge::IAdapter> adapter(new AdapterLib::ZigBeeAdapter());

  shared_ptr<Bridge::DeviceSystemBridge> bridge(new Bridge::DeviceSystemBridge(adapter));
  st = bridge->Initialize();
  if (st != ER_OK)
  {
    DSBLOG_ERROR("failed to initialize bridge: 0x%x", st);
    return 1;
  }

  return 0;
}

