######################################################
#
# GNUmakefile to build the LCIO library
#
# @author Frank Gaede
# @date   Mar 10,2003
######################################################


LCIOBASE := $(shell pwd)/../..
export LCIOBASE 

LCIOINCLUDE := -I $(LCIOBASE)/src/cpp/include
export LCIOINCLUDE

LCIOCPPFLAGS = -g -D NO_DEBUG
export LCIOCPPFLAGS


.PHONY: lib clean IMPL SIO IOIMPL CPPFORT bin

all: lib bin


lib: IMPL SIO IOIMPL CPPFORT


bin: lib
	$(MAKE) -C ./src/EXAMPLE bin

IMPL:
	echo $(LCIOBASE) ; $(MAKE) -C ./src/IMPL
SIO:
	$(MAKE) -C ./src/SIO
IOIMPL:
	$(MAKE) -C ./src/IOIMPL

CPPFORT:
	$(MAKE) -C ./src/CPPFORT

clean:
	$(MAKE) -C ./src/IMPL clean; $(MAKE) -C ./src/SIO clean; \
	 $(MAKE) -C ./src/IOIMPL clean ; $(MAKE) -C ./src/EXAMPLE clean;\
	rm -rf $(LCIOBASE)/lib/liblcio.a






