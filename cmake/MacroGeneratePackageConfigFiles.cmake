#GET_FILENAME_COMPONENT( _current_dir ${CMAKE_CURRENT_LIST_FILE} PATH )

# helper macro for generating project configuration file
MACRO( GENERATE_PACKAGE_CONFIGURATION_FILES )

    FOREACH( arg ${ARGN} )
        IF( ${arg} MATCHES "Config.cmake" )
            IF( EXISTS "${PROJECT_SOURCE_DIR}/cmake/${arg}.in" )
                CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${arg}.in"
                                "lib/cmake/${arg}" @ONLY
                )
            ENDIF()
        ENDIF()


        IF( ${arg} MATCHES "ConfigVersion.cmake" )
            # version configuration file
            IF( EXISTS "${PROJECT_SOURCE_DIR}/cmake/${arg}.in" )
                CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${arg}.in"
                                "lib/cmake/${arg}" @ONLY
                )
            ENDIF()
        ENDIF()

    ENDFOREACH()

ENDMACRO( GENERATE_PACKAGE_CONFIGURATION_FILES )

