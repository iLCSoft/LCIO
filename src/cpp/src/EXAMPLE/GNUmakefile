######################################################
#
# GNUmakefile to build the LCIO library examples
#
# @author Frank Gaede
# @date   Feb 11,2003
######################################################
SIOINSTALL = $(LCIOBASE)/sio

CXX = g++ 
AR = ar


INSTALL = $(LCIOBASE)/lib
BINDIR = $(LCIOBASE)/bin
INCLUDE =  $(LCIOINCLUDE) -I $(SIOINSTALL)/include
CPPFLAGS = $(LCIOCPPFLAGS) $(INCLUDE)

LIBS = -L $(INSTALL) -L $(SIOINSTALL)/lib -llcio -lsio -lz

BINS := $(patsubst %.cc,$(BINDIR)/%,$(wildcard *.cc))


bin: $(BINS)

clean:
	rm -f $(BINS)


$(BINDIR)/simjob: simjob.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/simjob $(CPPFLAGS) simjob.cc  $(LIBS) 

#../bin/recjob: recjob.C lib
#	$(CXX) -o ../bin/recjob $(CPPFLAGS) recjob.C  $(LIBS) 

#../bin/anajob: anajob.C lib
#	$(CXX) -o ../bin/anajob $(CPPFLAGS) anajob.C  $(LIBS) 



