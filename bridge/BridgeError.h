//
// Copyright (c) 2016, Comcast Cable Communications Management, LLC
//
// Permission to use, copy, modify, and/or distribute this software for any purpose with or
// without fee is hereby granted, provided that the above copyright notice and this
// permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
// THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT
// SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR
// ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
// OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
// USE OR PERFORMANCE OF THIS SOFTWARE.
//
#pragma once

#include "common/Adapter.h"
#include <alljoyn/Status.h>

#include <sstream>
#include <stdexcept>

namespace bridge
{
  class Error : public std::runtime_error
  {
  public:
    Error(std::string const& message);
    static void Throw(char const* fmt, ...);
    static void ThrowIfNotOk(QStatus st, char const* fmt, ...);
    static void Throw(adapter::Adapter& a, adapter::Status st, char const* fmt, ...);
  };
}
