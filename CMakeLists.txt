cmake_minimum_required(VERSION 3.26)
project(fif-root)

option(FIF_BUILD_FIFED "Build the fif editor" ON)
option(FIF_BUILD_FIF_RUNTIME "Build the fif runtime" ON)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-fdiagnostics-color=always)
endif()

add_compile_definitions(SOL_ALL_SAFETIES_ON)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
  add_compile_options(-fno-omit-frame-pointer)
  add_compile_definitions(FIF_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
  add_compile_definitions(FIF_RELEASE)
endif()

add_subdirectory(fif)

if(FIF_BUILD_FIFED)
  add_subdirectory(fifed)
endif()

if(FIF_BUILD_FIF_RUNTIME)
  add_subdirectory(runtime)
endif()
