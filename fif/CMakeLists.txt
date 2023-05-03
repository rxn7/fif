cmake_minimum_required(VERSION 3.26)
project(fif)

option(FIF_BUILD_CORE "Build the core module" ON)
option(FIF_BUILD_GFX "Build the gfx module" ON)
option(FIF_BUILD_INPUT "Build the input module" ON)
option(FIF_BUILD_IMGUI "Build the imgui module" ON)
option(FIF_BUILD_LUA_SCRIPTING "Build the lua_scripting module" ON)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_compile_options(-fPIC -fno-exceptions)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g3)
  add_compile_definitions(FIF_DEBUG)
  add_compile_definitions(FIF_PROFILING_ENABLED)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(FIF_RELEASE)
  add_compile_options(-O3)
endif()

function(build_module target dir libs) 
  file(GLOB_RECURSE SOURCES ${SRC_DIR}/${dir}/*.cpp)
  add_library(${target} STATIC ${SOURCES})

  set_target_properties(${target} PROPERTIES PUBLIC_HEADER ${INC_DIR}/fif/${dir})
  target_include_directories(${target} PUBLIC ${INC_DIR} ${INC_DIR}/fif/${dir} PRIVATE ${SRC_DIR}/${dir} ${LIB_DIR}/stb)
  target_link_libraries(${target} ${libs})

  target_compile_options(${target} PUBLIC -Wall -Wextra)
endfunction()

if(FIF_BUILD_CORE)
  build_module(fif-core core "glfw;glm;glad;EnTT")
endif()

if(FIF_BUILD_GFX)
  build_module(fif-gfx gfx "fif-core")
endif()

if(FIF_BUILD_INPUT)
  build_module(fif-input input "fif-core")
endif()

if(FIF_BUILD_IMGUI)
  build_module(fif-imgui imgui "fif-core;ImGui")
endif()

if(FIF_BUILD_LUA_SCRIPTING)
  add_compile_definitions(SOL_NO_EXCEPTIONS)
  add_subdirectory(${LIB_DIR}/lua)
  add_subdirectory(${LIB_DIR}/sol2)
  build_module(fif-lua_scripting lua_scripting "fif-core;lua::lib;sol2")
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW docs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Install GLFW")

add_subdirectory(${LIB_DIR}/glfw)
add_subdirectory(${LIB_DIR}/glad)
add_subdirectory(${LIB_DIR}/glm)
add_subdirectory(${LIB_DIR}/entt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/imgui.cmake)
