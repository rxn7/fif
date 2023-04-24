cmake_minimum_required(VERSION 3.26)
project(fif-root)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-fdiagnostics-color=always)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(fif)
add_subdirectory(fifed)
