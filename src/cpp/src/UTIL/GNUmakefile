######################################################
#
# GNUmakefile to build the LCIO library
#
# @author Frank Gaede
# @date   Mar 10,2003
######################################################


# check for private compiler definition
ifdef MY_CXX
  CXX = $(MY_CXX)
else
  CXX = g++
endif

AR = ar


INSTALL = $(LCIOBASE)/lib
INCLUDE =  $(LCIOINCLUDE)

CPPFLAGS = $(LCIOCPPFLAGS) $(INCLUDE)

ifdef CLHEP
 CPPFLAGS += -DUSE_CLHEP -I$(CLHEP)/include
endif

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

$(INSTALL)/liblcio.a: $(objects)
	$(AR) cr $(INSTALL)/liblcio.a  $?

lib: $(objects)

clean:
	rm -f *.o




