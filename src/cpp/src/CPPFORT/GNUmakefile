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
CPPFLAGS = -Df2cFortran $(LCIOCPPFLAGS) $(INCLUDE) 

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

$(INSTALL)/liblcio.a: $(objects)
	$(AR) cr $(INSTALL)/liblcio.a  $?

lib: $(objects)

clean:
	rm -f *.o




