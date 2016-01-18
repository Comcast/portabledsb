#pragma once

#include "Common/AdapterItemInformation.h"
#include "Common/AdapterInterface.h"

namespace common
{
  class AdapterDevice
  {
  public:
    typedef std::vector<AdapterDevice> Vector;

    AdapterItemInformation const& GetBasicInformation() const
      { return m_info; }

    void SetBasicInformation(AdapterItemInformation const& info)
      { m_info = info; }

    AdapterInterface::Vector const& GetInterfaces() const
      { return m_interfaces; }

    void AddInterface(AdapterInterface const& interface)
      { m_interfaces.push_back(interface); }

    void ClearInterfaces()
      { m_interfaces.clear(); }

  private:
    AdapterItemInformation          m_info;
    AdapterInterface::Vector        m_interfaces;
  };
}
