##################################################################################
# cmake module for finding JAVA
#
# sets following variables:
#
# JAVA_FOUND    - TRUE/FALSE (javadoc is ignored for the test)
#
# JAVA_HOME     - base dir where subdirs bin, lib, jre and include are located
# JAVA_BIN_PATH - path to subdirectory "${JAVA_HOME}/bin"
#
# JAVA_RUNTIME  - path to ${JAVA_BIN_PATH}/java
# JAVA_COMPILE  - path to ${JAVA_BIN_PATH}/javac
# JAVA_ARCHIVE  - path to ${JAVA_BIN_PATH}/jar
# JAVA_DOC      - path to ${JAVA_BIN_PATH}/javadoc
#
# JAVA_MAJOR_VERSION
# JAVA_MINOR_VERSION
# JAVA_PATCH_LEVEL
#
# JAVA_VERSION - ${JAVA_MAJOR_VERSION}.${JAVA_MINOR_VERSION}.${JAVA_PATCH_LEVEL}
#
# @author Jan Engels, DESY
##################################################################################

SET( JAVA_FOUND FALSE )
MARK_AS_ADVANCED( JAVA_FOUND )

IF( NOT DEFINED ENV{JDK_HOME} AND 
    NOT DEFINED ENV{JAVA_HOME} AND
    NOT DEFINED JAVA_HOME AND
    NOT DEFINED JDK_HOME )

    IF( NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "Autodetecting Java..." )
    ENDIF()
    
    # use the CMake FindJava.cmake module
    FIND_PACKAGE( Java )
    
    IF( JAVA_RUNTIME AND JAVA_COMPILE AND JAVA_ARCHIVE )

        IF( UNIX AND NOT APPLE )
            # look for the readlink binary
            FIND_PROGRAM( READLINK_BIN readlink )
            MARK_AS_ADVANCED( READLINK_BIN )
            
            IF( READLINK_BIN )

                # dereference links
                EXEC_PROGRAM( ${READLINK_BIN} ARGS "-f ${JAVA_COMPILE}"
                    OUTPUT_VARIABLE javac_dereferenced
                    RETURN_VALUE return_value )

                IF( NOT return_value )
                    GET_FILENAME_COMPONENT( JAVA_BIN_PATH ${javac_dereferenced} PATH )
                ELSE()
                    GET_FILENAME_COMPONENT( JAVA_BIN_PATH ${JAVA_COMPILE} PATH )
                ENDIF()
            ENDIF()

            GET_FILENAME_COMPONENT( JAVA_HOME ${JAVA_BIN_PATH} PATH )

        ENDIF()
    ELSE()
        IF( NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Failed to autodetect Java!!" )
        ENDIF()
    ENDIF()
ELSE()
    # definition of JAVA_HOME or JDK_HOME in cmake has priority over env vars
    IF( DEFINED JDK_HOME OR DEFINED JAVA_HOME )
        # ensure that both variables are set correctly (JDK_HOME as well as JAVA_HOME)
        IF( DEFINED JDK_HOME AND DEFINED JAVA_HOME )
            IF( NOT "${JDK_HOME}" STREQUAL "${JAVA_HOME}" )
                IF( NOT JAVA_FIND_QUIETLY )
                    MESSAGE( STATUS 
                        "WARNING: JDK_HOME and JAVA_HOME are set to different paths!!" )
                    MESSAGE( STATUS "JDK_HOME: ${JDK_HOME}" )
                    MESSAGE( STATUS "JAVA_HOME: ${JAVA_HOME}" )
                    MESSAGE( STATUS "${JAVA_HOME} will be used in this installation!!" )
                ENDIF()
            ENDIF()
        ELSE()
            IF( NOT DEFINED JAVA_HOME )
                SET( JAVA_HOME "${JDK_HOME}" )
            ENDIF()
        ENDIF()
    ELSE()
        # in case JDK_HOME or JAVA_HOME already set ensure that both variables
        # are set correctly (JDK_HOME as well as JAVA_HOME)
        IF( DEFINED ENV{JDK_HOME} AND DEFINED ENV{JAVA_HOME} )
            IF( NOT "$ENV{JDK_HOME}" STREQUAL "$ENV{JAVA_HOME}" )
                IF( NOT JAVA_FIND_QUIETLY )
                    MESSAGE( STATUS 
                        "WARNING: JDK_HOME and JAVA_HOME are set to different paths!!" )
                    MESSAGE( STATUS "JDK_HOME: $ENV{JDK_HOME}" )
                    MESSAGE( STATUS "JAVA_HOME: $ENV{JAVA_HOME}" )
                    MESSAGE( STATUS "$ENV{JAVA_HOME} will be used in this installation!!" )
                ENDIF()
            ENDIF()
            SET( JAVA_HOME "$ENV{JAVA_HOME}" )
        ELSE()
            IF( DEFINED ENV{JAVA_HOME} )
                SET( JAVA_HOME "$ENV{JAVA_HOME}" )
            ENDIF()
            IF( DEFINED ENV{JDK_HOME} )
                SET( JAVA_HOME "$ENV{JDK_HOME}" )
            ENDIF()
        ENDIF()
    ENDIF()

    SET( JAVA_BIN_PATH JAVA_BIN_PATH-NOTFOUND )
    FIND_PATH( JAVA_BIN_PATH
        javac
        ${JAVA_HOME}/bin
        ${JAVA_HOME}/Commands   # FIXME MacOS
        NO_DEFAULT_PATH )
    
    IF( NOT JAVA_BIN_PATH AND NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "${JAVA_HOME} is not a valid path for Java!!" )
    ENDIF()

    IF( JAVA_BIN_PATH )
        # find java, javac, jar and javadoc
        SET( JAVA_RUNTIME JAVA_RUNTIME-NOTFOUND )
        FIND_PROGRAM( JAVA_RUNTIME
            java
            ${JAVA_BIN_PATH}
            NO_DEFAULT_PATH )
      
        SET( JAVA_COMPILE JAVA_COMPILE-NOTFOUND )
        FIND_PROGRAM( JAVA_COMPILE
            javac
            ${JAVA_BIN_PATH}
            NO_DEFAULT_PATH )
        
        SET( JAVA_ARCHIVE JAVA_ARCHIVE-NOTFOUND )
        FIND_PROGRAM( JAVA_ARCHIVE
            jar
            ${JAVA_BIN_PATH}
            NO_DEFAULT_PATH )
         
        SET( JAVA_DOC JAVA_DOC-NOTFOUND )
        FIND_PROGRAM( JAVA_DOC
            javadoc
            ${JAVA_BIN_PATH}
            NO_DEFAULT_PATH )
        
        # abort if not found
        IF( NOT JAVA_RUNTIME AND NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Could not find java!!" )
        ENDIF()
        IF( NOT JAVA_COMPILE AND NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Could not find javac!!" )
        ENDIF()
        IF( NOT JAVA_ARCHIVE AND NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Could not find jar!!" )
        ENDIF()
        IF( NOT JAVA_DOC AND NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Could not find javadoc!!" )
        ENDIF()
    ENDIF()
ENDIF()

IF( JAVA_RUNTIME AND JAVA_COMPILE AND JAVA_ARCHIVE )
    
    SET( JAVA_FOUND TRUE )
    
    # parse the output of java -version
    EXEC_PROGRAM( "${JAVA_RUNTIME}" ARGS "-version"
            OUTPUT_VARIABLE out_tmp
            RETURN_VALUE out_ret )

    IF( out_ret )
        IF( NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Error executing java -version!! Java version variables will not be set!!" )
        ENDIF()
    ELSE()
        # extract major/minor version and patch level from "java -version" output
        STRING( REGEX REPLACE ".* version \"([0-9]+)\\.[0-9]+\\.[0-9]+.*"
                "\\1" JAVA_MAJOR_VERSION "${out_tmp}" )
        STRING( REGEX REPLACE ".* version \"[0-9]+\\.([0-9]+)\\.[0-9]+.*"
                "\\1" JAVA_MINOR_VERSION "${out_tmp}" )
        STRING( REGEX REPLACE ".* version \"[0-9]+\\.[0-9]+\\.([0-9]+).*"
                "\\1" JAVA_PATCH_LEVEL "${out_tmp}" )

        SET( JAVA_VERSION "${JAVA_MAJOR_VERSION}.${JAVA_MINOR_VERSION}.${JAVA_PATCH_LEVEL}" )

        # display info
        MESSAGE( STATUS "Java version ${JAVA_VERSION} configured successfully!" )
    ENDIF()
ELSE()
    IF( JAVA_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Failed configuring Java!!" )
    ENDIF()
    IF( NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "Failed configuring Java!!" )
    ENDIF()
ENDIF()

