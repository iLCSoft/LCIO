#####################################
# cmake module for finding ROOT
#
# returns:
#   ROOT_FOUND
#   ROOT_INCLUDE_DIRS
#   ROOT_LIBRARIES
#   ROOT_DEFINITIONS
#
# @author Jan Engels, DESY
#####################################

SET( ROOT_FOUND FALSE )
MARK_AS_ADVANCED( ROOT_FOUND )

# Threads library is needed for root
#FIND_PACKAGE( Threads REQUIRED)

# include dirs
SET( ROOT_INCLUDE_DIR ROOT_INCLUDE_DIR-NOTFOUND )
MARK_AS_ADVANCED( ROOT_INCLUDE_DIR )

# set env ROOTSYS for running root-config
SET( ENV{ROOTSYS} "${ROOT_HOME}" )

# also try to get include dir from root-config
EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
    ARGS --incdir
    OUTPUT_VARIABLE ROOT_INC_DIR
    RETURN_VALUE incdir_ret )
# if command fails clean up variable
IF( incdir_ret )
    SET( ROOT_INC_DIR )
ENDIF()

FIND_PATH( ROOT_INCLUDE_DIR
    NAMES TH1.h
    PATHS ${ROOT_HOME}/include ${ROOT_INC_DIR}
    NO_DEFAULT_PATH )

IF( NOT ROOT_INCLUDE_DIR AND NOT ROOT_FIND_QUIETLY )
    MESSAGE( STATUS "Check for ROOT: ${ROOT_HOME}"
            " -- failed to find ROOT include directory!!" )
ENDIF()

# libraries
SET( ROOT_LIB_NAMES )
SET( ROOT_FINDLIB_FAILED FALSE )
MARK_AS_ADVANCED( ROOT_LIB_NAMES ROOT_FINDLIB_FAILED )

# get lib names from 'root-config' output
EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
    ARGS --noauxlibs --glibs
    OUTPUT_VARIABLE out_tmp
    RETURN_VALUE out_ret )
# check if everything went ok
IF( NOT out_ret )
    # create a list out of the output
    SEPARATE_ARGUMENTS( out_tmp )
    # remove first item -L compiler flag
    LIST( REMOVE_AT out_tmp 0 )
    # extract libnames from -l compiler flags
    FOREACH( lib ${out_tmp} )
        STRING( REGEX REPLACE "^-.(.*)$" "\\1" libname "${lib}")
        LIST( APPEND ROOT_LIB_NAMES ${libname} )
    ENDFOREACH()

    # check for ROOT Minuit2
    EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
        ARGS --has-minuit2
        OUTPUT_VARIABLE has_minuit
        RETURN_VALUE has_minuit_ret )
    IF( NOT has_minuit_ret )
        # determine if Minuit2 is included with ROOT
        IF( has_minuit STREQUAL "yes" )
            LIST( APPEND ROOT_LIB_NAMES "Minuit2" )
        ENDIF()
    ENDIF()

    IF( NOT ROOT_FIND_QUIETLY )
        MESSAGE( STATUS "Check for ROOT: detected libraries: ${ROOT_LIB_NAMES}" )
    ENDIF()
ELSE()
    #IF( NOT ROOT_FIND_QUIETLY )
    #    MESSAGE( STATUS "Error trying to execute root-config --noauxlibs --glibs!!" )
    #    MESSAGE( STATUS "${out_tmp}" )
    #ENDIF()
    # ensure that detection fails
    SET( ROOT_FINDLIB_FAILED TRUE )
ENDIF()

# also try to get library dir from root-config
EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
    ARGS --libdir
    OUTPUT_VARIABLE ROOT_LIB_DIR
    RETURN_VALUE libdir_ret )
# if command fails clean up variable
IF( libdir_ret )
    SET( ROOT_LIB_DIR )
ENDIF()

# find libraries
FOREACH( libname ${ROOT_LIB_NAMES} )

    SET( ROOT_LIB_${libname} ROOT_LIB_${libname}-NOTFOUND )
    MARK_AS_ADVANCED( ROOT_LIB_${libname} )

    FIND_LIBRARY( ROOT_LIB_${libname}
        NAMES ${libname}
        PATHS ${ROOT_HOME}/lib ${ROOT_LIB_DIR}
        NO_DEFAULT_PATH )

    IF( NOT ROOT_LIB_${libname} )
        SET( ROOT_FINDLIB_FAILED TRUE )
        IF( NOT ROOT_FIND_QUIETLY )
            MESSAGE( STATUS "Check for ROOT: ${ROOT_HOME}"
                    " -- failed to find ROOT ${libname} library!!" )
        ENDIF()
    ELSE()
        LIST( APPEND ROOT_LIBS ${ROOT_LIB_${libname}} )
    ENDIF()
ENDFOREACH()

FIND_LIBRARY( DL_LIB NAMES ${CMAKE_DL_LIBS} dl )
IF( NOT DL_LIB AND NOT ROOT_FIND_QUIETLY )
    MESSAGE( STATUS "Check for ROOT: failed to find libdl.so" )
ELSE()
    MESSAGE( STATUS "Check for ROOT: using dl library: ${DL_LIB}" )
ENDIF()

# set variables and display results
IF( ROOT_INCLUDE_DIR AND NOT ROOT_FINDLIB_FAILED AND DL_LIB )
    SET( ROOT_FOUND TRUE )
    SET( ROOT_INCLUDE_DIRS ${ROOT_INCLUDE_DIR} )
    #SET( ROOT_LIBRARIES ${ROOT_LIBS} ${DL_LIB} ${CMAKE_THREAD_LIBS_INIT} )
    SET( ROOT_LIBRARIES ${ROOT_LIBS} ${DL_LIB} )
    SET( ROOT_DEFINITIONS "-DUSEROOT -DUSE_ROOT -DMARLIN_USE_ROOT" )
    MARK_AS_ADVANCED( ROOT_LIBRARIES ROOT_DEFINITIONS )
    MESSAGE( STATUS "Check for ROOT: ${ROOT_HOME} -- works" )
ELSE()
    IF( ROOT_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "did you set ROOT_HOME with -DROOT_HOME=<path_to_ROOT>?" )
    ENDIF()
    IF( NOT ROOT_FIND_QUIETLY )
        MESSAGE( STATUS "Check for ROOT -- failed!! skip this package..." )
    ENDIF()
ENDIF()
