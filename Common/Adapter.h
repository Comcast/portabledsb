#pragma once

#include "Common/AdapterDevice.h"
#include "Common/AdapterIoRequest.h"
#include "Common/AdapterItemInformation.h"
#include "Common/AdapterLog.h"
#include "Common/AdapterSignal.h"
#include "Common/AdapterSignalListener.h"
#include "Common/AdapterStatus.h"
#include "Common/EnumDeviceOptions.h"
#include "Common/Guid.h"

#include <memory>

namespace adapter
{
  typedef uint32_t RegistrationHandle;

  class Adapter
  {
  protected:
    Adapter() { }

  public:
    virtual ~Adapter() { }

    virtual std::string
    GetAdapterPrefix() = 0;

    virtual std::string
    GetApplicationName() = 0;

    virtual Guid
    GetApplicationGuid() = 0;

    virtual std::string
    GetStatusText(adapter::Status st) = 0;

    virtual ItemInformation const&
    GetInfo() = 0;

    virtual adapter::Status SetConfiguration(
      std::vector<uint8_t> const& configData,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status GetConfiguration(
      std::vector<uint8_t>& configData,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status Initialize(
      std::shared_ptr<adapter::Log> const& log,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status Shutdown(
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status GetSignals(
      Signal::Vector& signals,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status EnumDevices(
      EnumDeviceOptions opts,
      Device::Vector& devices,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status GetProperty(
      Interface const& ifc,
      Property const& prop,
      NamedValue& value,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status SetProperty(
      Interface const& ifc,
      Property const& prop,
      NamedValue const& value,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status InvokeMethod(
      Interface const& ifc,
      Method const& method,
      IoRequest::Pointer const& req) = 0;

    virtual adapter::Status RegisterSignalListener(
        std::string const& signalName,
        SignalListener const& listener,
        void* argp,
        RegistrationHandle& handle) = 0;

    virtual adapter::Status UnregisterSignalListener(RegistrationHandle const& h) = 0;
  };
}
