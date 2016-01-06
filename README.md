# portabledsb

A portable C++ version of the AllJoyn DSB.

The AllSeen Alliance has a Device System Bridge (DSB), which allows non-allseen devices to
communicate on an AllJoyn bus. There's [a good document describing the software is here](https://git.allseenalliance.org/cgit/dsb.git/plain/Docs/AllJoyn%20-%20Device%20System%20Bridge%20-%20Whitepaper%20v1.0.pdf).

The code was contributed by Microsoft and is writting using C++/CLI (WinRT). This project's
goal is to re-create that software in standard C++ so that it can be run on Windows, Linux, and
Mac.

The WinRT version is here:

  <https://git.allseenalliance.org/cgit/dsb.git/>

## Dependencies

You will need libaries and development headers for:

  * [The AllJoyn standard core (C++)](https://allseenalliance.org/framework/download) v15.09a
  * libxml2
  * OpenSSL's libcrypto
  * libuuid

See: [AllJoyn's instructions for building on Linux](https://allseenalliance.org/framework/documentation/develop/building/linux)

### Ubuntu

    sudo apt-get install build-essential libcap-dev uuid-dev libxml2-dev libssl-dev

## Building

Right now we're only building on OSX/Linux.

 1. export ALLJOYN_INSTALL_DIR=[path to your build of alljoyn core]
 2. make [options]

`[options]`

  * DEBUG -- debug build (-g) otherwise -O2 is used
  * V     -- verbose output (skip if you want cleaner output)
