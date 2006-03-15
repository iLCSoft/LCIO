# $Id: Makefile,v 1.4 2006-03-15 00:33:08 jeremy Exp $

#
# Makefile to build LCIO Python wrappers to the C++ library, using Swig.
#
# --Jeremy McCormick <jeremym@slac.stanford.edu>
#

# If LCIO is not set, then build env is not sane.
ifndef LCIO
  $(error define LCIO=/path/to/lcio)
endif

# What is Swig called?
ifndef SWIG
  SWIG = swig
endif

# Where is Swig's base dir?
ifndef SWIG_BASE
  SWIG_BASE = /usr/local/swig
endif

# Where are Swig's python .i files?
ifndef SWIG_LIB_DIR
  SWIG_LIB_DIR = $(SWIG_BASE)/Lib/python/
endif

# What is python called?
ifndef PYTHON
  PYTHON = python
endif

# Where is the python include dir?
ifndef PYTHON_INCLUDE_DIR
  PYTHON_INCLUDE_DIR = /usr/local/include/python2.4/
endif

# Where is Python installed?
ifndef PYTHONHOME
  PYTHONHOME = /usr/local
endif

# What is the Python version?
ifndef PYTHONVER
  PYTHONVER = 2.4
endif

# What is the Python install dir?
ifndef PYTHON_INSTALL_DIR
  PYTHON_INSTALL_DIR = $(PYTHONHOME)/lib/python$(PYTHONVER)/
endif

# End of vars we may take from environment.

# base name for output lib and wrapper file
module_name = lcio

# base include dir
lcio_include_dir = $(LCIO)/src/cpp/include

# sio
sio_dir = $(LCIO)/sio
sio_include_dir = $(sio_dir)/include

# full paths to all lcio include dirs, including base area
lcio_include_dirs = $(lcio_include_dir)
lcio_include_dirs += $(shell for i in $(lcio_include_subdirs); do echo $(lcio_include_dir)/$$i; done)

# swig options
#swig_opts = -c++ -python -makedefault -shadow
#swig_opts = -c++ -python -shadow -nodefaultdtor
swig_opts = -c++ -python -nodefaultctor -nodefaultdtor

# swig include flags
swig_include_flags = -I$(lcio_include_dir)
swig_include_flags += -I$(sio_include_dir)
swig_include_flags += -I$(SWIG_LIB_DIR)

# cpp includes
cpp_include_flags = -I$(PYTHON_INCLUDE_DIR) -I$(lcio_include_dir) -I$(sio_include_dir) -I.

# ld flags
ldflags = -L$(LCIO)/lib -llcio -L$(sio_dir)/lib -lsio -lz

# wrap file
wrap_file = $(module_name)_wrap

.DEFAULT: all
.PHONY: all

all: swig install

# build target
swig:
	$(SWIG) $(swig_opts) -o $(wrap_file).cc $(swig_include_flags) lcio_swig.i; \
	gcc -c $(wrap_file).cc $(cpp_include_flags) -o $(wrap_file).o; \
	g++ -shared $(wrap_file).o -L$(LCIO)/lib $(ldflags) -o _$(module_name).so;:

# run test file
test:
	$(PYTHON) test.py;:

# install to PYTHONHOME
install:
	python -c "import lcio" && cp _lcio.so lcio.pyc lcio.py $(PYTHON_INSTALL_DIR)

# clean up
clean:
	@rm -rf *.o *.so *.pyc;:
