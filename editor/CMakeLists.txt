cmake_minimum_required(VERSION 3.15)
project(fifed)

set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../core)

FILE(GLOB Sources ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${Sources})
include_directories(${INC_DIR})

target_link_libraries(${PROJECT_NAME} fif-core fif-gfx)
