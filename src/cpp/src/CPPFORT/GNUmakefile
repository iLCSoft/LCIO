######################################################
#
# GNUmakefile to build the LCIO library
#
# @author Frank Gaede
# @date   Mar 10,2003
# @modified Sep 17, 2003 (H. Vogt) 
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

# check the gcc compiler version which warning option is accepted
COMPILER_VERSION := $(shell gcc -v 2>&1 | grep version | cut -d' ' -f3 | cut -c1 )
COMPILER_MINOR_VERSION := $(shell gcc -v 2>&1 | grep "gcc version" | cut -d' ' -f3 | cut -c3 )
ifeq ($(COMPILER_VERSION),3) 
 ifneq ($(COMPILER_MINOR_VERSION),0)
  UNUSEDCPP = -Wno-unused-function
 else
  UNUSEDCPP = -Wno-unused
 endif
else
  UNUSEDCPP = -Wno-unused
endif

# suppress warnings for unused functions
CPPFLAGS = -Df2cFortran $(LCIOCPPFLAGS) $(UNUSEDCPP) $(INCLUDE)

objects := $(patsubst %.cc,%.o,$(wildcard *.cc))

$(INSTALL)/liblcio.a: $(objects)
	$(AR) cr $(INSTALL)/liblcio.a  $?

lib: $(objects)

clean:
	rm -f *.o




