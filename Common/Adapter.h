#pragma once

#include "Common/AdapterDevice.h"
#include "Common/AdapterIoRequest.h"
#include "Common/AdapterItemInformation.h"
#include "Common/AdapterSignal.h"
#include "Common/AdapterSignalListener.h"
#include "Common/AdapterStatus.h"
#include "Common/EnumDeviceOptions.h"
#include "Common/Guid.h"
#include "Common/Log.h"

#include <memory>

namespace common
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
    virtual std::string GetStatusText(AdapterStatus st) = 0;

    virtual AdapterStatus SetConfiguration(
      std::vector<uint8_t> const& configData,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus GetConfiguration(
      std::vector<uint8_t>& configData,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus GetBasicInformation(
      AdapterItemInformation& info,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus Initialize(
      std::shared_ptr<Logger> const& log,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus Shutdown(
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus GetSignals(
      AdapterSignal::Vector& signals,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus EnumDevices(
      EnumDeviceOptions opts,
      AdapterDevice::Vector& devices,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus GetProperty(
      AdapterInterface const& ifc,
      AdapterProperty const& prop,
      AdapterValue& value,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus SetProperty(
      AdapterInterface const& ifc,
      AdapterProperty const& prop,
      AdapterValue const& value,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus InvokeMethod(
      AdapterInterface const& ifc,
      AdapterMethod const& method,
      std::shared_ptr<AdapterIoRequest>* req = NULL) = 0;

    virtual AdapterStatus RegisterSignalListener(
        std::string const& signalName,
        AdapterSignalListener const& listener,
        void* argp,
        RegistrationHandle& handle) = 0;

    virtual AdapterStatus UnregisterSignalListener(RegistrationHandle const& h) = 0;
  };
}
