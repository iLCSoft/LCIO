######################################################
#
# GNUmakefile to build a fortran executable against LCIO
#
# @author Frank Gaede
# @date   Feb 11,2003
# @mod    Sep  4,2003  (Harald Vogt)
######################################################

# name of the program  - modify as needed
target_name = lciohbook

# path to cern lib - modify as needed
HBKLIBS = -L /cern/pro/lib -lpacklib



LCIOBASE = $(LCIO)
SIOINSTALL = $(LCIOBASE)/sio

INCLUDEDIR = $(LCIOBASE)/src/f77

# check the gcc compiler version if libfrtbegin.a has to be included
COMPILER_VERSION := $(shell gcc -v 2>&1 | grep version | cut -d' ' -f3 | cut -c1 )
COMPILER_MINOR_VERSION := $(shell gcc -v 2>&1 | grep version | cut -d' ' -f3 | cut -c3 )
ifeq ($(COMPILER_VERSION),3)
 ifneq ($(COMPILER_MINOR_VERSION),0)
  FTN_START_LIB = -lfrtbegin
 endif
endif

CXX = g++ 
FC = g77 -g

CPPFLAGS = -I$(INCLUDEDIR) 

BINDIR = $(PWD)
BINS := $(patsubst %.F,$(BINDIR)/%_F,$(wildcard *.F))

INSTALL = $(LCIOBASE)/lib
LIBS = $(HBKLIBS) -L $(INSTALL) -L $(SIOINSTALL)/lib -llcio -lsio -lz \
	-lg2c $(FTN_START_LIB)




bin: $(BINS)


clean:
	rm -f $(BINS) *.o


$(BINDIR)/$(target_name)_F: $(target_name).o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/$(target_name)_F $(target_name).o  $(LIBS)


