cmake_minimum_required(VERSION 3.26)
project(fif)

option(FIF_BUILD_FIFED "Build fifed" ON)
option(FIF_BUILD_CORE "Build the core module" ON)
option(FIF_BUILD_GFX "Build the gfx module" ON)
option(FIF_BUILD_INPUT "Build the input module" ON)
option(FIF_BUILD_IMGUI "Build the imgui module" ON)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(EMSCRIPTEN_LFLAGS "-sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sASSERTIONS=1 -sWASM=1 -sASYNCIFY")

add_compile_options(-fPIC -fno-exceptions)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options(-fcolor-diagnostics)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-O2)
  if(NOT EMSCRIPTEN)
    add_compile_definitions(FIF_PROFILING_ENABLED)
  endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(EMSCRIPTEN)
    add_compile_options(-Os)
  else()
    add_compile_options(-O3)
  endif()
endif()

function(build_module target dir libs) 
  FILE(GLOB_RECURSE SOURCES ${SRC_DIR}/${dir}/*.cpp)
  add_library(${target} STATIC ${SOURCES})

  set_target_properties(${target} PROPERTIES PUBLIC_HEADER ${INC_DIR}/fif/${dir})
  target_include_directories(${target} PUBLIC ${INC_DIR} ${INC_DIR}/fif/${dir} PRIVATE ${SRC_DIR}/${dir})
  target_link_libraries(${target} ${libs})

  target_compile_options(${target} PUBLIC -Wall -Wextra)

  if(EMSCRIPTEN)
    set_target_properties(${target} PROPERTIES LINK_FLAGS "${EMSCRIPTEN_LFLAGS}")
  endif()
endfunction()

if(FIF_BUILD_CORE)
  set(LIBS "")
  list(APPEND LIBS PUBLIC glfw glm)

  if(NOT EMSCRIPTEN)
    list(APPEND LIBS glad)
  endif()

  build_module(fif-core core "${LIBS}")
endif()

if(FIF_BUILD_GFX)
  build_module(fif-gfx gfx "fif-core")
endif()

if(FIF_BUILD_INPUT)
  build_module(fif-input input "fif-core")
endif()

if(FIF_BUILD_IMGUI)
  build_module(fif-imgui imGui "fif-core;ImGui")
endif()

if(FIF_BUILD_FIFED)
  add_subdirectory(fifed)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW docs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Install GLFW")

if(NOT EMSCRIPTEN)
  add_subdirectory(${LIB_DIR}/glfw)
  add_subdirectory(${LIB_DIR}/glad)
endif()

add_subdirectory(${LIB_DIR}/glm)

set(IMGUI_PATH "vendor/imgui")

file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    ${IMGUI_PATH}/backends/imgui_impl_glfw.h
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp
)

add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
set_target_properties("ImGui" PROPERTIES LINK_FLAGS "${LFLAGS}")
