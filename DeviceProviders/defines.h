#pragma once

#ifdef __APPLE__
#define QCC_OS_GROUP_POSIX
#include <memory>
using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::enable_shared_from_this;
#endif

class AllJoynBusObject;

