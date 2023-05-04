cmake_minimum_required(VERSION 3.26)
project(fif-root)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_BUILD_PARALLEL_LEVEL 4)
set(OPTIMIZE_DEPENDENCIES ON)

if(CMAKE_COMPILER_IS_GNUCXX)
   add_compile_options(-fdiagnostics-color=always)
endif()

add_subdirectory(fif)
add_subdirectory(fifed)
