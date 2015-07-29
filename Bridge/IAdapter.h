#pragma once

#include "defines.h"
#include <alljoyn/MsgArg.h>

#include <string>
#include <vector>

namespace Bridge
{
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
    virtual std::string GetName() = 0;
    virtual ajn::MsgArg GetData() = 0;
    virtual void SetData(ajn::MsgArg const& msg) = 0;
  };

  class IAdapterProperty
  {
  public:
    virtual std::string GetName() = 0;
    virtual AdapterValueVector GetAttributes() = 0;
  };

  class IAdapterMethod
  {
  public:
    virtual std::string GetName() = 0;
    virtual std::string GetDescription() = 0;

    virtual AdapterValueVector GetInputParams() = 0;
    virtual AdapterValueVector GetOutputParams() = 0;

    virtual void SetInputParams(AdapterValueVector const& params) = 0;
    virtual void SetOutputParams(AdapterValueVector const& params) = 0;

    virtual int32_t GetResult() = 0;
  };

  class IAdapterSignal
  {
  public:
    virtual std::string GetName() = 0;
    virtual AdapterValueVector GetParams() = 0;
  };

  class IAdapterSignalListener
  {
  public:
    virtual void AdapterSignalHandler(IAdapterSignal const& signal, void* argp) = 0;
  };

  class IAdapterDevice
  {
  public:
    virtual std::string GetName() = 0;
    virtual std::string GetVendor() = 0;
    virtual std::string GetModel() = 0;
    virtual std::string GetVersion() = 0;
    virtual std::string GetFirmwareVersion() = 0;
    virtual std::string GetSerialNumber() = 0;
    virtual std::string GetDescription() = 0;

    virtual AdapterPropertyVector GetProperties() = 0;
    virtual AdapterMethodVector GetMethods() = 0;
    virtual AdapterSignalVector GetSignals() = 0;
  };

  class IAdapterIoRequest
  {
  public:
    virtual uint32_t Status() = 0;;
    virtual uint32_t Wait(uint32_t timeoutMillis) = 0;
    virtual uint32_t Cancel() = 0;
    virtual uint32_t Release() = 0;
  };

  enum class EnumDeviceOptions
  {
    CacheOnly = 0,
    ForceRefresh = 1
  };

  class IAdapter
  {
  public:
    virtual std::string GetVendor() = 0;
    virtual std::string GetAdapterName() = 0;
    virtual std::string GetVersion() = 0;
    virtual std::string GetExposedAdapterPrefix() = 0;
    virtual std::string GetExposedApplicationName() = 0;
    virtual std::string GetExposedApplicationGuid() = 0;
    virtual AdapterSignalVector GetSignals() = 0;

    virtual uint32_t Initialize() = 0;
    virtual uint32_t Shutdown();

    virtual uint32_t EnumDevices(
      EnumDeviceOptions opts,
      shared_ptr<AdapterDeviceVector>& deviceList,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual uint32_t GetProperty(
      shared_ptr<IAdapterProperty>& prop,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual uint32_t SetProperty(
      shared_ptr<IAdapterProperty> const& prop,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual uint32_t GetPropertyValue(
      shared_ptr<IAdapterProperty> const& prop,
      std::string const& attributeName,
      shared_ptr<IAdapterValue>& value,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual uint32_t SetPropertyValue(
      shared_ptr<IAdapterProperty> const& prop,
      shared_ptr<IAdapterValue> const& value,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual uint32_t CallMethod(
      shared_ptr<IAdapterMethod>& method,
      shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual uint32_t RegisterSignalListener(
      shared_ptr<IAdapterSignal> const& signal,
      shared_ptr<IAdapterSignalListener> const& listener,
      void* argp) = 0;

    virtual uint32_t UnregisterSignalListener(
      shared_ptr<IAdapterSignal> const& signal,
      shared_ptr<IAdapterSignalListener> const& listener) = 0;
  
  };
}

