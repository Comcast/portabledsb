#pragma once

#ifdef DSB_USE_STDTHREADS
#include <mutex>
#elif DSB_USE_POSIXTHREADS
#include <pthread.h>
#else
#error "Not implemented"
#endif


namespace common
{
class Mutex
{
public:
  Mutex();

  inline void Lock();
  inline void Unlock();

private:
  Mutex(Mutex const& /*rhs*/)
    { }
  Mutex& operator = (Mutex const& /*rhs*/)
    { return *this; }

private:
  #ifdef DSB_USE_STDTHREADS
  std::mutex m_mutex;
  #elif DSB_USE_POSIXTHREADS
  pthread_mutex_t m_mutex;
  #else
  #error "Not implemented"
  #endif
};

class LockGuard
{
public:
  LockGuard(Mutex& m) : m_mutex(m)
  {
    m_mutex.Lock();
  }

  ~LockGuard()
  {
    m_mutex.Unlock();
  }

private:
  LockGuard& operator= (LockGuard const& /*rhs*/) { return *this; }

private:
  Mutex& m_mutex;

};

#ifdef DSB_USE_STDTHREADS
inline common::Mutex::Mutex()
{
}

inline void common::Mutex::Lock()
{
  m_mutex.lock();
}

inline void common::Mutex::Unlock()
{
  m_mutex.unlock();
}

#elif DSB_USE_POSIXTHREADS
inline common::Mutex::Mutex()
{
  pthread_mutex_init(&m_mutex, NULL);
}

inline void common::Mutex::Lock()
{
  pthread_mutex_lock(&m_mutex);
}

inline void common::Mutex::Unlock()
{
  phread_mutex_unlock(&m_mutex);
}
#else
#error "Not implemented"
#endif
}

