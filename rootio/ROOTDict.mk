
ALLH=$(shell find include/ -name '*.h')
ALLH+=lcio_templates.h
VETOH+=$(wildcard include/SIO/*.h include/CPPFORT/*.h)
#VETOH+=./include/UTIL/CellIDDecoder.h ./include/UTIL/PIDHandler.h

#-fg: just need the classes of the EDM:
#ALLH=$(wildcard include/EVENT/*.h include/IMPL/*.h)
#ALLH=./include/IMPL/ReconstructedParticleImpl.h ./include/EVENT/ReconstructedParticle.h ./include/EVENT/LCObject.h ./include/EVENT/LCEvent.h ./include/IMPL/LCEventImpl.h ./include/UTIL/LCTypedVector.h
#ALLH=./include/IMPL/ReconstructedParticleImpl.h ./include/EVENT/ReconstructedParticle.h ./include/EVENT/LCObject.h
 

ALLH:=$(filter-out $(VETOH),$(ALLH))
ALLHNAMES=$(addprefix dict/,$(subst /,_,$(basename $(ALLH))))
ALLRCDS=$(addsuffix _dict.cxx,$(ALLHNAMES))
ALLRCDO=$(ALLRCDS:.cxx=.o)

DICTLIB:=liblcioDict.so

ifeq ($(VERBOSE)$(V),)
CMDECHO=@
endif

.PHONY: all clean

all: $(DICTLIB)
$(DICTLIB): $(ALLRCDO)
	g++ -shared $^ -o$@

define ROOTCINT_template
  dict/$$(subst /,_,$$(basename $(1)))_Linkdef.h:
	$(CMDECHO)mkdir -p dict
	$(CMDECHO)echo '#pragma link C++ defined_in "'$1'";' > $$@
  dict/$$(subst /,_,$$(basename $(1)))_dict.cxx: $(1) dict/$$(subst /,_,$$(basename $(1)))_Linkdef.h
	$(CMDECHO)rootcint -f $$@ -c -DHANDLE_LCIO_EXCEPTIONS=IGNORED_FOR_CINT -Iinclude $(1) dict/$$(subst /,_,$$(basename $(1)))_Linkdef.h
endef

$(foreach hdr,$(ALLH),$(eval $(call ROOTCINT_template,$(hdr))))

%_dict.o: %_dict.cxx
	$(CMDECHO)g++ -fPIC -c $^ -Iinclude -I. `root-config --cflags` -o$@

clean:
	$(CMDECHO)rm -f dict/* $(DICTLIB)
