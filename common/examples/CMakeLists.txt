add_executable(shmfw_demo demo.cpp)
target_link_libraries(shmfw_demo PRIVATE tuw_shmfw)

add_executable(demo_string_cast demo_string_cast.cpp)
target_link_libraries(demo_string_cast PRIVATE tuw_shmfw)

add_executable(shmfw_usage_var usage_var.cpp)
target_link_libraries(shmfw_usage_var PRIVATE tuw_shmfw)

add_executable(shmfw_usage_vector usage_vector.cpp)
target_link_libraries(shmfw_usage_vector PRIVATE tuw_shmfw)

install(TARGETS shmfw_demo demo_string_cast shmfw_usage_var shmfw_usage_vector
  # IMPORTANT: Add the executables to the "export-set"
  EXPORT TUWShmFwTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
  
