######################################################
#
# GNUmakefile to build the LCIO library examples
#
# @author Frank Gaede
# @date   Feb 11,2003
######################################################
SIOINSTALL = $(LCIOBASE)/sio

# check for private compiler definition
ifdef MY_CXX
  CXX = $(MY_CXX)
else
  CXX = g++
endif

AR = ar


INSTALL = $(LCIOBASE)/lib
BINDIR = $(LCIOBASE)/bin
INCLUDE =  $(LCIOINCLUDE) -I$(SIOINSTALL)/include
CPPFLAGS = $(LCIOCPPFLAGS) $(INCLUDE) 

LIBS = -L$(INSTALL) -L$(SIOINSTALL)/lib -llcio -lsio -lz

#BINS := $(patsubst %.cc,$(BINDIR)/%,$(wildcard *.cc))

ifdef LCIO_TEST
 TESTJOBS :=
else
 TESTJOBS := testcopy.cc
endif

ifndef BUILD_LCIO_SHELL 
 TESTJOBS += lsh.cc
endif

#BINS = $(filter-out $(TESTJOBS),  $(patsubst %.cc,$(BINDIR)/%,$(wildcard *.cc))  ) 

BINS =  $(patsubst %.cc,$(BINDIR)/%, $(filter-out $(TESTJOBS), $(wildcard *.cc))) 


bin: $(BINS)


clean:
	rm -f $(BINS)


#$(BINDIR)/minijob: minijob.cc $(INSTALL)/liblcio.a
#	$(CXX) -o $(BINDIR)/minijob $(CPPFLAGS) minijob.cc  $(LIBS) 

$(BINDIR)/dumpevent: dumpevent.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/dumpevent $(CPPFLAGS) dumpevent.cc  $(LIBS) 

$(BINDIR)/copyfix: copyfix.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/copyfix $(CPPFLAGS) copyfix.cc  $(LIBS) 

$(BINDIR)/simjob: simjob.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/simjob $(CPPFLAGS) simjob.cc  $(LIBS) 

$(BINDIR)/recjob: recjob.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/recjob $(CPPFLAGS) recjob.cc  $(LIBS) 

$(BINDIR)/anajob: anajob.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/anajob $(CPPFLAGS) anajob.cc  $(LIBS) 

$(BINDIR)/readcalibration: readcalibration.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/readcalibration $(CPPFLAGS) readcalibration.cc  $(LIBS) 

# ----- building lsh has to be explicitly activated:
ifndef BUILD_LCIO_SHELL 
$(BINDIR)/lsh: lsh.cc $(INSTALL)/liblcio.a
	    $(CXX) -o $(BINDIR)/lsh $(CPPFLAGS) lsh.cc  $(LIBS) -lreadline -lncurses
endif

$(BINDIR)/stdhepjob: stdhepjob.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/stdhepjob $(CPPFLAGS) stdhepjob.cc  $(LIBS) 

$(BINDIR)/lcrtrelation: lcrtrelation.cc $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/lcrtrelation $(CPPFLAGS) lcrtrelation.cc  $(LIBS) 
