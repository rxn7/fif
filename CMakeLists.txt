cmake_minimum_required(VERSION 3.26)
project(fif-root)

if(CMAKE_COMPILER_IS_GNUCXX)
   add_compile_options(-fdiagnostics-color=always)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_compile_definitions(FIF_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
  add_compile_definitions(FIF_RELEASE)
endif()

add_subdirectory(fif)
add_subdirectory(fifed)
