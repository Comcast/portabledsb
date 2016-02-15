#pragma once

#include "Common/AdapterItemInformation.h"
#include "Common/AdapterInterface.h"

namespace adapter
{
  class Device
  {
  public:
    typedef std::vector<Device> Vector;
    typedef std::vector< std::shared_ptr<Device> > ChildList;

    ItemInformation const& GetInfo() const
      { return m_info; }

    void SetBasicInfo(ItemInformation const& info)
      { m_info = info; }

    // interfaces
    void AddInterface(Interface const& interface)
      { m_interfaces.push_back(interface); }

    void ClearInterfaces()
      { m_interfaces.clear(); }

    inline Interface::Vector const& GetInterfaces() const
      { return m_interfaces; }

    // children
    void AddChild(std::shared_ptr<Device> const& dev);

    void ClearChildren()
      { m_children.clear(); }

    inline ChildList const& GetChildren() const
      { return m_children; }

  private:
    ItemInformation         m_info;
    Interface::Vector       m_interfaces;
    ChildList               m_children;
  };
}
