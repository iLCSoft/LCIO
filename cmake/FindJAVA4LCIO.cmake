##################################################################################
# cmake module for finding JAVA
#
#   looks in following locations in the following order:
#       JAVA_DIR, JAVA_HOME, JDK_HOME, ENV{JAVA_HOME}, ENV{JDK_HOME}
#
# sets following variables:
#
# JAVA_FOUND    - set to TRUE or FALSE (javadoc is ignored for the test)
#
# JAVA_DIR      - base dir where subdirs bin, lib, jre and include are located
# JAVA_HOME     - same as JAVA_DIR
# JAVA_BIN_PATH - path to subdirectory "${JAVA_DIR}/bin"
#
# JAVA_RUNTIME  - path to ${JAVA_BIN_PATH}/java
# JAVA_COMPILE  - path to ${JAVA_BIN_PATH}/javac
# JAVA_ARCHIVE  - path to ${JAVA_BIN_PATH}/jar
# JAVA_DOC      - path to ${JAVA_BIN_PATH}/javadoc
#
# JAVA_VERSION_MAJOR
# JAVA_VERSION_MINOR
# JAVA_VERSION_PATCH
# JAVA_VERSION (MAJOR.MINOR.PATCH)
#
# @author Jan Engels, DESY
##################################################################################

SET( JAVA_FOUND FALSE )
MARK_AS_ADVANCED( JAVA_FOUND )

IF( NOT JAVA_DIR AND JAVA_HOME )
    SET( JAVA_DIR "${JAVA_HOME}" )
ENDIF()

IF( NOT JAVA_DIR AND JDK_HOME )
    SET( JAVA_DIR "${JDK_HOME}" )
ENDIF()

IF( NOT JAVA_DIR AND DEFINED ENV{JAVA_HOME} )
    SET( JAVA_DIR "$ENV{JAVA_HOME}" )
ENDIF()

IF( NOT JAVA_DIR AND DEFINED ENV{JDK_HOME} )
    SET( JAVA_DIR "$ENV{JDK_HOME}" )
ENDIF()


IF( NOT JAVA_FIND_QUIETLY )
    MESSAGE( STATUS "Check for Java: ${JAVA_DIR}" )
ENDIF()

IF( JAVA_DIR )

    SET( JAVA_BIN_PATH JAVA_BIN_PATH-NOTFOUND )
    FIND_PATH( JAVA_BIN_PATH
        javac
        ${JAVA_DIR}/bin
        ${JAVA_DIR}/Commands
        NO_DEFAULT_PATH )
    
    IF( NOT JAVA_BIN_PATH AND NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "${JAVA_DIR} is not a valid path for Java!!" )
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


ELSE() # try to autodetect java


    IF( NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "Autodetecting Java..." )
    ENDIF()
    
    # use the CMake FindJava.cmake module
    FIND_PACKAGE( Java )
    
    #MESSAGE( STATUS "JAVA_RUNTIME: ${JAVA_RUNTIME}" )
    #MESSAGE( STATUS "JAVA_COMPILE: ${JAVA_COMPILE}" )
    #MESSAGE( STATUS "JAVA_ARCHIVE: ${JAVA_ARCHIVE}" )
    
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

            GET_FILENAME_COMPONENT( JAVA_DIR ${JAVA_BIN_PATH} PATH )

        ELSE()

            SET( JAVA_HOME_MAC JAVA_HOME_MAC-NOTFOUND )
            FIND_PROGRAM( JAVA_HOME_MAC
                java_home
                /usr/libexec/
            )

            IF( JAVA_HOME_MAC )
     
                EXEC_PROGRAM( "${JAVA_HOME_MAC}"
                    OUTPUT_VARIABLE JAVA_DIR
                    RETURN_VALUE _exit_code
                )

                IF( _exit_code )
                    IF( NOT JAVA_FIND_QUIETLY )
                        MESSAGE( STATUS "Failed to execute java_home" )
                    ENDIF()
                ENDIF()

                SET( JAVA_BIN_PATH JAVA_BIN_PATH-NOTFOUND )
                FIND_PATH( JAVA_BIN_PATH
                    javac
                    ${JAVA_DIR}/bin
                    ${JAVA_DIR}/Commands
                    NO_DEFAULT_PATH )
                
                IF( NOT JAVA_BIN_PATH AND NOT JAVA_FIND_QUIETLY )
                    MESSAGE( STATUS "${JAVA_DIR} is not a valid path for Java!!" )
                ENDIF()

            ELSE()
                IF( NOT JAVA_FIND_QUIETLY )
                    MESSAGE( STATUS "Failed to autodetect Java!!" )
                ENDIF()
            ENDIF()

        ENDIF()
    ELSE()
        IF( NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Failed to autodetect Java!!" )
        ENDIF()
    ENDIF()

ENDIF()

IF( JAVA_DIR )
    SET( JAVA_HOME "${JAVA_DIR}" )
    IF( NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "JAVA_HOME set to ${JAVA_DIR}" )
    ENDIF()
ENDIF()

IF( JAVA_RUNTIME AND JAVA_COMPILE AND JAVA_ARCHIVE )
    
    SET( JAVA_FOUND TRUE )
    
    # parse the output of java -version
    EXEC_PROGRAM( "${JAVA_RUNTIME}" ARGS "-version"
            OUTPUT_VARIABLE out_tmp
            RETURN_VALUE _exit_code )

    IF( _exit_code )
        IF( NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Failed to execute java -version!! JAVA_VERSION will not be set!!" )
        ENDIF()
    ELSE()
        # extract major/minor version and patch level from "java -version" output
        STRING( REGEX REPLACE ".* version \"([0-9]+)\\.[0-9]+\\.[0-9]+.*"
                "\\1" JAVA_VERSION_MAJOR "${out_tmp}" )
        STRING( REGEX REPLACE ".* version \"[0-9]+\\.([0-9]+)\\.[0-9]+.*"
                "\\1" JAVA_VERSION_MINOR "${out_tmp}" )
        STRING( REGEX REPLACE ".* version \"[0-9]+\\.[0-9]+\\.([0-9]+).*"
                "\\1" JAVA_VERSION_PATCH "${out_tmp}" )

        SET( JAVA_VERSION "${JAVA_VERSION_MAJOR}.${JAVA_VERSION_MINOR}.${JAVA_VERSION_PATCH}" )

        # display info
        IF( NOT JAVA_FIND_QUIETLY )
            MESSAGE( STATUS "Found Java (version ${JAVA_VERSION})" )
        ENDIF()
    ENDIF()
ELSE()
    IF( JAVA_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Failed to find Java!!" )
    ENDIF()
    IF( NOT JAVA_FIND_QUIETLY )
        MESSAGE( STATUS "Failed to find Java!!" )
    ENDIF()
ENDIF()

