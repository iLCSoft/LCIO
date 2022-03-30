# LCIO

The is a forked version of LCIO which is *not up to date* with the main branch of LCIO. The reason
for the fork is that HPS uses LCIO for both C++ and Java, and main branch is out of sync since Java 
is no longer properly supported. For the code here, I made the LCIO version 2.7.5 compatible with MacOS
C++17. C++17 has removed a number of C++11 features.


[![Build Status](https://travis-ci.org/iLCSoft/LCIO.svg?branch=master)](https://travis-ci.org/iLCSoft/LCIO)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/11178/badge.svg)](https://scan.coverity.com/projects/ilcsoft-lcio)

LCIO ( **L**inear **C**ollider **I/O** ) provides the event data model (EDM) and 
persistency solution for Linear Collider detector R&D studies.

## Documentation

Please see [http://lcio.desy.de](http://lcio.desy.de) for more information and detailed 
documentation on LCIO.

## Copyright and Licence

Copyright (c) 2003-2016, DESY, Deutsches Elektronen Synchrotron

Copyright (c) 2003-2016, SLAC, National Accelerator Laboratory

LCIO is distributed under the [BSD-3-Clause licence](http://opensource.org/licenses/BSD-3-Clause).



