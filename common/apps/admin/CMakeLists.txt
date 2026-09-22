add_executable(shmfw_admin main_admin.cpp)
target_link_libraries(shmfw_admin PRIVATE tuw_shmfw)


install(TARGETS shmfw_admin
  # IMPORTANT: Add the executables to the "export-set"
  EXPORT TUWShmFwTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
  
