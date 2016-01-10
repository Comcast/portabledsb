#pragma once

#include "Common/defines.h"
#include "Common/Guid.h"
#include "Common/Variant.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace bridge
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
  class IAdapterLog;

  typedef std::vector< std::shared_ptr<IAdapterValue> >    AdapterValueVector;
  typedef std::vector< std::shared_ptr<IAdapterProperty> > AdapterPropertyVector;
  typedef std::vector< std::shared_ptr<IAdapterMethod> >   AdapterMethodVector;
  typedef std::vector< std::shared_ptr<IAdapterSignal> >   AdapterSignalVector;
  typedef std::vector< std::shared_ptr<IAdapterDevice> >   AdapterDeviceVector;
  typedef std::map< std::string, std::string>         AnnotationMap;

  enum class AccessType
  {
    Read,
    Write,
    ReadWrite
  };

  enum class SignalBehavior
  {
    Unspecified,
    Never,
    Always,
    AlwaysWithNoValue
  };

  enum class AdapterLogLevel
  {
    Off = -1,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Fatal = 5
  };

  class IAdapterLog
  {
  public:
    virtual void Write(AdapterLogLevel level, char const* file, int line, char const* format, ...) = 0;
    virtual bool IsLevelEnabled(AdapterLogLevel level) = 0;
  };

  class IAdapterAttribute
  {
  public:
    virtual std::shared_ptr<IAdapterValue> GetValue() = 0;
    virtual AnnotationMap GetAnnotations() = 0;
    virtual AccessType GetAccess() = 0;
    virtual SignalBehavior GetChangeOfValueBehavior() = 0;
  };

  class IAdapterValue
  {
  public:
    virtual ~IAdapterValue() { }
    virtual std::string GetName() = 0;
    virtual common::Variant const& GetData() const = 0;
    virtual void SetData(common::Variant const& msg) = 0;
  };

  class IAdapterProperty
  {
  public:
    virtual ~IAdapterProperty() { }
    virtual std::string GetName() = 0;
    virtual std::string GetInterfaceHint() = 0;
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

  class IAdapterIcon
  {
  public:
    virtual std::vector<uint8_t> GetImage() = 0;
    virtual std::string GetMimeType() = 0;
    virtual std::string GetUrl() = 0;
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
    virtual std::shared_ptr<IAdapterIcon> GetIcon() = 0;

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
    virtual common::Guid GetExposedApplicationGuid() = 0;
    virtual AdapterSignalVector GetSignals() = 0;

    virtual int32_t SetConfiguration(std::vector<uint8_t> const& configData) = 0;
    virtual int32_t GetConfiguration(std::vector<uint8_t>* configData) = 0;

    virtual int32_t Initialize(std::shared_ptr<IAdapterLog> const& log) = 0;
    virtual int32_t Shutdown() = 0;

    virtual int32_t EnumDevices(
      EnumDeviceOptions opts,
      AdapterDeviceVector& deviceList,
      std::shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t GetProperty(
      std::shared_ptr<IAdapterProperty>& prop,
      std::shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t SetProperty(
      std::shared_ptr<IAdapterProperty> const& prop,
      std::shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t GetPropertyValue(
      std::shared_ptr<IAdapterProperty> const& prop,
      std::string const& attributeName,
      std::shared_ptr<IAdapterValue>& value,
      std::shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t SetPropertyValue(
      std::shared_ptr<IAdapterProperty> const& prop,
      std::shared_ptr<IAdapterValue> const& value,
      std::shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t CallMethod(
      std::shared_ptr<IAdapterMethod>& method,
      std::shared_ptr<IAdapterIoRequest>* req) = 0;

    virtual int32_t RegisterSignalListener(
      std::string const& signalName,
      std::shared_ptr<IAdapterSignalListener> const& listener,
      void* argp, 
      RegistrationHandle& handle) = 0;

    virtual int32_t UnregisterSignalListener(RegistrationHandle const& h) = 0;
  };


  // concrete implementations of some of the simple interfaces

  class AdapterAttribute : public IAdapterAttribute
  {
  public:
    AdapterAttribute(std::shared_ptr<IAdapterValue> const& value, AnnotationMap const& annotations,
      AccessType accessType, SignalBehavior signalBehavior)
        : m_adapterValue(value)
        , m_annotationMap(annotations)
        , m_accessType(accessType)
        , m_signalBehavior(signalBehavior)
    {
      // empty
    }

  public:
    virtual std::shared_ptr<IAdapterValue> GetValue()
      { return m_adapterValue; }

    virtual AnnotationMap GetAnnotations()
      { return m_annotationMap; }

    virtual AccessType GetAccess()
      { return m_accessType; }

    virtual SignalBehavior GetChangeOfValueBehavior()
      { return m_signalBehavior; }

  private:
    std::shared_ptr<IAdapterValue> m_adapterValue;
    AnnotationMap             m_annotationMap;
    AccessType                m_accessType;
    SignalBehavior            m_signalBehavior;
  };

  class AdapterValue : public IAdapterValue
  {
  public:
    AdapterValue(std::string const& name, common::Variant const& data)
      : m_name(name)
      , m_data(data)
    {
      // empty
    }

    virtual std::string GetName()
      { return m_name; }

    virtual common::Variant const& GetData() const
      { return m_data; }

  private:
    std::string       m_name;
    common::Variant   m_data;
  };
}

