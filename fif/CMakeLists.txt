cmake_minimum_required(VERSION 3.26)
project(fif)

option(FIF_BUILD_CORE "Build the core module" ON)
option(FIF_BUILD_GFX "Build the gfx module" ON)
option(FIF_BUILD_INPUT "Build the input module" ON)
option(FIF_BUILD_IMGUI "Build the imgui module" ON)
option(FIF_BUILD_LUA_SCRIPTING "Build the lua_scripting module" ON)
option(FIF_BUILD_NATIVE_SCRIPTING "Build the native_scripting module" ON)
option(FIF_BUILD_AUDIO "Build the audio module" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_compile_options(-fPIC)
add_compile_options(-ffast-math)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW docs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Install GLFW")
add_subdirectory(${LIB_DIR}/glfw)

add_subdirectory(${LIB_DIR}/glad)
add_subdirectory(${LIB_DIR}/glm)

add_compile_definitions(ENTT_DISABLE_ASSERT)
add_subdirectory(${LIB_DIR}/entt)
add_subdirectory(${LIB_DIR}/yaml-cpp)

if(FIF_RELEASE)
  if(MSVC)
    add_compile_options(/O3)
  else()
    add_compile_options(-O3)
  endif()
else()
  add_compile_options(-g3)
endif()

function(build_module target dir libs)
  file(GLOB_RECURSE SOURCES ${SRC_DIR}/${dir}/*.cpp)
  add_library(${target} STATIC ${SOURCES})

  set_target_properties(${target} PROPERTIES PUBLIC_HEADER ${INC_DIR}/fif/${dir})

  target_include_directories(${target} PUBLIC ${INC_DIR} ${INC_DIR}/fif/${dir} ${SRC_DIR}/${dir})
  target_link_libraries(${target} PUBLIC ${libs})

  if(NOT ${target} MATCHES fif-core)
    target_precompile_headers(${target} REUSE_FROM fif-core)
  endif()

  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /bigobj)
  else()
    target_compile_options(${target} PUBLIC -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Woverloaded-virtual -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel -Wundef -Werror -Wno-unused)
  endif()
endfunction()

if(FIF_BUILD_CORE)
  build_module(fif-core core "yaml-cpp;glfw;glm;glad;EnTT")
  target_precompile_headers(fif-core PUBLIC ${INC_DIR}/fif/pch.hpp)
endif()

if(FIF_BUILD_GFX)
  include_directories(SYSTEM ${LIB_DIR}/stb)
  build_module(fif-gfx gfx "fif-core")
endif()

if(FIF_BUILD_INPUT)
  build_module(fif-input input "fif-core")
endif()

if(FIF_BUILD_IMGUI)
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/imgui.cmake)
  build_module(fif-imgui imgui "fif-core;imgui")
endif()

if(FIF_BUILD_LUA_SCRIPTING)
  add_compile_definitions(SOL_NO_EXCEPTIONS)
  add_subdirectory(${LIB_DIR}/lua)
  add_subdirectory(${LIB_DIR}/sol2)
  build_module(fif-lua_scripting lua_scripting "fif-core;lua::lib;sol2")
endif()

if(FIF_BUILD_NATIVE_SCRIPTING)
  build_module(fif-native_scripting native_scripting "fif-core")
endif()

if(FIF_BUILD_AUDIO)
  include_directories(SYSTEM ${LIB_DIR}/miniaudio)
  build_module(fif-audio audio "fif-core")
endif()