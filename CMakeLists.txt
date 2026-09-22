cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(TUWShmFw)
set(TUWSHMFW_MAJOR_VERSION 0)
set(TUWSHMFW_MINOR_VERSION 1)
set(TUWSHMFW_PATCH_VERSION 0)
set(TUWSHMFW_VERSION
  ${TUWSHMFW_MAJOR_VERSION}.${TUWSHMFW_MINOR_VERSION}.${TUWSHMFW_PATCH_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/TUWShmFw)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/common/include")

add_subdirectory(common)


# The interesting stuff goes here
# ===============================

# Add all targets to the build-tree export set
export(TARGETS tuw_shmfw shmfw_demo shmfw_usage_var 
  FILE "${PROJECT_BINARY_DIR}/TUWShmFwTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE TUWShmFw)

# Create the TUWShmFwConfig.cmake and TUWShmFwConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_INCLUDE_DIR}")  # local config
configure_file(TUWShmFwConfig.cmake.in
  "${PROJECT_BINARY_DIR}/TUWShmFwConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")  # installed config
configure_file(TUWShmFwConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TUWShmFwConfig.cmake" @ONLY)
# ... for both
configure_file(TUWShmFwConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/TUWShmFwConfigVersion.cmake" @ONLY)

# Install the TUWShmFwConfig.cmake and TUWShmFwConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TUWShmFwConfig.cmake"
  "${PROJECT_BINARY_DIR}/TUWShmFwConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
  
# Install the export set for use with the install-tree
install(EXPORT TUWShmFwTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)


# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
