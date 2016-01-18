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

    virtual std::string GetExposedAdapterPrefix() = 0;
    virtual std::string GetExposedApplicationName() = 0;
    virtual Guid GetExposedApplicationGuid() = 0;
    virtual std::string GetStatusText(adapter::Status st) = 0;

    virtual adapter::Status SetConfiguration(
      std::vector<uint8_t> const& configData,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status GetConfiguration(
      std::vector<uint8_t>& configData,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status GetBasicInformation(
      ItemInformation& info,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status Initialize(
      std::shared_ptr<adapter::Log> const& log,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status Shutdown(
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status GetSignals(
      Signal::Vector& signals,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status EnumDevices(
      EnumDeviceOptions opts,
      Device::Vector& devices,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status GetProperty(
      Interface const& ifc,
      Property const& prop,
      Value& value,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status SetProperty(
      Interface const& ifc,
      Property const& prop,
      Value const& value,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status InvokeMethod(
      Interface const& ifc,
      Method const& method,
      std::shared_ptr<IoRequest> const& req) = 0;

    virtual adapter::Status RegisterSignalListener(
        std::string const& signalName,
        SignalListener const& listener,
        void* argp,
        RegistrationHandle& handle) = 0;

    virtual adapter::Status UnregisterSignalListener(RegistrationHandle const& h) = 0;
  };
}
