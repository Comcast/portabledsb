#pragma once

#include "Common/defines.h"

#include <alljoyn/MsgArg.h>
#include <alljoyn/Status.h>

#include <string>
#include <vector>

namespace Bridge
{
  std::string const kDeviceArrivalSignal = "Device_Arrival";
  std::string const kDeviceArravalHandle = "Device_Handle";
  std::string const kDeviceRemovalSignal = "Device_Removal";
  std::string const kDeviceRemovalDeviceHandle = "Device_Handle";
  std::string const kChangeOfValueSignal = "Change_Of_Value";
  std::string const kCovPropertyHandle = "Property_Handle";
  std::string const kCovAttributeHandle = "Attribute_Handle";

  class IAdapterValue;
  class IAdapterProperty;
  class IAdapterMethod;
  class IAdapterSignal;
  class IAdapterDevice;

  typedef std::vector< shared_ptr<IAdapterValue> > AdapterValueVector;
  typedef std::vector< shared_ptr<IAdapterProperty> > AdapterPropertyVector;
  typedef std::vector< shared_ptr<IAdapterMethod> > AdapterMethodVector;
  typedef std::vector< shared_ptr<IAdapterSignal> > AdapterSignalVector;
  typedef std::vector< shared_ptr<IAdapterDevice> > AdapterDeviceVector;

  class IAdapterValue
  {
  public:
    virtual ~IAdapterValue() { }
    virtual std::string GetName() = 0;
    virtual ajn::MsgArg GetData() = 0;
    virtual void SetData(ajn::MsgArg const& msg) = 0;
  };

  class IAdapterProperty
  {
  public:
    virtual ~IAdapterProperty() { }
    virtual std::string GetName() = 0;
    virtual AdapterValueVector GetAttributes() = 0;
  };

  class IAdapterMethod
  {
  public:
    virtual ~IAdapterMethod() { }
    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() = 0;

    virtual AdapterValueVector const& GetInputParams() const = 0;
    virtual AdapterValueVector const& GetOutputParams() const = 0;

    virtual void SetInputParams(AdapterValueVector const& params) = 0;
    virtual void SetOutputParams(AdapterValueVector const& params) = 0;

    virtual QStatus GetResult() = 0;
  };

  class IAdapterSignal
  {
  public:
    virtual ~IAdapterSignal() { }
    virtual std::string GetName() const = 0;
    virtual AdapterValueVector const& GetParams() const = 0;
  };

  class IAdapterSignalListener
  {
  public:
    virtual ~IAdapterSignalListener() { }
    virtual void AdapterSignalHandler(IAdapterSignal const& signal, void* argp) = 0;
  };

  class IAdapterDevice
  {
  public:
    virtual ~IAdapterDevice() { }
    virtual std::string GetName() = 0;
    virtual std::string GetVendor() = 0;
    virtual std::string GetModel() = 0;
    virtual std::string GetVersion() = 0;
    virtual std::string GetFirmwareVersion() = 0;
    virtual std::string GetSerialNumber() = 0;
    virtual std::string GetDescription() = 0;

    virtual AdapterPropertyVector const& GetProperties() const = 0;
    virtual AdapterMethodVector const& GetMethods() const = 0;
    virtual AdapterSignalVector const& GetSignals() const = 0;
  };

  class IAdapterIoRequest
  {
  public:
    virtual QStatus Status() = 0;;
    virtual QStatus Wait(uint32_t timeoutMillis) = 0;
    virtual QStatus Cancel() = 0;
    virtual QStatus Release() = 0;
  };

  enum class EnumDeviceOptions
  {
    CacheOnly = 0,
    ForceRefresh = 1
  };

  class IAdapter
  {
  protected:
    IAdapter() { }
  public:
    virtual ~IAdapter() { }
    virtual std::string GetVendor() = 0;
    virtual std::string GetAdapterName() = 0;
    virtual std::string GetVersion() = 0;
    virtual std::string GetExposedAdapterPrefix() = 0;
    virtual std::string GetExposedApplicationName() = 0;
    virtual std::string GetExposedApplicationGuid() = 0;
    virtual AdapterSignalVector GetSignals() = 0;

    virtual QStatus Initialize() = 0;
    virtual QStatus Shutdown() = 0;

    virtual QStatus EnumDevices(
      EnumDeviceOptions opts,
      AdapterDeviceVector& deviceList,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual QStatus GetProperty(
      shared_ptr<IAdapterProperty>& prop,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual QStatus SetProperty(
      shared_ptr<IAdapterProperty> const& prop,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual QStatus GetPropertyValue(
      shared_ptr<IAdapterProperty> const& prop,
      std::string const& attributeName,
      shared_ptr<IAdapterValue>& value,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual QStatus SetPropertyValue(
      shared_ptr<IAdapterProperty> const& prop,
      shared_ptr<IAdapterValue> const& value,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual QStatus CallMethod(
      shared_ptr<IAdapterMethod>& method,
      IAdapterIoRequest** req) = 0;

    virtual QStatus RegisterSignalListener(
      shared_ptr<IAdapterSignal> const& signal,
      shared_ptr<IAdapterSignalListener> const& listener,
      void* argp) = 0;

    virtual QStatus UnregisterSignalListener(
      shared_ptr<IAdapterSignal> const& signal,
      shared_ptr<IAdapterSignalListener> const& listener) = 0;
  };
}

