#pragma once

#include "Common/AdapterStatus.h"

#include <condition_variable>
#include <mutex>

namespace common
{
  class AdapterIoRequest
  {
  public:
    AdapterIoRequest()
      : m_status(0)
      , m_canceled(false)
      , m_completed(false)
    {
    }

    virtual ~AdapterIoRequest()
    {
    }

    virtual AdapterStatus GetStatus()
      { return m_status; }

    virtual bool Wait(uint32_t millis = std::numeric_limits<uint32_t>::max())
    {
      if (millis != std::numeric_limits<uint32_t>::max())
      {
        auto delay = std::chrono::system_clock::now() + std::chrono::milliseconds(millis);

        std::unique_lock<std::mutex> lk(m_mutex);
        if (m_cond.wait_until(lk, delay, [this] { return this->m_canceled || this->m_completed; }))
          return true;
        else
          return false;
      }
      else
      {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cond.wait(lk, [this] { return this->m_canceled || this->m_completed; });
      }

      return 0;
    }

    virtual void Cancel()
    {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_canceled = true;
      m_completed = true;
      lk.unlock();
      m_cond.notify_all();
    }

    void SetComplete(AdapterStatus status)
    {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_status = status;
      m_canceled = false;
      m_completed = true;
      lk.unlock();
      m_cond.notify_all();
    }

  private:
    AdapterStatus             m_status;
    bool                      m_canceled;
    bool                      m_completed;
    std::mutex                m_mutex;
    std::condition_variable   m_cond;
  };
}
