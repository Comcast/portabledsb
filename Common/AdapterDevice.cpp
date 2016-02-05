#include "AdapterDevice.h"

using namespace adapter;

void
Device::AddChild(std::shared_ptr<Device> const&  dev)
{
  m_children.push_back(dev);
}
