cmake_minimum_required(VERSION 3.26)
project(fifed)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyfd.cmake)

set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE FIFED_SOURCES ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${FIFED_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)
target_link_libraries(${PROJECT_NAME} fif-core fif-gfx fif-input fif-imgui fif-lua_scripting fif-native_scripting tinyfd)

add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/default_imgui.ini ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)
