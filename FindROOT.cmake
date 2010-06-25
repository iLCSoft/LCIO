###############################################################################
# cmake module for finding ROOT
#
# Following cmake variables are returned by this module:
#
#   ROOT_FOUND              # set to TRUE if ROOT was successfully found
#   ROOT_INCLUDE_DIRS       # list of directories where ROOT headers live
#   ROOT_LIBRARIES          # list of all ROOT libraries (including components)
#   ROOT_DEFINITIONS        # definitions set by this module (-DUSE_ROOT ...)
#   ROOT_${COMPONENT}_FOUND # for ROOT components, e.g. Minuit2 MathMore ...
#   
#
#   Please note that by convention components should be entered exactly as
#   the library names, i.e. the component name equivalent to the library
#   $ROOTSYS/lib/libMathMore.so should be called MathMore and NOT:
#       mathmore or Mathmore or MATHMORE
#
#   However to follow the usual cmake convention it is agreed that the
#   ROOT_${COMPONENT}_FOUND variables are ALL uppercase, i.e. the MathMore
#   component returns: ROOT_MATHMORE_FOUND and NOT ROOT_MathMore_FOUND
#
#
# The additional ROOT components can be defined directly in the cmake commando:
# FIND_PACKAGE( ROOT COMPONENTS MathMore Gdml Geo ...)
#
# Or in the variable ROOT_USE_COMPONENTS before calling find_package, i.e.:
# SET( ROOT_USE_COMPONENTS MathMore Gdml Geo )
# FIND_PACKAGE( ROOT )
#
# The Minuit2 component is always added for backwards compatibility.
#
# @author Jan Engels, DESY
###############################################################################



SET( ROOT_FOUND FALSE )
MARK_AS_ADVANCED( ROOT_FOUND )

# Threads library is needed for root
#FIND_PACKAGE( Threads REQUIRED)

# set ROOTSYS for running root-config
IF( DEFINED ROOT_HOME )
    SET( ENV{ROOTSYS} "${ROOT_HOME}" )
ENDIF()




# ==============================================
# ===          ROOT_INCLUDE_DIR              ===
# ==============================================

# get include dir from root-config output
EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
    ARGS --incdir
    OUTPUT_VARIABLE ROOT_INC_DIR
    RETURN_VALUE exit_code
)
IF( NOT exit_code EQUAL 0 )
    # clear ROOT_INC_DIR if root-config exits with error
    # it could have garbage output
    SET( ROOT_INC_DIR )
ENDIF()


SET( ROOT_INCLUDE_DIR ROOT_INCLUDE_DIR-NOTFOUND )
MARK_AS_ADVANCED( ROOT_INCLUDE_DIR )

FIND_PATH( ROOT_INCLUDE_DIR
    NAMES TH1.h
    PATHS ${ROOT_HOME}/include ${ROOT_INC_DIR}
    NO_DEFAULT_PATH )

IF( NOT ROOT_INCLUDE_DIR AND NOT ROOT_FIND_QUIETLY )
    MESSAGE( STATUS "Check for ROOT: ${ROOT_HOME}"
            " -- failed to find ROOT include directory!!" )
ENDIF()




# ==============================================
# ===            ROOT_LIBRARIES              ===
# ==============================================

# check if this flag is set to FALSE at the end of
# this module to make sure all libraries were found
SET( ROOT_FINDLIB_FAILED FALSE )
MARK_AS_ADVANCED( ROOT_FINDLIB_FAILED )


# get library dir from root-config output
EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
    ARGS --libdir
    OUTPUT_VARIABLE ROOT_LIB_DIR
    RETURN_VALUE exit_code
)
IF( NOT exit_code EQUAL 0 )
    # clear ROOT_LIB_DIR if root-config exits with error
    # it could have garbage output
    SET( ROOT_LIB_DIR )
ENDIF()



# ========== look for standard root libraries =================

# standard root libraries (without components)
SET( ROOT_LIB_NAMES )
SET( ROOT_LIBS )
MARK_AS_ADVANCED( ROOT_LIB_NAMES ROOT_LIBS )

