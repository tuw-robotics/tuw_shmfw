cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(tuw_shmfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ament_cmake REQUIRED)

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

add_library(tuw_shmfw
  src/tuw_shmfw/shmfw.cpp
  src/tuw_shmfw/handler.cpp
  src/tuw_shmfw/header.cpp
  src/tuw_shmfw/variable.cpp
  src/tuw_shmfw/handler_object.cpp
  src/tuw_shmfw/string_cast.cpp
)

find_package(Boost REQUIRED COMPONENTS date_time thread program_options)
find_package(Threads REQUIRED)

target_include_directories(tuw_shmfw PUBLIC
  $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(tuw_shmfw PUBLIC
  Boost::date_time
  Boost::thread
  Boost::program_options
  Threads::Threads)

install(TARGETS tuw_shmfw
  EXPORT TUWShmFwTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)

add_subdirectory(src/apps)
add_subdirectory(src/examples)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.hpp")


ament_export_targets(TUWShmFwTargets HAS_LIBRARY_TARGET)
ament_export_dependencies(Boost)

ament_package()
