
#include "Bridge/Bridge.h"

#include "Common/Adapter.h"
#include "Common/Log.h"

#include "Adapters/MockAdapter/MockAdapter.h"

#include "Common/Variant.h"
#include <iostream>

namespace
{
  struct ObjectId
  {
    uint64_t DeviceId;
    uint64_t InterfaceId;
    uint64_t MemberId;

    bool operator < (ObjectId const& rhs) const
    {
      if (DeviceId == rhs.DeviceId)
        if (InterfaceId == rhs.InterfaceId)
          return MemberId < rhs.MemberId;
        return InterfaceId < rhs.InterfaceId;
      return DeviceId < rhs.DeviceId;
    }
  };

  DSB_DECLARE_LOGNAME(Main);

  inline std::shared_ptr<bridge::DeviceSystemBridge> DSB()
  {
    std::shared_ptr<bridge::DeviceSystemBridge> b = bridge::DeviceSystemBridge::GetInstance();
    assert(b.get() != nullptr);
    return b;
  }

  void PrintValue(common::AdapterInterface const& ifc, common::AdapterValue const& val)
  {
    std::cout << ifc.GetName() << "." << val.GetName();
    std::cout << " = " << val.GetValue().ToString();
    std::cout << " [" << common::TypeIdToString(val.GetValue().GetType()) << "]" << std::endl;
  }

  void PrintDevice(common::Adapter& adapter, common::AdapterDevice const& dev)
  {
    std::cout << "Device [" << dev.GetBasicInformation().GetName() << "]" << std::endl;
    for (auto ifc : dev.GetInterfaces())
    {
      std::cout << "InterfaceName:" << ifc.GetName() << std::endl;

      for (auto attr : ifc.GetAttributes())
        std::cout << "  [" << attr.first << "=" << attr.second.ToString() << "]" << std::endl;
      std::cout << std::endl;
     
      std::cout << "Properties" << std::endl;
      for (auto prop : ifc.GetProperties())
      {
        common::AdapterValue value = common::AdapterValue::Null();
        adapter.GetProperty(ifc, prop, value);

        std::cout << " ";
        PrintValue(ifc, value);

        for (auto attr : prop.GetAttributes())
          std::cout << "    [" << attr.first << "=" << attr.second.ToString() << "]" << std::endl;
      }
      std::cout << std::endl;

      std::cout << "Methods" << std::endl;
      for (auto method : ifc.GetMethods())
      {
        std::cout << "  " << method.GetName() << std::endl;
        for (auto attr : method.GetAttributes())
          std::cout << "    [" << attr.first << "=" << attr.second.ToString() << "]" << std::endl;
      }
      std::cout << std::endl;
      std::cout << std::endl;
    }
  }
}

int main(int /*argc*/, char* /*argv*/ [])
{
  #if 0
  bridge::bridgeConfig config;
  config.FromFile("/some/file.xml");

  std::cout << config.GetBridgeKeyX() << std::endl;
  std::cout << config.GetDeviceKeyX() << std::endl;
  std::cout << config.GetDeviceUsername() << std::endl;
  std::cout << config.GetDevicePassword() << std::endl;
  std::cout << config.GetDeviceEcdheEcdsaPrivateKey() << std::endl;
  std::cout << config.GetDeviceEcdheEcdsaCertChain() << std::endl;
  std::cout << config.GetDefaultVisibility() << std::endl;

  bridge::DsbObjectConfig obj;
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

  bridge::DeviceSystemBridge::InitializeSingleton(
      std::shared_ptr<common::Adapter>(new adapters::mock::MockAdapter()));

  st = DSB()->Initialize();
  if (st != ER_OK)
  {
    DSBLOG_ERROR("failed to initialize bridge: %s", QCC_StatusText(st));
    return 1;
  }

  std::shared_ptr<common::Adapter> adapter = DSB()->GetAdapter();

  common::AdapterItemInformation info;
  adapter->GetBasicInformation(info);

  common::Logger::GetLogger(info.GetName())->SetLevel(common::LogLevel::Debug);

  std::shared_ptr<common::AdapterIoRequest> req(new common::AdapterIoRequest());

  common::AdapterDevice::Vector devices;
  adapter->EnumDevices(common::EnumDeviceOptions::ForceRefresh, devices, &req);
  if (req)
  {
    DSBLOG_INFO("waiting for EnumDevices to complete");
    if (!req->Wait(20000))
      DSBLOG_INFO("EnumDevices timed out");
    else
      DSBLOG_INFO("EnumDevices completed");
  }

  for (auto const& device : devices)
    PrintDevice(*adapter, device);

  getchar();
  return 0;
}
