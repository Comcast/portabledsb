SRCS=AllJoynProvider.cpp \
     AllJoynService.cpp \
     dsb.cpp

ALLJOYN_INSTALL_DIR=/Users/jgladi200/Work/alljoyn/alljoyn-15.04.00-src/build/darwin/x86/debug/dist/cpp

CXXFLAGS=-Wall -Wextra -g -std=c++0x -I$(ALLJOYN_INSTALL_DIR)/inc
LDFLAGS=-L $(ALLJOYN_INSTALL_DIR)/lib -lalljoyn -lcrypto
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

all: dsb

clean:
	$(RM) dsb core *.o

dsb: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)