# get standard root libraries from 'root-config --libs' output
EXEC_PROGRAM( "${ROOT_HOME}/bin/root-config" "${ROOT_HOME}/bin"
    #ARGS --noauxlibs --glibs
    ARGS --noauxlibs --libs
    OUTPUT_VARIABLE cmd_output
    RETURN_VALUE exit_code
)
IF( exit_code EQUAL 0 )
    
    # create a list out of the output
    SEPARATE_ARGUMENTS( cmd_output )

    # remove first item -L compiler flag
    LIST( REMOVE_AT cmd_output 0 )

    FOREACH( lib ${cmd_output} )

        # extract libnames from -l compiler flags
        STRING( REGEX REPLACE "^-.(.*)$" "\\1" libname "${lib}")

        # append all library names into a list
        LIST( APPEND ROOT_LIB_NAMES ${libname} )

        SET( ROOT_LIB_${libname} ROOT_LIB_${libname}-NOTFOUND )
        MARK_AS_ADVANCED( ROOT_LIB_${libname} )

        FIND_LIBRARY( ROOT_LIB_${libname}
            NAMES ${libname}
            PATHS ${ROOT_HOME}/lib ${ROOT_LIB_DIR}
            NO_DEFAULT_PATH
        )

        IF( NOT ROOT_LIB_${libname} )
            SET( ROOT_FINDLIB_FAILED TRUE )
            IF( NOT ROOT_FIND_QUIETLY )
                MESSAGE( STATUS "Check for ROOT: ${ROOT_HOME}"
                        " -- failed to find ROOT library: ${libname}" )
            ENDIF()
        ELSE()
            LIST( APPEND ROOT_LIBS ${ROOT_LIB_${libname}} )
        ENDIF()

    ENDFOREACH()

    IF( NOT ROOT_FIND_QUIETLY )
        MESSAGE( STATUS "Check for ROOT: detected libraries: ${ROOT_LIB_NAMES}" )
    ENDIF()

ELSE()
    SET( ROOT_FINDLIB_FAILED TRUE )
ENDIF()



# ========== look for additional root components =================

SET( ROOT_COMPONENT_LIBS )
MARK_AS_ADVANCED( ROOT_COMPONENT_LIBS )

# Minuit2 is always included (for backwards compatibility )
LIST( APPEND ROOT_FIND_COMPONENTS Minuit2 )

# append components defined in the variable ROOT_USE_COMPONENTS
IF( DEFINED ROOT_USE_COMPONENTS )
    LIST( APPEND ROOT_FIND_COMPONENTS ${ROOT_USE_COMPONENTS} )
ENDIF()

# REMOVE_DUPLICATES is only available in cmake versions >= 2.6
# it is not a problem if a component is duplicated in the list, this is just done
# for consistency and to display the message below without duplicate components
IF( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.6 OR
    ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6 )
    IF( ROOT_FIND_COMPONENTS )
        LIST(REMOVE_DUPLICATES ROOT_FIND_COMPONENTS)
    ENDIF()
ENDIF()

IF( NOT ROOT_FIND_QUIETLY )
    MESSAGE( STATUS "Check for ROOT: additional components: ${ROOT_FIND_COMPONENTS}" )
ENDIF()

FOREACH( libname ${ROOT_FIND_COMPONENTS} )

    # name of the component in upper case
    STRING( TOUPPER ${libname} upper_component_name)

    SET( ROOT_LIB_${libname} ROOT_LIB_${libname}-NOTFOUND )
    MARK_AS_ADVANCED( ROOT_LIB_${libname} )

    FIND_LIBRARY( ROOT_LIB_${libname}
        NAMES ${libname}
        PATHS ${ROOT_HOME}/lib ${ROOT_LIB_DIR}
        NO_DEFAULT_PATH
    )

    IF( NOT ROOT_LIB_${libname} )
        #SET( ROOT_FINDLIB_FAILED TRUE )
        IF( NOT ROOT_FIND_QUIETLY )
            MESSAGE( STATUS "Check for ROOT: ${ROOT_HOME}"
                    " -- failed to find ROOT component: ${libname}" )
        ENDIF()
    ELSE()
        SET( ROOT_${upper_component}_FOUND TRUE )
        LIST( APPEND ROOT_COMPONENT_LIBS ${ROOT_LIB_${libname}} )
    ENDIF()
ENDFOREACH()


# ====== DL LIBRARY ==================================================
# comment out this code due to cmake bug in 64 bit:
# see: http://public.kitware.com/mantis/view.php?id=10813
#
#FIND_LIBRARY( DL_LIB NAMES ${CMAKE_DL_LIBS} dl )
#IF( NOT DL_LIB AND NOT ROOT_FIND_QUIETLY )
#    MESSAGE( STATUS "Check for ROOT: failed to find libdl.so" )
#    SET( ROOT_FINDLIB_FAILED TRUE )
#ELSE()
#    MESSAGE( STATUS "Check for ROOT: using dl library: ${DL_LIB}" )
#ENDIF()



# set variables and display results
IF( ROOT_INCLUDE_DIR AND NOT ROOT_FINDLIB_FAILED )
    SET( ROOT_FOUND TRUE )
    SET( ROOT_INCLUDE_DIRS ${ROOT_INCLUDE_DIR} )
    #SET( ROOT_LIBRARIES ${ROOT_LIBS} ${DL_LIB} ${CMAKE_THREAD_LIBS_INIT} )
    #SET( ROOT_LIBRARIES ${ROOT_LIBS} ${ROOT_COMPONENT_LIBS} ${DL_LIB} )
    SET( ROOT_LIBRARIES ${ROOT_LIBS} ${ROOT_COMPONENT_LIBS} )
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

