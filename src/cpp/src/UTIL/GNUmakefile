######################################################
#
# GNUmakefile to build the LCIO library
#
# @author Frank Gaede
# @date   Mar 10,2003
######################################################


CXX = g++ 
AR = ar


INSTALL = $(LCIOBASE)/lib
INCLUDE =  $(LCIOINCLUDE)

CPPFLAGS = $(LCIOCPPFLAGS) $(INCLUDE)

ifdef CLHEP
 CPPFLAGS += -DCLHEP -I$(CLHEP)/include
endif

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

$(INSTALL)/liblcio.a: $(objects)
	$(AR) cr $(INSTALL)/liblcio.a  $?

lib: $(objects)

clean:
	rm -f *.o




