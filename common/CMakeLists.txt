include_directories(include)

set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(examples)

INSTALL(DIRECTORY include/  DESTINATION include FILES_MATCHING PATTERN "*.hpp")
