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
COMPILER_VERSION := $(shell gcc -v 2>&1 | grep "gcc version" | cut -d' ' -f3 | cut -c1 )
COMPILER_MINOR_VERSION := $(shell gcc -v 2>&1 | grep "gcc version" | cut -d' ' -f3 | cut -c3 )
ifeq ($(COMPILER_VERSION),3)
 ifneq ($(COMPILER_MINOR_VERSION),0)
  FTN_START_LIB = -lfrtbegin
 endif
endif

CXX = g++ 
FC = g77 -g

BINDIR = $(LCIOBASE)/bin
BINS := $(patsubst %.F,$(BINDIR)/%_F,$(wildcard *.F))

INSTALL = $(LCIOBASE)/lib
LIBS = -L $(INSTALL) -L $(SIOINSTALL)/lib -llcio -lsio -lz -lg2c $(FTN_START_LIB) -lg2c


bin: $(BINS)


clean:
	rm -f $(BINS) *.o


$(BINDIR)/simjob_F: simjob.o $(INSTALL)/liblcio.a
	@echo COMPILER_VERSION and COMPILER_MINOR_VERSION are $(COMPILER_VERSION) $(COMPILER_MINOR_VERSION)
	$(CXX) -o $(BINDIR)/simjob_F simjob.o  $(LIBS)

$(BINDIR)/anajob_F: anajob.o $(INSTALL)/liblcio.a
	@echo COMPILER_VERSION and COMPILER_MINOR_VERSION are $(COMPILER_VERSION) $(COMPILER_MINOR_VERSION)
	$(CXX) -o $(BINDIR)/anajob_F anajob.o  $(LIBS)




