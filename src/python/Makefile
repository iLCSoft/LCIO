# $Id: Makefile,v 1.6 2006-03-22 01:14:29 jeremy Exp $

#
# Makefile to build LCIO Python wrappers using Swig.
#
# --Jeremy McCormick <jeremym@slac.stanford.edu>
#

# LCIO top dir
ifndef LCIO
  LCIO = ../..
endif

# What is Swig called?
ifndef SWIG
  SWIG = swig
endif

# What is python called?
ifndef PYTHON
  PYTHON = python
endif

# Where is Python installed?
ifndef PYTHONHOME
  # Make a guess at PYTHONHOME based on binary location.
  PYTHONHOME = $(shell which python | xargs dirname)/..
endif

# What is the Python version?
ifndef PYTHONVER
  # Make a guess at PYTHONVER using version output.
  PYTHONVER = $(shell python -V 2>&1 | grep -o "[0-9]\.[0-9]")
endif

# Where is the python include dir?
ifndef PYTHON_INCLUDE_DIR
  PYTHON_INCLUDE_DIR = $(PYTHONHOME)/include/python$(PYTHONVER)/
endif

# What is the Python install dir?
ifndef PYTHON_INSTALL_DIR
  PYTHON_INSTALL_DIR = $(PYTHONHOME)/lib/python$(PYTHONVER)/
endif

# End of vars we may take from environment.

# Base name for output lib
module_name = lcio
module_lib_name = _$(module_name).so

# LCIO include dir
lcio_include_dir =

# SIO dir
SIO = $(LCIO)/sio

# lcio include flags
lcio_include_flags = -I$(LCIO)/src/cpp/include -I$(SIO)/include

# combined cpp include flags
cpp_include_flags = -I. -I$(PYTHON_INCLUDE_DIR) $(lcio_include_flags)

# loader flags
ldflags = -L$(LCIO)/lib -llcio -L$(SIO)/lib -lsio -lz

# wrapper file naming
wrap_file = $(module_name)_wrap
wrap_cc = $(wrap_file).cc
wrap_obj = $(wrap_file).o

# swig options
swig_opts = -c++ -python -nodefaultctor -nodefaultdtor

# swig args, flags
swig_args = -o $(wrap_cc) $(lcio_include_flags) lcio_swig.i

# Default target builds all
.DEFAULT: all
.PHONY: all

# Default target executes build and install
all: swig 

# Build the wrapper lib
swig:
	$(SWIG) $(swig_opts) $(swig_args) ; \
	gcc -c $(wrap_cc) $(cpp_include_flags) -o $(wrap_obj); \
	g++ -shared $(wrap_file).o $(ldflags) -o $(module_lib_name);:

# Run a test file
test:
	$(PYTHON) test.py;:

# Install the wrapper to PYTHONHOME
install:
	python -c "import lcio" && cp _lcio.so lcio.pyc lcio.py $(PYTHON_INSTALL_DIR)

# Clean up this directory (does not uninstall from PYTHONHOME)
clean:
	@rm -rf *.o *.so *.pyc $(wrap_cc);:
