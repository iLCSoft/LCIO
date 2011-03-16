###########################################
# cmake module for finding CLHEP
#
# returns:
#   CLHEP_FOUND
#   CLHEP_INCLUDE_DIRS
#   CLHEP_LIBRARIES
#
# @author Jan Engels, DESY
###########################################

SET( CLHEP_FOUND FALSE )
MARK_AS_ADVANCED( CLHEP_FOUND )
# is HepPDT a part of CLHEP? (CLHEP versions < 2.0.3.0/1.9.3.0)
SET( CLHEP_includes_HepPDT TRUE )
MARK_AS_ADVANCED( CLHEP_includes_HepPDT )

# HepPDT - include dir
#SET( HepPDT_INCLUDE_DIR HepPDT_INCLUDE_DIR-NOTFOUND )
#MARK_AS_ADVANCED( HepPDT_INCLUDE_DIR )
#
## first look if it is available under CLHEP_HOME
#FIND_PATH( HepPDT_INCLUDE_DIR
#    NAMES CLHEP/HepPDT/ParticleID.hh
#    PATHS ${CLHEP_HOME}
#    PATH_SUFFIXES include
#    NO_DEFAULT_PATH )
#
## if not, look in HepPDT_HOME
#IF( NOT HepPDT_INCLUDE_DIR )
#    # HepPDT was not found in CLHEP
#    SET( CLHEP_includes_HepPDT FALSE )
#
#    IF( DEFINED HepPDT_HOME OR DEFINED ENV{HEPPDT} )
#
#        # write HepPDT_HOME to cache
#        IF( NOT DEFINED HepPDT_HOME )
#            SET( HepPDT_HOME "$ENV{HEPPDT}" )
#        ENDIF()
#        SET( HepPDT_HOME "${HepPDT_HOME}" CACHE PATH "Path to HepPDT" FORCE )
#
#        # find include dir
#        FIND_PATH( HepPDT_INCLUDE_DIR
#            NAMES HepPDT/ParticleID.hh
#            PATHS ${HepPDT_HOME}
#            PATH_SUFFIXES include
#            NO_DEFAULT_PATH )
#    ELSE()
#        IF( NOT CLHEP_FIND_QUIETLY )
#            MESSAGE( STATUS "Check for HepPDT: HepPDT_HOME not set!!" )
#            MESSAGE( STATUS "Please define it with cmake -DHepPDT_HOME=<Path_to_HepPDT>" )
#        ENDIF()
#    ENDIF()
#ENDIF()

## HepPDT - libraries
#SET( HepPDT_FINDLIB_FAILED FALSE )
#MARK_AS_ADVANCED( HepPDT_FINDLIB_FAILED )
#
#IF( NOT CLHEP_includes_HepPDT )
#    SET( HepPDT_LIB_NAMES HepPDT HepPID )
#    MARK_AS_ADVANCED( HepPDT_LIB_NAMES )
#
#    FOREACH( libname ${HepPDT_LIB_NAMES} )
#
#        SET( HepPDT_LIB_${libname} HepPDT_LIB_${libname}-NOTFOUND )
#        MARK_AS_ADVANCED( HepPDT_LIB_${libname} )
#
#        FIND_LIBRARY( HepPDT_LIB_${libname}
#            NAMES ${libname}
#            PATHS ${HepPDT_HOME}
#            PATH_SUFFIXES lib
#            NO_DEFAULT_PATH )
#
#        IF( NOT HepPDT_LIB_${libname} )
#            SET( HepPDT_FINDLIB_FAILED TRUE )
#            IF( NOT CLHEP_FIND_QUIETLY )
#                MESSAGE( STATUS "Check for HepPDT -- failed to find ${libname} library!!" )
#            ENDIF()
#        ELSE()
#            LIST( APPEND HepPDT_LIBS ${HepPDT_LIB_${libname}} )
#        ENDIF()
#    ENDFOREACH()
#ENDIF()

# CLHEP include dir
SET( CLHEP_INCLUDE_DIR CLHEP_INCLUDE_DIR-NOTFOUND )
MARK_AS_ADVANCED( CLHEP_INCLUDE_DIR )

FIND_PATH( CLHEP_INCLUDE_DIR
    NAMES CLHEP/Vector/ThreeVector.h
    PATHS ${CLHEP_HOME}
    PATH_SUFFIXES include
    NO_DEFAULT_PATH )

IF( NOT CLHEP_INCLUDE_DIR AND NOT CLHEP_FIND_QUIETLY )
    MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME}"
            " -- failed to find CLHEP include directory!!" )
ENDIF()

# CLHEP library
SET( CLHEP_LIB CLHEP_LIB-NOTFOUND )
MARK_AS_ADVANCED( CLHEP_LIB )

FIND_LIBRARY( CLHEP_LIB
    NAMES CLHEP
    PATHS ${CLHEP_HOME}
    PATH_SUFFIXES lib
    NO_DEFAULT_PATH )

IF( NOT CLHEP_LIB AND NOT CLHEP_FIND_QUIETLY )
    MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME}"
            " -- failed to find CLHEP library!!" )
ENDIF()

# set variables and display results
IF( CLHEP_INCLUDE_DIR AND CLHEP_LIB )
    #AND HepPDT_INCLUDE_DIR AND NOT HepPDT_FINDLIB_FAILED )

    SET( CLHEP_FOUND TRUE )
    IF( CLHEP_includes_HepPDT )
        SET( CLHEP_LIBRARIES ${CLHEP_LIB} )
        SET( CLHEP_INCLUDE_DIRS ${CLHEP_INCLUDE_DIR} )
        SET( CLHEP_DEFINITIONS "-DUSE_CLHEP" )
    ELSE()
        SET( CLHEP_LIBRARIES ${CLHEP_LIB} ${HepPDT_LIBS} )
        SET( CLHEP_INCLUDE_DIRS ${CLHEP_INCLUDE_DIR} ${HepPDT_INCLUDE_DIR} )
        SET( CLHEP_DEFINITIONS "-DUSE_CLHEP -DUSE_SEPARATE_HEPPDT" )
    ENDIF()
    MARK_AS_ADVANCED( CLHEP_INCLUDE_DIRS CLHEP_LIBRARIES CLHEP_DEFINITIONS )
    MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME} -- works" )
ELSE()
    IF( CLHEP_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Check for CLHEP: ${CLHEP_HOME} -- failed!!" )
    ENDIF()
    IF( NOT CLHEP_FIND_QUIETLY )
        MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME}"
                " -- failed!! will skip this package..." )
    ENDIF()
ENDIF()
