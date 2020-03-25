CXX ?= g++
CXXFLAGS += -std=c++11 -g -O3 -fPIC -DUSE_TCL_STUBS -I../RvFun

LIB := rvfuntcl.so

all: $(LIB)

$(LIB): tclmain.cpp ../RvFun/rvfun.a
	@g++ -shared $(CXXFLAGS) -o $@ $^ -ltclstub8.6

