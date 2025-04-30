CXX      := g++
LD       := g++
CXXFLAGS := -Wall -Wextra -Werror -Wno-tautological-compare -O2 -std=c++1y -g -Iinterface/ $(shell root-config --cflags)
LDFLAGS  := -shared -Wall -Wextra $(shell root-config --libs)
SRC 	 := $(wildcard src/*.cc)
BINSRC 	 := $(wildcard test/*.cpp)
OBJS     := $(SRC:src/%.cc=obj/lib/%.o)
BINOBJS  := $(BINSRC:test/%.cpp=obj/bin/%.o)
BINS 	 := $(BINSRC:test/%.cpp=bin/%)
PROJNAME = HGCalBufferModel
LIB 	 := lib/lib$(PROJNAME).so

-include $(OBJS:.o=.d)
-include $(BINOBJS:.o=.d)

obj/lib/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
	$(CXX) $(CXXFLAGS) -MM -MP -MT $@ $< -o $(@:.o=.d)

obj/bin/%.o: test/%.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
	$(CXX) $(CXXFLAGS) -MM -MP -MT $@ $< -o $(@:.o=.d)

lib/$(PROJNAME)_xr.cc: src/classes.h src/classes_def.xml interface/*.h
	genreflex src/classes.h -s src/classes_def.xml -o lib/$(PROJNAME)_xr.cc --deep --fail_on_warnings \
	--rootmap=lib/$(PROJNAME)_xr.rootmap --rootmap-lib=$(LIB) --capabilities=lib/$(PROJNAME)_xi.cc \
	-DCMS_DICT_IMPL -D_REENTRANT -DGNUSOURCE -D__STRICT_ANSI__ -DGNU_GCC -D_GNU_SOURCE \
	-Iinterface/ -DCMSSW_REFLEX_DICT

lib/$(PROJNAME)_xr.cc.o: lib/$(PROJNAME)_xr.cc
	c++ -MMD -MF lib/$(PROJNAME)_xr.cc.d -c -Iinterface/ -I./ -DGNU_GCC -D_GNU_SOURCE $(shell root-config --cflags)  \
	-DCMSSW_REFLEX_DICT -pthread -Os -Wno-unused-variable -g -fPIC lib/$(PROJNAME)_xr.cc -o lib/$(PROJNAME)_xr.cc.o

$(LIB): $(OBJS) lib/$(PROJNAME)_xr.cc.o
	$(LD) $(LDFLAGS) -o $@ $^

bin/%: obj/bin/%.o $(LIB)
	$(CXX) -o $@ $^ $(shell root-config --libs)

all: $(LIB) $(BINS)

.SECONDARY: $(BINOBJS) obj/lib/$(PROJNAME)_xr.cc
.PHONY: all clean
.DEFAULT_GOAL := all

clean:
	rm obj/lib/* obj/bin/* lib/*
