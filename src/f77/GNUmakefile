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
COMPILER_VERSION := $(shell gcc -v 2>&1 | grep version | cut -d' ' -f3 | cut -c1 )
COMPILER_MINOR_VERSION := $(shell gcc -v 2>&1 | grep version | cut -d' ' -f3 | cut -c3 )
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
LIBS = -L $(INSTALL) -L $(SIOINSTALL)/lib -llcio -lsio -lz -lg2c $(FTN_START_LIB)


bin: $(BINS)


clean:
	rm -f $(BINS) *.o


$(BINDIR)/anajob_F: anajob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/anajob_F anajob.o  $(LIBS)

$(BINDIR)/pyanajob_F: pyanajob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/pyanajob_F pyanajob.o  $(LIBS)

$(BINDIR)/pysimjob_F: pysimjob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/pysimjob_F pysimjob.o  $(LIBS)

$(BINDIR)/test_F: test.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/test_F test.o  $(LIBS)

$(BINDIR)/simjob_F: simjob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/simjob_F simjob.o  $(LIBS)


