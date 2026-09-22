
find_package(Curses REQUIRED)

add_executable(shmfw_editor main_editor.cpp)
target_include_directories(shmfw_editor PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(shmfw_editor PRIVATE tuw_shmfw ${CURSES_LIBRARIES})


install(TARGETS shmfw_editor
  # IMPORTANT: Add the executables to the "export-set"
  EXPORT TUWShmFwTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
  
