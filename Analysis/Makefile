#Makefile for compiling n3He user end Analysis scripts.
#Update and Modified By: Latiful Kabir
#Date: 12/25/14
ROOTCINT  = rootcint
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)
CXXFLAGS  += -fPIC -L/home/kabir/GIT/n3HeAnalysisTool/libn3He/lib -ln3He
LIB_INCLUDE = /home/kabir/GIT/n3HeAnalysisTool/libn3He/src ####<===add

all: libAnalysis

clean: libAnalysis_clean
clear: libAnalysis_clear

# BEGIN libAnalysis

####Old Approaches
# libAnalysis: lib/libAnalysis.so
# libAnalysis_base= SpinAnalyzer CheckTree ### Put individual Analysis script name with out extension here. 
# libAnalysis_inc	= $(libAnalysis_base:=.h)
# libAnalysis_obj	= $(addprefix src/,$(libAnalysis_base:=.o))

libAnalysis: lib/libAnalysis.so
libAnalysis_base= $(wildcard src/*.cc)  
libAnalysis_inc	= $(patsubst src/%.cc, ./%.h,$(libAnalysis_base)) 
libAnalysis_obj	= $(patsubst src/%.cc, src/%.o,$(libAnalysis_base))

lib/libAnalysis.so: $(libAnalysis_obj)  src/libAnalysis_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -I$(LIB_INCLUDE) -shared -Wl,-soname,libAnalysis.so $^
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation successfully!!!"
	@echo "Before you try it you MUST do the following:"
	@echo "1. Put the following command into your ~/.bashrc file:"
	@echo "if [ -f /path/to/libAnalysis/bin/thisn3HeAnalysis.sh ]; then" 
	@echo ". /path/to/libAnalysis/bin/thisn3He.sh"
	@echo "fi"
	@echo "2. Now copy the rootlogon.C file in the directory 'macros' under ROOT installation directory."  
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cc
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -I$(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libAnalysis_Dict.cc:
	cd src/; $(ROOTCINT) -f libAnalysis_Dict.cc -c $(libAnalysis_inc) LinkDef.h

libAnalysis_clean:
	rm -f $(libAnalysis_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libAnalysis.so

libAnalysis_clear:
	rm -f $(libAnalysis_obj)
	rm -f  src/*_Dict.*


