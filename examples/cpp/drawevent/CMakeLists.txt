########################################################
# cmake file for building LCIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
########################################################

# project name
PROJECT( lcio_draw_event )

### DEPENDENCIES ############################################################

#LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )

FIND_PACKAGE( LCIO REQUIRED ) 
FIND_PACKAGE( ROOT 5.27.06 REQUIRED  COMPONENTS Geom) # look for ROOT versions >= 5.27.06

FOREACH( pkg LCIO ROOT )
    IF( ${pkg}_FOUND )
        INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
        #ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
    ENDIF()
ENDFOREACH()

# definitions to pass to the compiler
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
ADD_DEFINITIONS( "-Wno-long-long" )


# include directories
INCLUDE_DIRECTORIES( . )

ADD_EXECUTABLE( lcio_draw_event ./lcio_draw_event.cc )
#SET_TARGET_PROPERTIES( bin PROPERTIES OUTPUT_NAME lcio_draw_events )

TARGET_LINK_LIBRARIES( lcio_draw_event  ${ROOT_COMPONENT_LIBRARIES} )

INSTALL( TARGETS lcio_draw_event DESTINATION . )


# display some variables and write them to cache
DISPLAY_STD_VARIABLES()


