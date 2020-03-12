#  When building, don't use the install RPATH already (but later on when installing)
set(CMAKE_SKIP_BUILD_RPATH FALSE)         # don't skip the full RPATH for the build tree
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) # use always the build RPATH for the build tree
set(CMAKE_MACOSX_RPATH TRUE)              # use RPATH for MacOSX
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # point to directories outside the build tree to the install RPATH

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR}) # install LIBDIR
set(CMAKE_SKIP_INSTALL_RPATH FALSE)          # don't skip the full RPATH for the install tree
if(APPLE)
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_INSTALL_RPATH "@loader_path/../lib")    # self relative LIBDIR
  set(CMAKE_SKIP_INSTALL_RPATH FALSE)          # don't skip the full RPATH for the install tree
endif()

