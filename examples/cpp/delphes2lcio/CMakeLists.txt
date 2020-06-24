# project name
PROJECT( DELPHES2LCIO )


# project version
SET( DELPHES2LCIO_VERSION_MAJOR 2 )
SET( DELPHES2LCIO_VERSION_MINOR 13 )
SET( DELPHES2LCIO_VERSION_PATCH 3 )

cmake_minimum_required(VERSION 3.10)

# install binary here:
SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(LCIO)
find_package(ROOT)
find_package(Delphes)

# build the executable and put it into bin/
add_executable(DelphesSTDHEP2LCIO ./src/DelphesSTDHEP2LCIO.cpp ./src/DelphesLCIOConverter.cc ./src/DelphesLCIOConfig.cc)
target_include_directories(DelphesSTDHEP2LCIO PRIVATE ${DELPHES_INCLUDE_DIRS} ${LCIO_INCLUDE_DIRS} ./include)
target_link_libraries(DelphesSTDHEP2LCIO ${LCIO_LIBRARIES} ${DELPHES_LIBRARY} ROOT::Core ROOT::RIO ROOT::Tree ROOT::Physics  ROOT::GenVector ROOT::MathCore)
install(TARGETS DelphesSTDHEP2LCIO DESTINATION bin)