# set default install prefix to project root directory
# instead of the cmake default /usr/local

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
 	SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)


