#include "Mutex.h"

common::Mutex::Mutex()
{
}

void
common::Mutex::Lock()
{
  m_mutex.lock();
}

void
common::Mutex::Unlock()
{
  m_mutex.unlock();
}

