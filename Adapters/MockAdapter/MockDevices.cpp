#include "Adapters/MockAdapter/MockAdapter.h"

namespace
{
  void AddClusterToDevice(common::AdapterDevice& dev, uint16_t clusterId)
  {
    std::shared_ptr<adapters::mock::InterfaceDefinition> def = adapters::mock::GetZigBeeCluster(clusterId);
    if (def)
    {
      common::AdapterInterface ifc(def->Name);
      ifc.AddAttribute("code", clusterId);
      for (auto const& m : def->Methods)
        ifc.AddMethod(m);
      for (auto const& p : def->Properties)
        ifc.AddProperty(p);
      for (auto const& s : def->Signals)
        ifc.AddSignal(s);
      dev.AddInterface(std::move(ifc));
    }
  }
}


common::AdapterDevice
adapters::mock::MockAdapter::CreateDoorWindowSensor()
{
  using namespace common;

  AdapterDevice dev;

  AdapterItemInformation info;
  info.SetName("MyDoorWindowSensor");
  info.SetVendor("Comcast");
  info.SetModel("801417");
  info.SetVersion("1.0");
  info.SetFirmwareVersion("1.0");
  info.SetSerialNumber("H123ALK");
  info.SetDescription("Door/Window sensor");
  dev.SetBasicInformation(info);

  AddClusterToDevice(dev, 0x0000); // basic
  AddClusterToDevice(dev, 0x000f); // binary input

  return std::move(dev);
}

