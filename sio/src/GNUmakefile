######################################################
#
# GNUmakefile to build the SIO library for LCIO
#
# @author Frank Gaede
# @date   Feb 11,2003
######################################################

#SHELL = /bin/sh


CXX = g++ 
AR = ar

INCLUDE = ../include

#CPPFLAGS = -O4 -I $(INCLUDE) 
CPPFLAGS = -g -I $(INCLUDE) 


objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

lib : $(INSTALL)/libsio.a

$(INSTALL)/libsio.a: $(objects)
	$(AR) -cr $(INSTALL)/libsio.a  $?



clean:
	rm -f *.o $(INSTALL)/libsio.a



