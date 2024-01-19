cmake_minimum_required(VERSION 3.26)
project(fifed)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyfd.cmake)

set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FIF_BUILD_NATIVE_SCRIPTING OFF)
set(FIF_ENABLE_MOUSE_PICKING ON)

file(GLOB_RECURSE FIFED_SOURCES ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${FIFED_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)
target_link_libraries(${PROJECT_NAME} fif-core tinyfd fif-gfx fif-input fif-imgui fif-lua_scripting)
add_compile_options(-fno-stack-protector)

if(WIN32)
  target_link_options(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++)
  target_link_libraries(${PROJECT_NAME} comdlg32 ole32 -static pthread)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
  )
endif()

add_custom_target(copy_fifed_assets
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE ${CMAKE_CURRENT_BINARY_DIR}/LICENSE
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/default_layout.ini ${CMAKE_CURRENT_BINARY_DIR}/default_layout.ini
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_fifed_assets)
