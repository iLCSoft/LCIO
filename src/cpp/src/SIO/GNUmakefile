######################################################
#
# GNUmakefile to build the SIO prototype library for LCIO
#
# @author Frank Gaede
# @date   Feb 11,2003
######################################################

#SHELL = /bin/sh

SIOINSTALL = ../../../../sio

CXX = g++ 
AR = ar

INCLUDE = ../../include


#CPPFLAGS = -g -I $(INCLUDE) -I ../sio/include
CPPFLAGS = -g -I $(INCLUDE)  -I $(SIOINSTALL)/include

#LIBS = -L $(INSTALL) -L ../sio/lib -lsioprototype -lsio -lz



objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

#lib : $(INSTALL)/libsioprototype.a

#$(INSTALL)/libsioprototype.a: $(objects)
#	$(AR) cr $(INSTALL)/libsioprototype.a  $?



lib: $(objects)

clean:
	rm -f *.o



#bin: ../bin/simjob ../bin/recjob ../bin/anajob

#../bin/simjob: simjob.C lib
#	$(CXX) -o ../bin/simjob $(CPPFLAGS) simjob.C  $(LIBS) 

#../bin/recjob: recjob.C lib
#	$(CXX) -o ../bin/recjob $(CPPFLAGS) recjob.C  $(LIBS) 

#../bin/anajob: anajob.C lib
#	$(CXX) -o ../bin/anajob $(CPPFLAGS) anajob.C  $(LIBS) 

