#############################################################
# cmake module for finding CLHEP
#
# returns:
#   CLHEP_FOUND        : set to TRUE or FALSE
#   CLHEP_VERSION      : package version
#   CLHEP_INCLUDE_DIRS : paths to clhep includes
#   CLHEP_LIBRARY_DIRS : paths to clhep libraries
#   CLHEP_LIBRARIES    : list of clhep libraries
#
# @author Jan Engels, DESY
#############################################################


# find clhep-config
SET( CLHEP_CONFIG_EXECUTABLE CLHEP_CONFIG_EXECUTABLE-NOTFOUND )
MARK_AS_ADVANCED( CLHEP_CONFIG_EXECUTABLE )
FIND_PROGRAM( CLHEP_CONFIG_EXECUTABLE clhep-config PATHS ${CLHEP_DIR}/bin NO_DEFAULT_PATH )
IF( NOT CLHEP_DIR )
    FIND_PROGRAM( CLHEP_CONFIG_EXECUTABLE clhep-config )
ENDIF()

IF( CLHEP_CONFIG_EXECUTABLE )

    # ==============================================
    # ===          CLHEP_PREFIX                  ===
    # ==============================================

    EXECUTE_PROCESS( COMMAND "${CLHEP_CONFIG_EXECUTABLE}" --prefix
        OUTPUT_VARIABLE CLHEP_ROOT
        RESULT_VARIABLE _exit_code
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( NOT _exit_code EQUAL 0 )
        SET( CLHEP_ROOT )
    ENDIF()


    # ==============================================
    # ===          CLHEP_VERSION                 ===
    # ==============================================
    INCLUDE( MacroCheckPackageVersion )

    EXECUTE_PROCESS( COMMAND "${CLHEP_CONFIG_EXECUTABLE}" --version
        OUTPUT_VARIABLE _output
        RESULT_VARIABLE _exit_code
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( _exit_code EQUAL 0 )
        #SEPARATE_ARGUMENTS( CLHEP_VERSION UNIX_COMMAND "${_output}" ) # needs cmake >= 2.8
        SET( CLHEP_VERSION ${_output} )
        SEPARATE_ARGUMENTS( CLHEP_VERSION )
        LIST( REMOVE_AT CLHEP_VERSION 0 ) # remove CLHEP string from output of 'clhep-config --version'
        CHECK_PACKAGE_VERSION( CLHEP ${CLHEP_VERSION} )
    ELSE()
        SET( CLHEP_VERSION )
    ENDIF()

ENDIF( CLHEP_CONFIG_EXECUTABLE )


# ---------- includes ---------------------------------------------------------
SET( CLHEP_INCLUDE_DIRS CLHEP_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( CLHEP_INCLUDE_DIRS )

FIND_PATH( CLHEP_INCLUDE_DIRS NAMES CLHEP/Vector/ThreeVector.h PATHS ${CLHEP_DIR}/include NO_DEFAULT_PATH )
IF( NOT CLHEP_DIR )
    FIND_PATH( CLHEP_INCLUDE_DIRS NAMES CLHEP/Vector/ThreeVector.h )
ENDIF()


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( CLHEP CLHEP )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set CLHEP_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CLHEP DEFAULT_MSG CLHEP_INCLUDE_DIRS CLHEP_LIBRARIES PACKAGE_VERSION_COMPATIBLE )

