######################################################
#
# GNUmakefile to build a fortran executable against LCIO
#
# @author Frank Gaede
# @date   Feb 11,2003
######################################################
SIOINSTALL = $(LCIOBASE)/sio

#LCIOBASE = /afs/desy.de/user/g/gaede/lcio/devel/lcio-v00-01/
LCIOBASE = $(LCIO)

CXX = g++ 
F77 = g77

#g++  -o anajob anajob.o -L ../../lib -llcio -L ../../sio/lib/  -lsio -lz -lg2c 

INSTALL = $(LCIOBASE)/lib
BINDIR = $(LCIOBASE)/bin

LIBS = -L $(INSTALL) -L $(SIOINSTALL)/lib -llcio -lsio -lz -lg2c

BINS := $(patsubst %.f,$(BINDIR)/%_f,$(wildcard *.f))


bin: $(BINS)

clean:
	rm -f $(BINS) *.o


$(BINDIR)/anajob_f: anajob.o $(INSTALL)/liblcio.a
	$(CXX) -o $(BINDIR)/anajob_f anajob.o  $(LIBS) 

