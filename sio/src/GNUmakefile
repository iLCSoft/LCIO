######################################################
#
# GNUmakefile to build the SIO library for LCIO
#
# @author Frank Gaede
# @date   Feb 11,2003
######################################################

#SHELL = /bin/sh

# check for private compiler definition
ifdef MY_CXX
  CXX = $(MY_CXX)
else
  CXX = g++
endif

AR = ar

INCLUDE = ../include

ifdef LCIODEBUG
 CPPFLAGS = -g -pg -I $(INCLUDE)
else
 CPPFLAGS = -O3  -D _FILE_OFFSET_BITS=64 -I $(INCLUDE)
endif


objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

lib : $(INSTALL)/libsio.a

$(INSTALL)/libsio.a: $(objects)
	$(AR) -cr $(INSTALL)/libsio.a  $?



clean:
	rm -f *.o $(INSTALL)/libsio.a



