add_executable(shmfw_usage_var usage_var.cpp)
target_link_libraries(shmfw_usage_var PRIVATE tuw_shmfw)

add_executable(shmfw_usage_vector usage_vector.cpp)
target_link_libraries(shmfw_usage_vector PRIVATE tuw_shmfw)

install(TARGETS shmfw_usage_var shmfw_usage_vector
  RUNTIME DESTINATION "lib/${PROJECT_NAME}")
  
