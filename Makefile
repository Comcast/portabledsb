SRCS=\
     bridge/AllJoynAbout.cpp \
     bridge/AllJoynHelper.cpp \
     bridge/Bridge.cpp \
     bridge/BridgeBusObject.cpp \
     bridge/BridgeConfig.cpp \
     bridge/ConfigManager.cpp \
     common/AdapterLog.cpp \
     main.cpp

ADAPTER_SRCS=\
     adapters/mock/MockAdapter.cpp \
     adapters/mock/ZigBeeClusterDatabase.cpp

SDK_SRCS=\
  common/AdapterInterface.cpp \
  common/AdapterDevice.cpp \
  common/AdapterNamedValue.cpp \
  common/AdapterMethod.cpp \
  common/AdapterProperty.cpp \
  common/AdapterSignal.cpp \
  common/Guid.cpp \
  common/Value.cpp

TEST_SRCS = Tests/VariantTest.cpp
TEST_OBJS = $(patsubst %.cpp, %.o, $(TEST_SRCS))
TESTS = $(patsubst %.cpp, %, $(TEST_SRCS))
EXE_NAME=alljoyndsb
LIBXML_INC?=/usr/include/libxml2

ALLJOYN_INSTALL_DIR?=/Users/jgladi200/Work/alljoyn/alljoyn-15.09.00a-src/build/darwin/x86/debug/dist/cpp

CXXFLAGS          = -Wall -Wextra -std=c++0x -I. -I$(ALLJOYN_INSTALL_DIR)/inc -I$(LIBXML_INC) -Wno-unused-parameter
LDFLAGS           = -L $(ALLJOYN_INSTALL_DIR)/lib -lalljoyn -lcrypto -lxml2
DEV_PROVIDER_OBJS = $(patsubst %.cpp, %.o, $(SRCS))
OBJS              = $(DEV_PROVIDER_OBJS)
SDK_OBJS          = $(patsubst %.cpp, %.o, $(SDK_SRCS))
ADAPTER_OBJS      = $(patsubst %.cpp, %.o, $(ADAPTER_SRCS))
DEPS              = $(OBJS:%.o=%.d) $(TEST_OBJS:%.o=%.d)
DEPS             += $(SDK_OBJS:%.o=%.d) $(TEST_OBJS:%.o=%.d)
DEPS             += $(ADAPTER_OBJS:%.o=%.d) $(TEST_OBJS:%.o=%.d)

GTEST_DIR?=/usr/src/gtest
ifneq ("$(wildcard $(GTEST_DIR)/libgtest.a)","")
	GTEST_FLAGS = $(GTEST_DIR)/libgtest.a
	CXXFLAGS += -I$(GTEST_DIR)/include
else
	GTEST_FLAGS = -lgtest
endif

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIB_UUID=-luuid
    LDFLAGS += -pthread -luuid
    CXXFLAGS += -DQCC_OS_GROUP_POSIX
else
    CXXFLAGS += -DQCC_OS_GROUP_POSIX
endif

ifeq ($(DEBUG), 1)
  CXXFLAGS += -g -O0 -fno-inline
else
  CXXFLAGS += -O2
endif

ifeq ($V, 1)
  CXX_PRETTY = $(CXX)
  LD_PRETTY = $(CXX)
else
  CXX_PRETTY = @echo " [CXX] $<" ; $(CXX)
  LD_PRETTY = @echo "[LINK] $@" ; $(CXX)
endif

all: $(EXE_NAME)

tests: $(TESTS)

check: tests
	@for test in $(TESTS) ; do \
		$$test ; \
	done

clean:
	$(RM) $(EXE_NAME) $(ADAPTER_OBJS) $(TESTS) $(OBJS) $(DEPS) $(TEST_OBJS) $(SDK_OBJS) liballjoyndsb.so

$(EXE_NAME): mocadapter sdk $(OBJS)
	$(LD_PRETTY) -o $@ $(OBJS) -ldl $(LDFLAGS) -L. -lalljoyndsb

$(SDK_OBJS): CXXFLAGS := -fPIC $(CXXFLAGS)
sdk: $(SDK_OBJS)
	$(LD_PRETTY) -shared  $(SDK_OBJS) $(LIB_UUID) -o liballjoyndsb.so

$(ADAPTER_OBJS): CXXFLAGS := -fPIC $(CXXFLAGS)
mocadapter: $(ADAPTER_OBJS) sdk
	$(LD_PRETTY) -shared  $(ADAPTER_OBJS) -o libmockadapter.so -L. -lalljoyndsb

Tests/VariantTest: Tests/VariantTest.o common/Variant.o
	$(LD_PRETTY) -o $@ $^ $(LDFLAGS) $(GTEST_FLAGS)

%.o: %.cpp
	$(CXX_PRETTY) $(CXXFLAGS) -MMD -c -o $@ $<

-include $(DEPS)
