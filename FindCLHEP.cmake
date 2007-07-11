#####################################
# cmake module for finding CLHEP
# @author Jan Engels, DESY
#####################################

SET( CLHEP_FOUND FALSE )
MARK_AS_ADVANCED( CLHEP_FOUND )

# include dirs
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


# libraries
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
    SET( CLHEP_FOUND TRUE )
    SET( CLHEP_LIBRARIES ${CLHEP_LIB} )
    SET( CLHEP_DEFINITIONS "-DUSE_CLHEP" )
    MARK_AS_ADVANCED( CLHEP_LIBRARIES CLHEP_DEFINITIONS )
    MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME} -- works" )
ELSE()
    IF( CLHEP_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Check for CLHEP: ${CLHEP_HOME} -- failed!!" )
    ENDIF()
    MESSAGE( STATUS "Check for CLHEP: ${CLHEP_HOME}"
            " -- failed!! will skip this package..." )
ENDIF()
