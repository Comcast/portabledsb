#include "DeviceMethod.h"

#include "Common/Log.h"
#include "Bridge/AllJoynHelper.h"
#include "Bridge/Bridge.h"

namespace
{
  DSB_DECLARE_LOGNAME(DeviceMethod);
}

bridge::DeviceMethod::DeviceMethod(DeviceMain& dev)
  : m_parent(dev)
{
}

bridge::DeviceMethod::~DeviceMethod()
{
}

uint32_t
bridge::DeviceMethod::InvokeMethod(ajn::Message& msg, std::vector<ajn::MsgArg>& outArgs)
{
  int i = 0;
  QStatus status = ER_OK;
  std::shared_ptr<common::AdapterIoRequest> request;

  outArgs.clear();

  for (auto const &p : m_adapterMethod->GetInputArguments())
  {
    common::AdapterValue param = p;

    ajn::MsgArg const* inArg = msg->GetArg(i++);
    if (inArg == nullptr)
      return 1;

    // TODO: this seems odd. The GetAdapterValue() populates the param argument with the
    // data from the inArg. Why wouldn't we create a temporary based on the incoming
    // ajn::Message? We need the description or prototype to do some validation. I would
    // think the m_adapterMethod would be const. I guess DeviceMethod (this class) can
    // only be used once and then must be discarded, or only used by one thread to
    // completion at any given time
    status = AllJoynHelper::GetAdapterValue(param, *inArg);
    if (status != ER_OK)
      return 1;
  }

  // TODO: we're ignoring the return value here
  #if 0
  bridge::DeviceSystemBridge::GetInstance()->GetAdapter()
    ->InvokeMethod(*m_adapterMethod, &request);
  #endif

  // TODO: timeout should be configurable or passed in by caller
  if (request)
    request->Wait(2000);

  i = 0;
  for (auto const& param : m_adapterMethod->GetOutputArguments())
  {
    status = AllJoynHelper::SetMsgArg(param, outArgs[i]);
    if (status != ER_OK)
      return 1;
  }

  return 0;
}

QStatus
bridge::DeviceMethod::Initialize(common::AdapterMethod const&)
{
  DSBLOG_NOT_IMPLEMENTED();
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::DeviceMethod::SetName(std::string const&)
{
  DSBLOG_NOT_IMPLEMENTED();
  return ER_NOT_IMPLEMENTED;
}

QStatus
bridge::DeviceMethod::BuildSignature(common::AdapterValue::Vector const&, std::string&, std::string&)
{
  DSBLOG_NOT_IMPLEMENTED();
  return ER_NOT_IMPLEMENTED;
}

