######################################################
#
# GNUmakefile to build the LCIO library
#
# @author Frank Gaede
# @date   Mar 10,2003
# @update Sep 30,2003 H. Vogt
######################################################


LCIOBASE := $(LCIO)
export LCIOBASE 


CLHEP_INCLUDE := $(CLHEP)/include


LCIOINCLUDE := -I $(LCIOBASE)/src/cpp/include -I $(CLHEP_INCLUDE)
export LCIOINCLUDE

ifdef LCIODEBUG
 LCIOCPPFLAGS = -g -pg -Wall -pedantic -Wno-long-long -ansi 
# -E -dM
else
 LCIOCPPFLAGS = -O3 -Wall -pedantic -Wno-long-long -ansi 
endif


#-pedantic
export LCIOCPPFLAGS


.PHONY: lib clean IMPL SIO IOIMPL CPPFORT bin UTIL

all: lib bin


lib: IMPL SIO IOIMPL CPPFORT UTIL


bin: lib
	$(MAKE) -C ./src/EXAMPLE bin

IMPL:
	echo $(LCIOBASE) ; $(MAKE) -C ./src/IMPL

SIO:
	$(MAKE) -C ./src/SIO

UTIL:
	$(MAKE) -C ./src/UTIL


IOIMPL:
# on new fast pc's it is required here to wait a bit to avoid
# that the objects (here only one!) are not added to the library
	sleep 1 ; $(MAKE) -C ./src/IOIMPL

CPPFORT:
	$(MAKE) -C ./src/CPPFORT

clean:
	$(MAKE) -C ./src/IMPL clean; $(MAKE) -C ./src/SIO clean; \
	$(MAKE) -C ./src/IOIMPL clean ; $(MAKE) -C ./src/EXAMPLE clean;\
	$(MAKE) -C ./src/CPPFORT clean ; $(MAKE) -C ./src/UTIL clean ; \
	rm -rf $(LCIOBASE)/lib/liblcio.a






