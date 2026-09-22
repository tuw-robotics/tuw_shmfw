add_executable(shmfw_admin main_admin.cpp)
target_link_libraries(shmfw_admin PRIVATE tuw_shmfw)


install(TARGETS shmfw_admin
  RUNTIME DESTINATION "lib/${PROJECT_NAME}")
  
