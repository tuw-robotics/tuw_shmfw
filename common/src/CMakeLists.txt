configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_library(tuw_shmfw 
  shmfw.cpp
  handler.cpp
  header.cpp
  variable.cpp
  handler_object.cpp
  string_cast.cpp
)

find_package(Boost REQUIRED COMPONENTS date_time thread program_options)
find_package(Threads REQUIRED)

target_include_directories(tuw_shmfw PUBLIC
  $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>)

target_link_libraries(tuw_shmfw PUBLIC
  Boost::date_time
  Boost::thread
  Boost::program_options
  Threads::Threads)

install(TARGETS tuw_shmfw
  # IMPORTANT: Add the tuw_shmfw library to the "export-set"
  EXPORT TUWShmFwTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/tuw_shmfw"
    COMPONENT dev)
