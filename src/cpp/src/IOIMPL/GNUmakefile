######################################################
#
# GNUmakefile to build the LCIO library
#
# @author Frank Gaede
# @date   Mar 10,2003
# @modified Sep 30, 2003 (H. Vogt)
######################################################


CXX = g++ 
AR = ar


INSTALL = $(LCIOBASE)/lib
INCLUDE =  $(LCIOINCLUDE)
CPPFLAGS = $(LCIOCPPFLAGS) $(INCLUDE)

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

$(INSTALL)/liblcio.a: $(objects)
# on new fast pc's it is required here to wait a bit to avoid
# that the objects (here only one!) are not added to the library
	sleep 1
	$(AR) cr $(INSTALL)/liblcio.a  $?

lib: $(objects)

clean:
	rm -f *.o




