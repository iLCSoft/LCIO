#############################################################################
# cmake build setup for LCIO
#
# For building lcio with cmake type:
# (1) $ mkdir build
# (2) $ cd build
# (3) $ cmake -C ../BuildSetup.cmake ..
# (4) $ make install
#
# @author Jan Engels, DESY
#############################################################################


#############################################################################
# Setup path variables
#############################################################################

# ILC_HOME
SET( ILC_HOME "/afs/desy.de/group/it/ilcsoft/v01-01_beta"
    CACHE PATH "Path to ILC Software" FORCE )

# CMake Modules Path
SET( CMAKE_MODULE_PATH "${ILC_HOME}/CMakeModules/v01-00"
    CACHE PATH "Path to CMake Modules" FORCE )

#############################################################################
# Java
#############################################################################

# If you don't want to use the java version automatically detected by cmake
# you can define here what java to use

# Path to Java
#SET( JAVA_HOME "${ILC_HOME}/java/1.5.0"
#    CACHE PATH "Path to Java" FORCE )

# If you don't need lcio.jar you can set this to OFF
#SET( INSTALL_JAR OFF CACHE BOOL "Set to OFF to skip build/install lcio.jar" FORCE )

#############################################################################
# CLHEP
#############################################################################

# Enable this for building LCIO with CLHEP
# For building LCIO with CLHEP you also need the CMakeModules (see above!)
#SET( BUILD_WITH "CLHEP" CACHE STRING "Build LCIO with these optional packages" FORCE )

# Path to CLHEP
SET( CLHEP_HOME "${ILC_HOME}/CLHEP/2.0.2.2"
    CACHE PATH "Path to CLHEP" FORCE )

#############################################################################
# Project options
#############################################################################

#SET( INSTALL_DOC OFF CACHE BOOL "Set to OFF to skip build/install Documentation" FORCE )
#SET( BUILD_LCIO_TESTJOBS ON CACHE BOOL "Set to ON to build LCIO testjobs" FORCE )
#SET( BUILD_F77_TESTJOBS ON CACHE BOOL "Set to ON to build LCIO F77 testjobs" FORCE )

# set cmake build type, default value is: RelWithDebInfo
# possible options are: None Debug Release RelWithDebInfo MinSizeRel
#SET( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE )

#############################################################################
# Advanced options
#############################################################################

#SET( BUILD_SHARED_LIBS OFF CACHE BOOL "Set to OFF to build static libraries" FORCE )

# installation path for LCIO
#SET( CMAKE_INSTALL_PREFIX "/foo/bar" CACHE STRING "Where to install LCIO" FORCE )
