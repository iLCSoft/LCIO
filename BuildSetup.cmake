#############################################################################
# cmake build setup for LCIO
#
# For building lcio with cmake type:
# (1) $ mkdir build
# (2) $ cd build
# (3) $ cmake -C ../BuildSetup.cmake ..
# (4) $ make install
#
# NOTE for SL3/SL4 users:
# For using afs reference installation replace step (3) with:
# $ cmake -C /afs/desy.de/group/it/ilcsoft/vXX-XX/ILCSoft.cmake ..
#
# @author Jan Engels, DESY
#############################################################################


#############################################################################
# Project paths
#############################################################################

# ILC_HOME
SET( ILC_HOME "/afs/desy.de/group/it/ilcsoft/v01-01_beta"
    CACHE PATH "Path to ILC Software" FORCE )

# Path to CLHEP
SET( CLHEP_HOME "${ILC_HOME}/CLHEP/2.0.2.2"
    CACHE PATH "Path to CLHEP" FORCE )

# CMake Modules Path
SET( CMAKE_MODULE_PATH "${ILC_HOME}/CMakeModules/v01-00"
    CACHE PATH "Path to CMake Modules" FORCE )

#############################################################################
# Project options
#############################################################################

#SET( BUILD_WITH "CLHEP" CACHE STRING "Build LCIO with these optional packages" FORCE )

#SET( BUILD_SHARED_LIBS OFF CACHE BOOL "Set to OFF to build static libraries" FORCE )
#SET( INSTALL_DOC OFF CACHE BOOL "Set to OFF to skip build/install Documentation" FORCE )
#SET( BUILD_LCIO_TESTJOBS ON CACHE BOOL "Set to ON to build LCIO testjobs" FORCE )
#SET( BUILD_F77_TESTJOBS ON CACHE BOOL "Set to ON to build LCIO F77 testjobs" FORCE )

# set cmake build type, default value is: RelWithDebInfo
# possible options are: None Debug Release RelWithDebInfo MinSizeRel
#SET( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE )

#############################################################################
# Advanced options
#############################################################################

# installation path for LCIO
#SET( CMAKE_INSTALL_PREFIX "/foo/bar" CACHE STRING "Where to install LCIO" FORCE )
