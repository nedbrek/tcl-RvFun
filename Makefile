CXX ?= g++
TCLDIR := $(shell echo "puts [::tcl::pkgconfig get includedir,install]" | tclsh)
CXXFLAGS += -std=c++11 -g -O3 -fPIC -DUSE_TCL_STUBS -I../RvFun -I$(TCLDIR)

LIB := rvfuntcl.so

all: $(LIB)

$(LIB): tclmain.cpp ../RvFun/rvfun.a
	@g++ -shared $(CXXFLAGS) -o $@ $^ -ltclstub8.6

.PHONY: clean
clean:
	rm -f $(LIB)

