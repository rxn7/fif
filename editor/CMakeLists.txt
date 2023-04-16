cmake_minimum_required(VERSION 3.15)
project(fifed)

set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../core)

FILE(GLOB FIFED_SOURCES ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${FIFED_SOURCES})
include_directories(${INC_DIR})

target_link_libraries(${PROJECT_NAME} fif-core fif-gfx)

if(EMSCRIPTEN)
  set(SHELL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/emsc/shell.html")

  # TODO: Recompile when shell file has changed

  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file=${SHELL_PATH}")
  set(LFLAGS "-sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sASSERTIONS=1 -sWASM=1 -sASYNCIFY")
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LFLAGS}")
endif()
