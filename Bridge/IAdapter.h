#pragma once

#include "Common/defines.h"
#include "Common/Variant.h"

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

  std::string const kDsbDeviceNotificationSignal = "DeviceNotificationSignal";
  std::string const kDsbSendMsgToDeviceMethod = "SendMessageToDeviceSynchronous";


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
    virtual Common::Variant GetData() = 0;
    virtual void SetData(Common::Variant const& msg) = 0;
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

    virtual int32_t GetResult() = 0;
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
    virtual int32_t Status() = 0;;
    virtual int32_t Wait(uint32_t timeoutMillis) = 0;
    virtual int32_t Cancel() = 0;
    virtual int32_t Release() = 0;
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
    typedef int32_t RegistrationHandle;

    virtual ~IAdapter() { }
    virtual std::string GetVendor() = 0;
    virtual std::string GetAdapterName() = 0;
    virtual std::string GetVersion() = 0;
    virtual std::string GetExposedAdapterPrefix() = 0;
    virtual std::string GetExposedApplicationName() = 0;
    virtual std::string GetExposedApplicationGuid() = 0;
    virtual AdapterSignalVector GetSignals() = 0;

    virtual int32_t Initialize() = 0;
    virtual int32_t Shutdown() = 0;

    virtual int32_t EnumDevices(
      EnumDeviceOptions opts,
      AdapterDeviceVector& deviceList,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t GetProperty(
      shared_ptr<IAdapterProperty>& prop,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t SetProperty(
      shared_ptr<IAdapterProperty> const& prop,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t GetPropertyValue(
      shared_ptr<IAdapterProperty> const& prop,
      std::string const& attributeName,
      shared_ptr<IAdapterValue>& value,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t SetPropertyValue(
      shared_ptr<IAdapterProperty> const& prop,
      shared_ptr<IAdapterValue> const& value,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t CallMethod(
      shared_ptr<IAdapterMethod>& method,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t RegisterSignalListener(
      std::string const& signalName,
      shared_ptr<IAdapterSignalListener> const& listener,
      void* argp, 
      RegistrationHandle& handle) = 0;

    virtual int32_t UnregisterSignalListener(RegistrationHandle const& h) = 0;
  };
}

