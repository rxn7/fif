cmake_minimum_required(VERSION 3.26)
project(fifed)

set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE FIFED_SOURCES ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${FIFED_SOURCES})

target_link_libraries(${PROJECT_NAME} fif-core fif-gfx fif-input fif-imgui)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_CLANG_TIDY clang-tidy --fix-errors --checks=-*,readability-*)

if(EMSCRIPTEN)
  set(SHELL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/emsc/index.html")

  # TODO: Recompile when shell file has changed

  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file=${SHELL_PATH}")
  set(LFLAGS "-sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sASSERTIONS=1 -sWASM=1 -sASYNCIFY")
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LFLAGS}")
endif()
