######################################################
#
# GNUmakefile to build a fortran executable against LCIO
#
# @author Frank Gaede
# @date   Feb 11,2003
# @mod    Sep  4,2003  (Harald Vogt)
######################################################

#LCIOBASE = /afs/desy.de/user/g/gaede/lcio/devel/lcio-v00-01/
LCIOBASE = $(LCIO)
SIOINSTALL = $(LCIOBASE)/sio

# check the gcc compiler version if libfrtbegin.a has to be included
COMPILER_VERSION := $(shell gcc --version 2>&1 | grep "gcc" | cut -d' ' -f3 | cut -c1 )
COMPILER_MINOR_VERSION := $(shell gcc --version 2>&1 | grep "gcc" | cut -d' ' -f3 | cut -c3 )

FC = g77
ifeq ($(COMPILER_VERSION),3)
  ifneq ($(COMPILER_MINOR_VERSION),0)
    FTN_START_LIB = -lfrtbegin
  endif
  LIBS = -L $(INSTALL) -L $(SIOINSTALL)/lib  -llcio -lsio -lz -lg2c $(FTN_START_LIB) -lg2c -llcio -lsio -lz
endif
ifeq ($(COMPILER_VERSION),4)
  FC = gfortran
  LIBS = -L $(INSTALL) -L $(SIOINSTALL)/lib -lgfortran -lgfortranbegin -lgfortran -llcio -lsio -lz
endif

# check for AMD64 Linux and set -fno-f2c to get correct results
# not needed anymore with the modified "cfortran.h"
# /OS_TYPE:= $(shell uname -m)
# /ifeq ($(OS_TYPE),x86_64)
# /  X86_64_OPTION = -fno-f2c
# /endif

# check for private compiler definitions
ifdef MY_CXX
  CXX = $(MY_CXX)
else
  CXX = g++
endif
ifdef MY_FC
  FC = $(MY_FC)
endif


BINDIR = $(LCIOBASE)/bin
BINS := $(patsubst %.F,$(BINDIR)/%_F,$(wildcard *.F))

INSTALL = $(LCIOBASE)/lib


bin: $(BINS)


clean:
	rm -f $(BINS) *.o


$(BINDIR)/simjob_F: simjob.o $(INSTALL)/liblcio.a
# 	@echo COMPILER_VERSION and COMPILER_MINOR_VERSION are $(COMPILER_VERSION) $(COMPILER_MINOR_VERSION)
	$(CXX) -o $(BINDIR)/simjob_F simjob.o  $(LIBS)

$(BINDIR)/recjob_F: recjob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/recjob_F recjob.o  $(LIBS)

$(BINDIR)/anajob_F: anajob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/anajob_F anajob.o  $(LIBS)

$(BINDIR)/simjob_chain_F: simjob_chain.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/simjob_chain_F simjob_chain.o  $(LIBS)




