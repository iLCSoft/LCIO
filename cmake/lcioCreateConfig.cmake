set(CMAKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake")

include(CMakePackageConfigHelpers)

# Write a version file
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${LCIO_VERSION}
  # LCIO is fully backwards compatible, so any newer version should work
  COMPATIBILITY AnyNewerVersion)

if(NOT SIO_FOUND)
  # Install the SIOTargets similarly to the standalone version to make it easy
  # to use them via the SIO::sio target in dependent projects
  install(EXPORT SIOTargets
          NAMESPACE SIO::
          FILE SIOTargets.cmake
          DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME})
endif()

# Export the LCIO targets
install(EXPORT LCIOTargets
        NAMESPACE LCIO::
        FILE LCIOTargets.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME})

# Build the package config file
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/LCIOConfig.cmake.in
  ${PROJECT_BINARY_DIR}/LCIOConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR)


# install all the necessary config files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LCIOConfigVersion.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/LCIOConfig.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_NAME})
