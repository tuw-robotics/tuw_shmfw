add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(examples)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.hpp")
