# LCIO

![linux](https://github.com/iLCSoft/LCIO/workflows/linux/badge.svg)
![mac](https://github.com/iLCSoft/LCIO/workflows/macOS/badge.svg)
![python](https://github.com/iLCSoft/LCIO/workflows/python/badge.svg)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/11178/badge.svg)](https://scan.coverity.com/projects/ilcsoft-lcio)

LCIO ( **L**inear **C**ollider **I/O** ) is a persistency framework and event data model for linear collider detector studies. 

-------

## Introduction

LCIO is intended to be used in both simulation studies and analysis frameworks. Its light weight and portability makes it also suitable for use in detector R&D
testbeam applications. It provides a C++ and optionally a Java implementation with a common interface (API) - a Fortran interface to the C++ implementation also exists,
if built with ROOT, Python bindings are available .

Using a common persistency format and event data model allows to easily share results and compare reconstruction algorithms.
LCIO is used by almost all groups involved in linear collider detector studies and thus has become a de facto standard.

In particular it is used in the [iLCSoft](https://github.com/iLCSoft) framework.

-------


## Documentation

The most current code documentation for LCIO can be found at: 
- [https://ilcsoft.desy.de/LCIO/current/doc/doxygen_api/html/index.html](https://ilcsoft.desy.de/LCIO/current/doc/doxygen_api/html/index.html)

as an entry point to quickly find the API documentation for the EDM objects use:

- [https://ilcsoft.desy.de/LCIO/current/doc/doxygen_api/html/classEVENT_1_1LCObject.html](https://ilcsoft.desy.de/LCIO/current/doc/doxygen_api/html/classEVENT_1_1LCObject.html)

As a quick introduction you can read this LC-note on LCIO (CHEP proceedings 2003):

- [http://www-flc.desy.de/lcnotes/notes/LC-TOOL-2003-053.pdf](http://www-flc.desy.de/lcnotes/notes/LC-TOOL-2003-053.pdf)

A somewhat outdated manual can be found in 

- [./doc/manual.pdf](./doc/manual.pdf) 

it still provides a good overview over the philosophy of LCIO and how to use it.


-------


## Installation

- download the source code:

```sh
git clone https://github.com/iLCSoft/LCIO.git
cd LCIO
git checkout v02-15-01    ##  use a specific version
```

- if you just want to build a plain version of LCIO:

```sh
mkdir build
cd build
cmake ..
make -j 4 install
```

- to get a ROOT dictionary for LCIO that allows to read LCIO files in ROOT macros and provides the Python bindings you need a compatible
  version of ROOT installed and initialized (*make sure you use a compatible compiler and  C++ standard*), e.g.

```sh
. /cvmfs/ilc.desy.de/sw/x86_64_gcc82_sl6/root/6.18.04/bin/thisroot.sh
cmake -DBUILD_ROOTDICT=ON -D CMAKE_CXX_STANDARD=17 ..
make -j 4 install
cd ..
```

and then initialize your LCIO installation:

```sh
. ./setup.sh  ## <--- run this in the source directory
```


- you can test your installation with 

```sh
make test
```

- this should return with sth. like `100% tests passed, 0 tests failed out of 53`

-------


## Examples 

You can find a number of simple examples and tools in [./src/cpp/src/EXAMPLE/](./src/cpp/src/EXAMPLE/)

- for example run

```
. ./setup.sh  ## <--- run this in the source directory

simjob                  # a dummy simulation job
recjob   simjob.slcio   # a dummy reconstruction job     
```

- inspect files with

```sh
anajob recjob.slcio                       # print collections and sizes 
dumpevent recjob.slcio  1 | less          # detailed print of the 1st event

```


### delphes2lcio

For a more advanced example on how to create LCIO files with Delphes and analyze them with ROOT macros see

- [./examples/cpp/delphes2lcio/README.md](./examples/cpp/delphes2lcio/README.md)

-------


## Release Notes

You can find the release notes for LCIO in [./doc/ReleaseNotes.md](./doc/ReleaseNotes.md)

-------

## Reference

If you use LCIO for your physics analysis, please cite as:

```
@article{Gaede:2003ip,
    author = "Gaede, Frank and Behnke, Ties and Graf, Norman and Johnson, Tony",
    title = "{LCIO: A Persistency framework for linear collider simulation studies}",
    eprint = "physics/0306114",
    archivePrefix = "arXiv",
    reportNumber = "SLAC-PUB-9992, CHEP-2003-TUKT001",
    journal = "eConf",
    volume = "C0303241",
    pages = "TUKT001",
    year = "2003"
}

```

-------

## Copyright and Licence

Copyright (c) 2003-2016, DESY, Deutsches Elektronen Synchrotron

Copyright (c) 2003-2016, SLAC, National Accelerator Laboratory

LCIO is distributed under the [BSD-3-Clause licence](http://opensource.org/licenses/BSD-3-Clause).



