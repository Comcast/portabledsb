
#include "Bridge/Bridge.h"
#include "Bridge/IAdapter.h"
#include "Common/Log.h"
#include "Adapters/MockAdapter/MockAdapter.h"

#include "Common/Variant.h"
#include <iostream>

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

  shared_ptr<Bridge::DeviceSystemBridge> bridge(new Bridge::DeviceSystemBridge(
    shared_ptr<Bridge::IAdapter>(new AdapterLib::MockAdapter())));

  st = bridge->Initialize();
  if (st != ER_OK)
  {
    DSBLOG_ERROR("failed to initialize bridge: 0x%x", st);
    return 1;
  }

  shared_ptr<Bridge::IAdapter> adapter = bridge->GetAdapter();

  Bridge::AdapterDeviceVector deviceList;
  shared_ptr<Bridge::IAdapterIoRequest> req;

    // TODO: do we need enums for this?
  int32_t ret = adapter->EnumDevices(Bridge::EnumDeviceOptions::ForceRefresh, deviceList, &req);
  if (ret != 0)
  {
    std::cout << "EnumDevices:" << ret << std::endl;
    exit(0);
  }

  for (auto const& d : deviceList)
  {
    std::cout << "Device" << std::endl;
    std::cout << "\tName:" << d->GetName() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
  }

  return 0;
}

